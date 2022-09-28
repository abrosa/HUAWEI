#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Solutions {
    public:
        // map size
        int n;
        int m;
        // number of fragments
        int p;
        // rect with packed coordinates
        uint32_t rect;
        // info about corners
        uint16_t c_ll;
        uint16_t c_lr;
        uint16_t c_ul;
        uint16_t c_ur;
        uint8_t x1;
        uint8_t y1;
        uint8_t x2;
        uint8_t y2;
        int square;
        vector <int> score;
        // info about children
        vector <Solutions> children;
        //
        bool processed;
};

class Scores {
    public:
        uint16_t ll;
        uint16_t ul;
        uint16_t lr;
        vector <int> score;
};

vector <Solutions> collect_info(int n, int m, vector <uint32_t> & rects) {
    // collect info for Solutions class
    vector <Solutions> vsols;
    for (auto & x : rects) {
        Solutions curr;
            // initial data
            curr.n = n;
            curr.m = m;
            curr.p = rects.size();
            // packed rect
            curr.rect = x;
            // unpack rect
            curr.x1 = x >> 24;
            curr.y1 = x >> 16 & 255;
            curr.x2 = x >> 8 & 255;
            curr.y2 = x & 255;
            // corners
            curr.c_ll = (curr.x1 << 8) + curr.y1;
            curr.c_lr = (curr.x2 << 8) + curr.y1;
            curr.c_ul = (curr.x1 << 8) + curr.y2;
            curr.c_ur = (curr.x2 << 8) + curr.y2;
            curr.square = (curr.x2 - curr.x1) * (curr.y2 - curr.y1);
            curr.score.push_back(0);
            curr.score.push_back(curr.square);
            // processed flag
            curr.processed = false;
        vsols.push_back(curr);
    }
    return vsols;
}

Solutions merge_solutions(Solutions & curr, Solutions & next) {
    //Solution temp;
    int new_size = curr.score.size() + next.score.size();
    new_size = min(new_size, curr.p);
    next.score.resize(new_size);
    //temp_score.clear();
    //int tmp
    for (int i = 0; i < next.score.size(); ++i) {
    for (int j = i; j < i + curr.score.size(); ++j) {
        if (next.score[i] + curr.score[j-i] <= curr.n * curr.m && j + 2 <= curr.p) {
            next.score[j+1] = max(next.score[j+1], next.score[i] + curr.score[j-i]);
        }
    }
    }
    next.score[0] = max(next.score[0], curr.score[0]);
    return next;
}

void merge_scores(int n, int m, int p, vector <int> & score1, vector <int> & score2) {
//cout << endl;
//cout << endl << "score1 <"; for (auto & x : score1) {cout << x << "'";} cout << ">";
//cout << endl << "score2 <"; for (auto & x : score2) {cout << x << "'";} cout << ">";
    // merge score1 to score2
    if (score1 == score2) return;
    int new_size = score1.size() + score2.size() - 1;
    new_size = min(new_size, p);
    vector <int> result(new_size, 0);
    for (int k = 0; k < new_size; ++k) {
        for (int i = 0; i < score1.size(); ++i) {
            result[i] = max(result[i], score1[i]);
            for (int j = 0; j < score2.size(); ++j) {
                result[j] = max(result[j], score2[j]);
                if (i + j == k && i + j <= p) {
                    if (score1[i] + score2[j] <= n * m) {
                        result[k] = max(result[k], score1[i] + score2[j]);
                    }
                }
            }
        }
    }
//cout << endl << "result <"; for (auto & x : result) {cout << x << "'";} cout << ">";
    score2 = result;
}

