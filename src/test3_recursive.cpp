#include <iostream>
#include <vector>
#include <array>
#include <set>

using namespace std;

uint32_t pack_4(uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4) {
    return n4 | (n3 << 8) | (n2 << 16) | (n1 << 24);
}

array <uint8_t, 4> unpack_4(uint32_t n1n2n3n4) {
    array <uint8_t, 4> data;
    data[0] = (n1n2n3n4 >> 24);
    data[1] = (n1n2n3n4 >> 16) & 0xFF;
    data[2] = (n1n2n3n4 >>  8) & 0xFF;
    data[3] = n1n2n3n4 & 0xFF;
    return data;
}

void print_rect(uint32_t rect) {
    array <uint8_t, 4> coords = unpack_4(rect);
    cout << "(" << int(coords[0]) << "'" << int(coords[1]);
    cout << "'" << int(coords[2]) << "'" << int(coords[3]) << ")" << endl;
}

array <uint16_t, 2> unpack_2(uint32_t num1num2) {
    array <uint16_t, 2> data;
    data[0] = (num1num2 >> 16);
    data[1] = num1num2 & 0xFFFF;
    return data;
}

void check_solution(uint32_t data, set <uint32_t> & prev) {
    // Unpack data
    array <uint8_t, 4> coords = unpack_4(data);
    uint8_t t = coords[0];
    uint8_t n = coords[1];
    uint8_t m = coords[2];
    uint8_t p = coords[3];
    for (auto it = prev.begin(); it != prev.end(); ++it) {
        auto solution = * it;
        array <uint16_t, 2> scores = unpack_2(solution);
        uint16_t square = scores[1];
        uint16_t counter = scores[0];
        if (square > n * m || counter > p) {
            prev.erase(it);
            --it;
        }
    }
}

