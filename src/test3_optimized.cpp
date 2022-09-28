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
        // rect score = square
        vector <int> score;
        // info about children
        vector <Solutions> children;
        //
        bool processed;
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
            uint8_t x1 = x >> 24;
            uint8_t y1 = x >> 16 & 255;
            uint8_t x2 = x >> 8 & 255;
            uint8_t y2 = x & 255;
            // corners
            curr.c_ll = (x1 << 8) + y1;
            curr.c_lr = (x2 << 8) + y1;
            curr.c_ul = (x1 << 8) + y2;
            curr.c_ur = (x2 << 8) + y2;
            // score = rect square
            curr.score.push_back((x2 - x1) * (y2 - y1));
            // processed flag
            curr.processed = false;
        vsols.push_back(curr);
    }
    return vsols;
}

void merge_solutions(Solutions & curr, Solutions & next) {
    //vector <int> temp_score;
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
    //next.score = temp_score;
}

void recurse(Solutions & current, vector <Solutions> & vsols) {
    if (current.processed) return;
cout << endl << "vsols size and curr children size (" << vsols.size() << "'" << current.children.size() << ")" << endl;
cout << "rect [" << int(current.rect>>24) << "'" << int(current.rect>>16&255) << "'" << int(current.rect>>8&255) << "'" << int(current.rect&255) << "]" << endl;
cout << "curr square = score      <"; for (int i = 0; i < current.score.size(); ++i) cout << "<" << current.score[i] << ">"; cout << ">" << endl;
    for (auto next = current.children.begin(); next != current.children.end(); ++next) {
cout << "    next before merge    {"; for (int i = 0; i < (*next).score.size(); ++i) cout << "<" << (*next).score[i] << ">"; cout << "}" << endl;
        merge_solutions(current, *next);
cout << "        next after merge ("; for (int i = 0; i < (*next).score.size(); ++i) cout << "<" << (*next).score[i] << ">"; cout << ")" << endl;
        recurse(*next, vsols);
    }
    current.processed = true;
}

int walkthrough(int & n, int & m, vector <uint32_t> & rects) {
    vector <Solutions> vsols = collect_info(n, m, rects);
    for (auto & curr : vsols) {
        curr.children.clear();
        for (auto & next : vsols) {
            if ((curr.c_ul == next.c_ll) || (curr.c_lr == next.c_ll))
                curr.children.push_back(next);
        }
    }
    int counter = 5;
    do {
        //for (auto & curr : vsols) {
        //    curr.children.clear();
        //    for (auto & next : vsols) {
        //        if ((curr.c_ul == next.c_ll) || (curr.c_lr == next.c_ll))
        //            curr.children.push_back(next);
        //    }
        //}
        for (auto curr = vsols.begin(); curr != vsols.end(); ++curr) {
            if ((*curr).processed) continue;
            for (auto next = (*curr).children.begin(); next != (*curr).children.end(); ++next) {
                merge_solutions(*curr, *next);
                recurse(*next, vsols);
            }
            (*curr).processed = true;
            //if (vsols.size() > 1) {
            //    vsols.erase(curr);
            //    --curr;
            //}
        }
    } while (counter --> 0);
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
}

void pack_rect(int x1, int y1, int x2, int y2, vector <uint32_t> & rects) {
    // pack 4 small int coords to one int and push it to rects
    rects.push_back((x1 << 24) + (y1 << 16) + (x2 << 8) + y2);
}

int main() {
    int n = 5, m = 5;
    vector <uint32_t> rects;
    for (int i = 0; i < n; i += 1){
        for (int j = 0; j < m; j += 1){
            pack_rect(i, j, i + 1, j + 1, rects);
        }
    }

/*
pack_rect(0,0,3,2,rects);
pack_rect(3,0,5,3,rects);
pack_rect(0,2,3,5,rects);
pack_rect(3,3,5,5,rects);
*/

    cout << walkthrough(n, m, rects) << endl;
}
