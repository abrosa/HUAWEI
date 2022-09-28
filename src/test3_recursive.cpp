#include <iostream>
#include <array>
#include <set>
#include <algorithm>

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

array <uint16_t, 2> unpack_2(uint32_t num1num2) {
    array <uint16_t, 2> data;
    data[0] = (num1num2 >> 16);
    data[1] = num1num2 & 0xFFFF;
    return data;
}

void print_rect(uint32_t rect) {
    array <uint8_t, 4> coords = unpack_4(rect);
    cout << "(" << int(coords[0]) << "'" << int(coords[1]);
    cout << "'" << int(coords[2]) << "'" << int(coords[3]) << ")" << endl;
}

void print_data(uint32_t rect) {
    array <uint8_t, 4> data = unpack_4(rect);
    cout << "[" << int(data[0]) << "'" << int(data[1]);
    cout << "'" << int(data[2]) << "'" << int(data[3]) << "]" << endl;
}

void print_solutions(set <uint32_t> solutions) {
    for (uint32_t solution : solutions) {
        array <uint16_t, 2> score = unpack_2(solution);
        cout << "(" << int(score[0]) << "'" << int(score[1]) << ")" << endl;
    }
}

void check_solutions(uint32_t data, set <uint32_t> & solutions) {
//print_solutions(solutions);
    // Unpack data
    array <uint8_t, 4> coords = unpack_4(data);
    uint8_t t = coords[0];
    uint8_t n = coords[1];
    uint8_t m = coords[2];
    uint8_t p = coords[3];
    for (auto it = solutions.begin(); it != solutions.end(); ++it) {
        auto solution = * it;
        array <uint16_t, 2> scores = unpack_2(solution);
        uint16_t square = scores[1];
        uint16_t counter = scores[0];
        if (square > n * m || counter > p) {
            solutions.erase(it);
            --it;
        }
    }
}