set <uint32_t> walkthrough(uint32_t data, set <uint32_t> prev, uint32_t rect, vector <uint32_t> & rects) {
    check_solution(data, prev);
    // Unpack info about current rectangle
    // Coords
    array <uint8_t, 4> coords = unpack_4(rect);
    uint8_t x1 = coords[0];
    uint8_t y1 = coords[1];
    uint8_t x2 = coords[2];
    uint8_t y2 = coords[3];
    // Vertices
    uint16_t ll = (x1 << 8) | y1;
    uint16_t lr = (x2 << 8) | y1;
    uint16_t ul = (x1 << 8) | y2;
    uint16_t ur = (x2 << 8) | y2;
    // Square
    uint16_t square = (x2 - x1) * (y2 - y1);
    // Zero-point solution = prev
    uint16_t counter = 1;
    uint32_t score = square | (counter << 16);
    prev.insert(score);

    // Search of children rectangles
    set <uint32_t> candidate_solution;
    // Coords
    uint16_t candidate_ll, candidate_lr, candidate_ul, candidate_ur;
    // Vectors for candidates processing
    vector <uint32_t> candidates_ll, candidates_lr, candidates_ul, candidates_ur;
    // Iterate through the candidates
    for (auto & candidate : rects) {
        // Will not process the same rects
        if (candidate == rect) continue;
        // Unpack info about candidate
        // Coords
        // uint32_t candidate = x1_y1_x2_y2
        uint8_t candidate_x1 = (candidate >> 24);
        uint8_t candidate_y1 = (candidate >> 16) & 0xFF;
        uint8_t candidate_x2 = (candidate >>  8) & 0xFF;
        uint8_t candidate_y2 = candidate & 0xFF;
        // Vertices
        uint16_t candidate_ll = (candidate_x1 << 8) | candidate_y1;
        uint16_t candidate_lr = (candidate_x2 << 8) | candidate_y1;
        uint16_t candidate_ul = (candidate_x1 << 8) | candidate_y2;
        uint16_t candidate_ur = (candidate_x2 << 8) | candidate_y2;
        // Square
        int candidate_s = (candidate_x2 - candidate_x1) * (candidate_y2 - candidate_y1);
        // Counter
        uint16_t candidate_c = 1;
        // Score
        uint32_t candidate_sc = candidate_s | (candidate_c << 16);
        // Zero-point solution
        candidate_solution.insert(candidate_sc);
        // Push candidates rects to vectors
        if (candidate_ll == ll) candidates_ll.push_back(candidate);
        if (candidate_ll == lr) candidates_lr.push_back(candidate);
        if (candidate_ll == ul) candidates_ul.push_back(candidate);
        if (candidate_ll == ur) candidates_ur.push_back(candidate);
    }

    // Flags for kids presenñe
    bool ll_f, lr_f, ul_f, ur_f;
    ll_f = candidates_ll.size() == 0;
    lr_f = candidates_lr.size() == 0;
    ul_f = candidates_ul.size() == 0;
    ur_f = candidates_ur.size() == 0;

    // Exit without candidates
    if (ll_f && lr_f && ul_f && ur_f) {
        // Remove rect from list if no children
        for (auto it = rects.begin(); it != rects.end(); ++it) {
            if (* it == rect) {
                rects.erase(it);
                --it;
            }
        }
        return candidate_solution;
    }

    // Set for saving all solutions
    set <uint32_t> scores;

    // temporary sets for calculations
    set <uint32_t> tmp_xx, tmp0, tmp_ll, tmp_lr, tmp_ul, tmp_ur;

    // If only one vertex lr with children
    if (ll_f && !lr_f && ul_f && ur_f) {
        tmp_xx.clear();
        // Iterate through these children
        for (auto & lr : candidates_lr) {
            tmp_lr = walkthrough(data, prev, lr, rects);
            for (auto & x1 : prev) {
                for (auto & x2 : tmp_lr) {
                    tmp_xx.insert(x1 + x2);
                }
            }
        }
        return tmp_xx;
    }

    // If only one vertex ul with children
    if (ll_f && lr_f && !ul_f && ur_f) {
        tmp_xx.clear();
        // Iterate through these children
        for (auto & ul : candidates_ul) {
            tmp_ul = walkthrough(data, prev, ul, rects);
            for (auto & x1 : prev) {
                for (auto & x2 : tmp_ul) {
                    tmp_xx.insert(x1 + x2);
                }
            }
        }
        return tmp_xx;
    }

    // If only one vertex ur with children
    if (ll_f && lr_f && ul_f && !ur_f) {
        tmp_xx.clear();
        // Iterate through these children
        for (auto & ur : candidates_ur) {
            tmp_ur = walkthrough(data, prev, ur, rects);
            for (auto & x1 : prev) {
                for (auto & x2 : tmp_ul) {
                    tmp_xx.insert(x1 + x2);
                }
            }
        }
        return tmp_xx;
    }

    // If two vertices with children lr ul 
    if (ll_f && !lr_f && !ul_f && ur_f) {
        tmp_xx.clear();
        for (auto & lr : candidates_lr) tmp_lr = walkthrough(data, prev, lr, rects);
        for (auto & ul : candidates_ul) tmp_ul = walkthrough(data, prev, ul, rects);
        for (auto & x1 : tmp_lr) {
            for (auto & x2 : tmp_ul) {
                tmp_xx.insert(score);
                tmp_xx.insert(x1 + score);
                tmp_xx.insert(x2 + score);
                tmp_xx.insert(x1 + x2 + score);
            }
        }
        return tmp_xx;
    }

    // If two vertices with children lr ur 
    if (ll_f && !lr_f && ul_f && !ur_f) {
        tmp_xx.clear();
        for (auto & lr : candidates_lr) tmp_lr = walkthrough(data, prev, lr, rects);
        for (auto & ur : candidates_ur) tmp_ur = walkthrough(data, prev, ur, rects);
        for (auto & x1 : tmp_lr) {
            for (auto & x2 : tmp_ur) {
                tmp_xx.insert(score);
                tmp_xx.insert(x1 + score);
                tmp_xx.insert(x2 + score);
                tmp_xx.insert(x1 + x2 + score);
            }
        }
        return tmp_xx;
    }

    // If two vertices with children ul ur
    if (ll_f && lr_f && !ul_f && !ur_f) {
        tmp_xx.clear();
        for (auto & ul : candidates_ul) tmp_ul = walkthrough(data, prev, ul, rects);
        for (auto & ur : candidates_ur) tmp_ur = walkthrough(data, prev, ur, rects);
        for (auto & x1 : tmp_ul) {
            for (auto & x2 : tmp_ur) {
                tmp_xx.insert(score);
                tmp_xx.insert(x1 + score);
                tmp_xx.insert(x2 + score);
                tmp_xx.insert(x1 + x2 + score);
            }
        }
        return tmp_xx;
    }

    // If three vertices with children
    if (ll_f && !lr_f && !ul_f && !ur_f) {
        tmp_xx.clear();
        for (auto & lr : candidates_lr) tmp_lr = walkthrough(data, prev, lr, rects);
        for (auto & ul : candidates_ul) tmp_ul = walkthrough(data, prev, ul, rects);
        for (auto & ur : candidates_ur) tmp_ur = walkthrough(data, prev, ur, rects);
        for (auto & x1 : tmp_lr) {
            for (auto & x2 : tmp_ul) {
                for (auto & x3 : tmp_ur) {
                    tmp_xx.insert(score);
                    tmp_xx.insert(x1 + score);
                    tmp_xx.insert(x2 + score);
                    tmp_xx.insert(x3 + score);
                    tmp_xx.insert(x1 + x2 + score);
                    tmp_xx.insert(x2 + x3 + score);
                    tmp_xx.insert(x3 + x1 + score);
                    tmp_xx.insert(x1 + x2 + x3 + score);
                }
            }
        }
        return tmp_xx;
    }
    // couldn't be here
    return tmp_xx;
} 