/*
void recurse(int n, int m, int p, vector <int> & result, Solutions & current) {
    bool has_ul;
    bool has_lr;
    for (auto & first : vsols) {
        if (first.rect != current.rect) continue;
cout << endl << "first.rect ["; auto x = first.rect; cout << (x>>24) << "'" << (x>>16&255) << "'" << (x>>8&255) << "'" << (x&255) << "]";
        //if (first.processed) continue;
        //recurse(first.score, first, vsols);
        merge_scores(int n, int m, int p, first.score, result);
        //first.processed = true;
        has_ul = false;
        has_lr = false;
        for (auto & second : vsols) {
            if (first.rect == second.rect) continue;
            if (first.c_ul == second.c_ll) {
                has_ul = true;
                break;
            }
            if (first.c_lr == second.c_ll) {
                has_lr = true;
                break;
            }
        }
        // if it has both children (doesn't work)
        if (has_ul && has_lr) {
            for (auto & second : vsols) {
                if (first.rect == second.rect) continue;
                if (first.c_ul == second.c_ll) {
                    for (auto & third : vsols) {
                        if (first.rect == third.rect) continue;
                        if (second.rect == third.rect) continue;
                        if (first.c_lr == third.c_ll) {
                            recurse(second.score, second, vsols);
                            merge_scores(second.score, result, vsols);
                            recurse(third.score, third, vsols);
                            merge_scores(third.score, result, vsols);
                        }
                    }
                }
            }
        }
        // if it has only one child
        if ((has_ul && ! has_lr) || (! has_ul && has_lr)) {
            for (auto & second : vsols) {
                if (first.rect == second.rect) continue;
                if ((first.c_ul == second.c_ll) ||
                    (first.c_lr == second.c_ll)) {
                    // will not recurse if it's ur corner
                    if (second.c_ur == (second.n << 8) + second.m) {
                    //    merge_scores(second.score, result, vsols);
                        continue;
                    }
                    recurse(second.score, second, vsols);
                    merge_scores(second.score, result, vsols);
                }
            }
        }
    }
}
*/

void recursive2(int & n, int & m, int & p, Scores & result, vector <uint32_t> & rects) {
    //for (auto & rect : rects) {
        //uint8_t x1 = rect >> 24;
        //uint8_t y1 = rect >> 16 & 255;
        //uint8_t x2 = rect >> 8 & 255;
        //uint8_t y2 = rect & 255;
        //uint16_t ll = (x1 << 8) + y1;
        //int square = (x2 - x1) * (y2 - y1);
        //Scores score;
        uint16_t ll = result.ll;
        uint16_t ul = result.ul;
        uint16_t lr = result.lr;
        //score.score = {0, square};
        for (auto & child : rects) {
            uint8_t xx1 = child >> 24;
            uint8_t yy1 = child >> 16 & 255;
            uint8_t xx2 = child >> 8 & 255;
            uint8_t yy2 = child & 255;
            uint16_t child_ll = (xx1 << 8) + yy1;
            int child_square = (xx2 - xx1) * (yy2 - yy1);
            Scores child_score;
            child_score.ll = child_ll;
            child_score.score = {0, child_square};
            // corner to attach is ll
            if (child_ll == lr || child_ll == ul) {
                merge_scores(n, m, p, child_score.score, result.score);
            }
        }
    //}
}