// Recurrent function
// data is packed t, n, m, p
// solutions is a set for solutions
// every solution is packed to one unsigned int square and counter
// rects is a set with all not-processed rects
// every rect is packed to one unsigned int x1, y1, x2, y2
void walkthrough(uint32_t data, set <uint32_t> & solutions, uint32_t & root, set <uint32_t> & rects) {
    // Check solutions for incorrect data and remove them
    check_solutions(data, solutions);
    // Unpack info about current root rect
    // Coords
    array <uint8_t, 4> root_coords = unpack_4(root);
    uint8_t root_x1 = root_coords[0];
    uint8_t root_y1 = root_coords[1];
    uint8_t root_x2 = root_coords[2];
    uint8_t root_y2 = root_coords[3];
    // Vertices
    uint16_t root_lr = (root_x2 << 8) | root_y1;
    uint16_t root_ul = (root_x1 << 8) | root_y2;
    uint16_t root_ur = (root_x2 << 8) | root_y2;
    // Square and score
    uint16_t root_square = (root_x2 - root_x1) * (root_y2 - root_y1);
    uint32_t root_score = root_square | (1 << 16);
    // Add to solutions current root rect
    solutions.insert(root_score);

    // Search of children rectangles
    set <uint32_t> cand_solution;
    // Coords
    uint16_t cand_ll, cand_lr, cand_ul, cand_ur;
    // Vectors for cands processing
    set <uint32_t> cands_ll, cands_lr, cands_ul, cands_ur;
    // Iterate through the cands
    for (auto & cand : rects) {
            // Will not process the same rects
            //if (cand == root) continue;
            // Unpack info about cand
            // Coords
            // uint32_t cand = x1_y1_x2_y2
            uint8_t cand_x1 = (cand >> 24);
            uint8_t cand_y1 = (cand >> 16) & 0xFF;
            uint8_t cand_x2 = (cand >>  8) & 0xFF;
            uint8_t cand_y2 = cand & 0xFF;
            // Vertices
            uint16_t cand_ll = (cand_x1 << 8) | cand_y1;
            uint16_t cand_lr = (cand_x2 << 8) | cand_y1;
            uint16_t cand_ul = (cand_x1 << 8) | cand_y2;
            uint16_t cand_ur = (cand_x2 << 8) | cand_y2;
            // Square
            int cand_s = (cand_x2 - cand_x1) * (cand_y2 - cand_y1);
            // Counter
            uint16_t cand_c = 1;
            // Score
            uint32_t cand_sc = cand_s | (cand_c << 16);
            // Zero-point solution
            cand_solution.insert(cand_sc);
            // Push cands rects to sets for processing
            if (cand_ll == root_lr) cands_lr.insert(cand);
            if (cand_ll == root_ul) cands_ul.insert(cand);
            if (cand_ll == root_ur) cands_ur.insert(cand);
        }
        
        // Flags for kids presented
        bool ll_f, lr_f, ul_f, ur_f;
        //ll_f = cands_ll.size() != 0;
        lr_f = cands_lr.size() != 0;
        ul_f = cands_ul.size() != 0;
        ur_f = cands_ur.size() != 0;
        
        // Exit without cands
        if (!lr_f && !ul_f && !ur_f) {
            // Remove root rect from list if no children
            for (auto it = rects.begin(); it != rects.end(); ++it) {
                if (* it == root) {
                    rects.erase(it);
                    --it;
                }
            }
            return;
        }
        
        // Set for saving all solutions
        set <uint32_t> scores;
        
        // If only one vertex lr with children
        if (lr_f && !ul_f && !ur_f) {
            walkthrough(data, solutions, cands_lr, rects);
            check_solutions(data, solutions);
            for (auto it = solutions.begin(); it != solutions.end(); ++it) {
                auto solution = * it;
                solution += root_score;
            }
            return;
        }
        
        // If only one vertex ul with children
        if (!lr_f && ul_f && !ur_f) {
            walkthrough(data, solutions, cands_ul, rects);
            check_solutions(data, solutions);
            for (auto it = solutions.begin(); it != solutions.end(); ++it) {
                auto solution = * it;
                solution += root_score;
            }
            return;
        }
        
        // If only one vertex ur with children
        if (!lr_f && !ul_f && ur_f) {
            walkthrough(data, solutions, cands_ur, rects);
            check_solutions(data, solutions);
            for (auto it = solutions.begin(); it != solutions.end(); ++it) {
                auto solution = * it;
                solution += root_score;
            }
            return;
        }
        
        // If two vertices with children lr ul 
        if (lr_f && ul_f && !ur_f) {
            for (auto & x : cands_lr) {
                for (auto & y : cands_ul) {
                    walkthrough(data, solutions, cands_lr, rects);
                    check_solutions(data, solutions);
                    walkthrough(data, solutions, cands_ul, rects);
                    check_solutions(data, solutions);
                }
            }
            for (auto it = solutions.begin(); it != solutions.end(); ++it) {
                auto solution = * it;
                solution += root_score;
            }
            return;
        }
        
        // If two vertices with children lr ur 
        if (lr_f && !ul_f && ur_f) {
            for (auto & x : cands_lr) {
                for (auto & y : cands_ur) {
                    walkthrough(data, solutions, cands_lr, rects);
                    check_solutions(data, solutions);
                    walkthrough(data, solutions, cands_ur, rects);
                    check_solutions(data, solutions);
                }
            }
            for (auto it = solutions.begin(); it != solutions.end(); ++it) {
                auto solution = * it;
                solution += root_score;
            }
            return;
        }
        
        // If two vertices with children ul ur
        if (!lr_f && ul_f && ur_f) {
            for (auto & x : cands_ul) {
                for (auto & y : cands_ur) {
                    walkthrough(data, solutions, cands_ul, rects);
                    check_solutions(data, solutions);
                    walkthrough(data, solutions, cands_ur, rects);
                    check_solutions(data, solutions);
                }
            }
            for (auto it = solutions.begin(); it != solutions.end(); ++it) {
                auto solution = * it;
                solution += root_score;
            }
            return;
        }
        
        // If three vertices with children
        if (lr_f && ul_f && ur_f) {
            for (auto & x : cands_lr) {
                for (auto & y : cands_ul) {
                    for (auto & z : cands_ur) {
                    walkthrough(data, solutions, cands_lr, rects);
                    check_solutions(data, solutions);
                    walkthrough(data, solutions, cands_ul, rects);
                    check_solutions(data, solutions);
                    walkthrough(data, solutions, cands_ur, rects);
                    check_solutions(data, solutions);
                    }
                }
            }
            for (auto it = solutions.begin(); it != solutions.end(); ++it) {
                auto solution = * it;
                solution += root_score;
            }
            return;
        }
        cout << "???" << endl;
    }
    cout << "!!!" << endl;
    // couldn't be here
    //return;
} 

int main() {
    // Working data
    uint8_t t = 1, n = 2, m = 2, p = n * m;
    //p = 100;
    uint32_t data = pack_4(t, n, m, p);
//    print_data(data);

    // Rects data
    set <uint32_t> rects;

    // Data filling
    for(int i = 0; i < n; i += 1) {
        for(int j = 0; j < m; j += 1) {
            rects.insert(pack_4(i, j, i + 1, j + 1));
        }
    }

    // Debug print rects data
    for (auto & rect : rects) {
//        print_rect(rect);
    }

    set <uint32_t> roots;
    // Collect info about root rects
    for (auto & rect : rects) {
        // If root started at zero
        if ((rect >> 16) == 0) {
            // Save it
            roots.insert(rect);
        }
    } 

    // set for storing solutions
    set <uint32_t> solutions;
    for (auto & root : roots) {
        // Call recurrent function
        walkthrough(data, solutions, root, rects);
        check_solutions(data, solutions);
    }

    // Print solutions
    print_solutions(solutions);
    // Iterate through solutions to find the best
    uint16_t min_count = p;
    for (auto & solution : solutions) {
        array <uint16_t, 2> scores = unpack_2(solution);
        if (scores[1] == n * m) {
            min_count = min(min_count, scores[0]);
        }
    }
    // Print result or -1
    int result = min_count == p ? -1 : min_count;
    cout << endl << result << endl;
}
