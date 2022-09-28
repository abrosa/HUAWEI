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
        // coordinates
        uint8_t x1;
        uint8_t y1;
        uint8_t x2;
        uint8_t y2;
        // rect with packed coordinates
        uint32_t rect;
        // info about corners
        uint16_t c_ll;
        uint16_t c_lr;
        uint16_t c_ul;
        uint16_t c_ur;
        // rect square and counter
        uint16_t square;
        uint16_t counter;
        vector <int> score;
        // flag for processed rects
        bool processed = false;
        bool is_ll = false;
        bool is_lr = false;
        bool is_ul = false;
        bool is_ur = false;
        // info about neighbours
        vector <Solutions> neighbours;
};

void merge_solutions(Solutions & curr, Solutions & next) {
    // merge curr and next score
    vector <int> score1, score2;
    score1 = curr.score;
    score2 = next.score;
    while (score1.size > score2.size) score2.push_back(score2.size() + 1);
    while (score2.size > score1.size) score1.push_back(score1.size() + 1);
    // multiply solution matrices
    for (auto & score1 : curr.score) {
        uint16_t square1 = score1 & 65535;
        uint16_t counter1 = score1 >> 16;
        for (auto & score2 : next.score) {
            uint16_t square2 = score2 & 65535;
            uint16_t counter2 = score2 >> 16;
            if (square1 + square2 <= curr.n * curr.m && counter1 + counter2 <= curr.p) {
                //cout << "!";
                tmp.insert(score1 + score2);
            }
        }
    }
    curr.score = score;
    next.score = score;
}

vector <Solutions> collect_info(int n, int m, vector <uint32_t> & rects) {
    // collect info for Solutions class
    vector <Solutions> vsols;
    for (auto & x : rects) {
        Solutions curr;
        curr.n = n;
        curr.m = m;
        curr.p = rects.size();
        curr.rect = x;
        curr.x1 = x >> 24;
        curr.y1 = x >> 16 & 255;
        curr.x2 = x >> 8 & 255;
        curr.y2 = x & 255;
        curr.c_ll = (curr.x1 << 8) + curr.y1;
        curr.c_lr = (curr.x2 << 8) + curr.y1;
        curr.c_ul = (curr.x1 << 8) + curr.y2;
        curr.c_ur = (curr.x2 << 8) + curr.y2;
        curr.is_ll = curr.x1 == 0 && curr.y1 == 0;
        curr.is_lr = curr.x2 == n && curr.y1 == 0;
        curr.is_ul = curr.x1 == 0 && curr.y2 == m;
        curr.is_ur = curr.x2 == n && curr.y2 == m;
        curr.square = (curr.x2 - curr.x1) * (curr.y2 - curr.y1);
        curr.counter = 1;
        uint32_t score = curr.square + (1<<16);
        curr.score.insert({score});
        vsols.push_back(curr);
    }
    return vsols;
}

void update_info(vector <Solutions> & vsols) {
    // update info about neighbours
    for (auto & curr : vsols) {
        for (auto & next : vsols) {
            if ((curr.c_ul == next.c_ur) ||
                (curr.c_lr == next.c_ur) ||
                (curr.c_ur == next.c_ur) ||
                (curr.c_ur == next.c_ul) ||
                (curr.c_ll == next.c_ul) ||
                (curr.c_ul == next.c_ul) ||
                (curr.c_ur == next.c_lr) ||
                (curr.c_ll == next.c_lr) ||
                (curr.c_lr == next.c_lr) ||
                (curr.c_ul == next.c_ll) ||
                (curr.c_lr == next.c_ll) ||
                (curr.c_ll == next.c_ll))
                curr.neighbours.push_back(next);
        }
    }
}

void visualize(vector <Solutions> & vsols) {
    string tmp;
    //system("ping 127.0.0.1 -n 1 > nul");
    uint32_t rect;
    for (int y = vsols[0].m - 1; y >= 0; --y) {
        for (int x = 0; x < vsols[0].n; ++x) {
            rect = x * 256 * 65537 + y * 65537 + 257;
            for (auto & curr : vsols) {
                if (curr.rect == rect) {
                    if (curr.processed) {
                        tmp += "X";
                    } else {
                        tmp += "-";
                    }
                    tmp += " ";
                }
            }
        }
        tmp += "\n";
    }
    tmp += "\n";
    system("cls");
    cout << tmp;
}