int walkthrough(int & n, int & m, int & p, vector <uint32_t> & rects) {
    // full vector with info (deprecated?)
    vector <Solutions> vsols = collect_info(n, m, rects);
    // collect info about vertices
    set <uint16_t> vertices;
    for (auto & rect : rects) {
        uint8_t x1 = rect >> 24;
        uint8_t y1 = rect >> 16 & 255;
        uint8_t x2 = rect >> 8 & 255;
        uint8_t y2 = rect & 255;
        vertices.insert((x1 << 8) + y1);
        vertices.insert((x2 << 8) + y2);
        vertices.insert((x1 << 8) + y2);
        vertices.insert((x2 << 8) + y1);
    }
    // check input data
    // all corners must be presented
    if (! vertices.contains((0 << 8) + 0)) return -1;
    if (! vertices.contains((n << 8) + 0)) return -1;
    if (! vertices.contains((0 << 8) + m)) return -1;
    if (! vertices.contains((n << 8) + m)) return -1;

    // start from the lower left corner vertex = 0
    uint16_t vertex = 0;
    // iterate through rects
    for (auto & rect : rects) {
        uint8_t x1 = rect >> 24;
        uint8_t y1 = rect >> 16 & 255;
        uint8_t x2 = rect >> 8 & 255;
        uint8_t y2 = rect & 255;
        // base corner ll
        uint16_t ll = (x1 << 8) + y1;
        // two corners for growing lr and ul
        uint16_t lr = (x2 << 8) + y1;
        uint16_t ul = (x1 << 8) + y2;
        int square = (x2 - x1) * (y2 - y1);
        Scores score;
        score.ll = ll;
        score.score = {0, square};
        // good rect for start
        if (vertex == (x1 << 8) + y1) {
            for (auto & child : rects) {
                uint8_t xx1 = child >> 24;
                uint8_t yy1 = child >> 16 & 255;
                uint8_t xx2 = child >> 8 & 255;
                uint8_t yy2 = child & 255;
                uint16_t child_ll = (xx1 << 8) + yy1;
                int child_square = (xx2 - xx1) * (yy2 - yy1);
                Scores child_score;
                child_score.ll = child_ll;
                child_score.score = {0, child_square};
                // corners to attach is ll
                if (child_ll == lr || child_ll == ul) {
                    //cout << "Hello, World!" << endl;
                    //cout << "child_square=" << child_square << endl;
                    //cout << "counter=" << 1 << endl;
cout << "result <"; for (auto & x : score.score) {cout << x << "'";} cout << ">" << endl;
                    merge_scores(n, m, p, child_score.score, rects);
cout << "result <"; for (auto & x : score.score) {cout << x << "'";} cout << ">" << endl;
                }
            }
        }
    }
    /*

    for (auto & curr : vsols) {
        curr.children.clear();
        for (auto & next : vsols) {
            if ((curr.c_ul == next.c_ll) || (curr.c_lr == next.c_ll))
                curr.children.push_back(next);
        }
    }
    vector <int> result;
    int counter = 1;
    while (counter != 0) {
        for (auto & first : vsols) {
            if (first.c_ll != 0) continue;
            result = first.score;
            recurse(first.score, first, vsols);
            merge_scores(first.score, result, vsols);
        }
        for (auto & last : vsols) {
            if (last.c_ur == (last.n << 8) + last.m) {
                cout << endl << "main result <"; for (auto & x : result) {cout << x << "'";} cout << ">";
                cout << endl << "main result <"; for (auto & x : last.score) {cout << x << "'";} cout << ">";
                merge_scores(last.score, result, vsols);
                cout << endl << "main result <"; for (auto & x : result) {cout << x << "'";} cout << ">";
            }
        }
        --counter;
    }

    int min_count = rects.size() + 1;
    for (auto & solution : vsols) {
        for (auto & curr_solution : solution.score) {
            if (curr_solution == n * m) {
                min_count = min(min_count, counter);
            }
        }
    }
    cout << endl;
    return min_count == rects.size() + 1 ? -1 : min_count;
    */
    return -1;
}

void pack_rect(int x1, int y1, int x2, int y2, vector <uint32_t> & rects) {
    // pack 4 small int coords to one int and push it to rects
    rects.push_back((x1 << 24) + (y1 << 16) + (x2 << 8) + y2);
}

int main() {
    int n = 3, m = 5;
    vector <uint32_t> rects;
/*
    for (int i = 0; i < n; i += 1){
        for (int j = 0; j < m; j += 1){
            pack_rect(i, j, i + 1, j + 1, rects);
        }
    }
*/

pack_rect(0,0,1,1,rects);
pack_rect(0,1,1,5,rects);
pack_rect(1,0,3,1,rects);
pack_rect(1,1,3,5,rects);

    int p = rects.size(); 
    cout << walkthrough(n, m, p, rects) << endl;
}