int main() {
    // Working data
    // TODO: fix processing of rects with the same ll corners
    //
    uint8_t t = 1, n = 30, m = 30, p = n * m;
    p += 225;
    uint32_t data = pack_4(t, n, m, p);

    // Debug print working data
    //cout << "t=" << int(t) << ", n=" << int(n) << ", m=" << int(m) << ", p=" << int(p) << "." << endl;

    // Rects data
    vector <uint32_t> rects;

    // Data filling
    for(int i = 0; i < n; i += 3) {
        for(int j = 0; j < m; j += 3) {
            rects.push_back(pack_4(i, j, i + 3, j + 3));
            rects.push_back(pack_4(i, j, i + 2, j + 2));
        }
    }

    // Debug print rects data
    //for (auto & rect : rects) {
    //    print_rect(rect);
    //}

    // set for storing solutions
    set <uint32_t> prev;

    // Iterate through rects
    for (auto & rect : rects) {
        // If rect started at zero
        if ((rect >> 16) == 0) {
            // Unpack info about current rectangle
            // Coords
            array <uint8_t, 4> coords = unpack_4(rect);
            uint8_t x1 = coords[0];
            uint8_t y1 = coords[1];
            uint8_t x2 = coords[2];
            uint8_t y2 = coords[3];
            // Vertices
            uint16_t ll = (x1 << 8) | y1;
            uint16_t lr = (x2 << 8) | y1;
            uint16_t ul = (x1 << 8) | y2;
            uint16_t ur = (x2 << 8) | y2;
            // Square
            uint16_t square = (x2 - x1) * (y2 - y1);
            // Zero-point solution = prev
            uint16_t counter = 1;
            uint32_t score = square | (counter << 16);
            prev.insert(score);
            // Call recurrent function
            prev = walkthrough(data, prev, rect, rects);
            // Remove incorrect solutions
            check_solution(data, prev);
            // Iterate through results
            uint16_t min_count = p;
            for (auto & result : prev) {
                array <uint16_t, 2> scores = unpack_2(result);
                uint16_t square = scores[1];
                uint16_t counter = scores[0];
                if (square == n * m) {
                    min_count = min(min_count, counter);
                }
            }
            int result = min_count == p ? -1 : min_count;
            cout << result << endl;
        }
    }
}