int walkthrough(int & n, int & m, vector <uint32_t> & rects) {
    // collect info from the rects
    vector <Solutions> vsols = collect_info(n, m, rects);
    update_info(vsols);
    // main loop
    int counter = 10;
    int processed;
    do {
        processed = 0;
        for (auto curr = vsols.begin(); curr != vsols.end(); ++curr) {
            for (auto next = (*curr).neighbours.begin(); next != (*curr).neighbours.end(); ++next) {
                merge_solutions(*curr, *next); ++processed;
            }

            //visualize(vsols);
            cout << vsols.size() << endl;
            for (auto & vsol : vsols) {
                for (auto & score : vsols[0].score) {
                    auto square = score & 65535;
                    auto cou    = score >> 16;
                    cout << square << "'" << cou << " ";
                }
                cout << ";";
            }
            cout << endl;

            if (vsols.size() > 1) {
                vsols.erase(curr);
                --curr;
            }
        }
    } while (counter --> 0);
    //} while (processed != 0);
    //cout << vsols.size() << endl;
    // Iterate through solutions to find the best
    int p = rects.size();
    int min_count = p + 1;
    for (auto & solution : vsols) {
        if (solution.processed) continue;
        for (auto & curr_solution : solution.score) {
            int square = curr_solution & 65535;
            int counter = curr_solution >> 16;
            if (square == n * m) {
                min_count = min(min_count, counter);
            }
        }
    }
    return min_count == p + 1 ? -1 : min_count;
}

void pack_rect(int x1, int y1, int x2, int y2, vector <uint32_t> & rects) {
    // pack 4 small int coords to one int and push it to rects
    rects.push_back((x1 << 24) + (y1 << 16) + (x2 << 8) + y2);
}

int main() {
    // Map size
    int n = 5, m = 5;
    // Fill rects
    vector <uint32_t> rects;
/*
    for (int i = 0; i < n; i += 1){
        for (int j = 0; j < m; j += 1){
            pack_rect(i, j, i + 1, j + 1, rects);
        }
    }
*/
/*
    for (int i = 0; i < n; i += 3){
        for (int j = 0; j < m; j += 3){
            pack_rect(i, j, i + 3, j + 3, rects);
        }
    }
*/
pack_rect(0,0,1,1,rects);
pack_rect(0,1,1,2,rects);
pack_rect(0,2,1,3,rects);
pack_rect(0,3,1,4,rects);
pack_rect(0,4,1,5,rects);
pack_rect(1,0,2,1,rects);
pack_rect(1,1,2,2,rects);
pack_rect(1,2,2,3,rects);
pack_rect(1,3,2,4,rects);
pack_rect(1,4,2,5,rects);
pack_rect(2,0,3,1,rects);
pack_rect(2,1,3,2,rects);
pack_rect(2,2,3,3,rects);
pack_rect(2,3,3,4,rects);
pack_rect(2,4,3,5,rects);
pack_rect(3,0,4,1,rects);
pack_rect(3,1,4,2,rects);
pack_rect(3,2,4,3,rects);
pack_rect(3,3,4,4,rects);
pack_rect(3,4,4,5,rects);
pack_rect(4,0,5,1,rects);
pack_rect(4,1,5,2,rects);
pack_rect(4,2,5,3,rects);
pack_rect(4,3,5,4,rects);
pack_rect(4,4,5,5,rects);

pack_rect(0,0,2,1,rects);
pack_rect(0,1,2,2,rects);
pack_rect(0,2,2,3,rects);
pack_rect(0,3,2,4,rects);
pack_rect(0,4,2,5,rects);
pack_rect(1,0,3,1,rects);
pack_rect(1,1,3,2,rects);
pack_rect(1,2,3,3,rects);
pack_rect(1,3,3,4,rects);
pack_rect(1,4,3,5,rects);
pack_rect(2,0,4,1,rects);
pack_rect(2,1,4,2,rects);
pack_rect(2,2,4,3,rects);
pack_rect(2,3,4,4,rects);
pack_rect(2,4,4,5,rects);
pack_rect(3,0,5,1,rects);
pack_rect(3,1,5,2,rects);
pack_rect(3,2,5,3,rects);
pack_rect(3,3,5,4,rects);
pack_rect(3,4,5,5,rects);
    // Print result
    cout << walkthrough(n, m, rects) << endl;
    system("pause");
    return 0;
}


/*
 ||
                    ((*next1).lr == (*curr).ll || (*next1).lr == (*curr).ur) ||
                    ((*next1).ul == (*curr).ll || (*next1).ul == (*curr).ur) ||
                    ((*next1).ur == (*curr).lr || (*next1).ur == (*curr).ul)


*/