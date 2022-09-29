#include <iostream>
#include <vector>
#include <set>

using namespace std;

int n;
int m;
int p;

class Rectangle {
    public:
        int x1;
        int y1;
        int x2;
        int y2;
        int ll;
        int ul;
        int lr;
        int ur;
        int square;
        vector <Rectangle *> ll_neighbours;
        vector <Rectangle *> ul_neighbours;
        vector <Rectangle *> lr_neighbours;
        vector <Rectangle *> ur_neighbours;
        vector <vector <Rectangle *>> solutions;
};

Rectangle fill_rect(int x1, int y1, int x2, int y2) {
    Rectangle rect;
    rect.x1 = x1;
    rect.y1 = y1;
    rect.x2 = x2;
    rect.y2 = y2;
    rect.ll = x1 * 32 + y1;
    rect.ul = x1 * 32 + y2;
    rect.lr = x2 * 32 + y1;
    rect.ur = x2 * 32 + y2;
    rect.square = (x2 - x1) * (y2 - y1);
    return rect;
}

void fill_neighbours(vector <Rectangle> & rects) {
    for (auto & rect : rects) {
        for (auto & cand : rects) {
            // will check candidate
            if (rect.ll == cand.ul || rect.ll == cand.lr) {
                rect.ll_neighbours.push_back(& cand);
            }
            if (rect.ul == cand.ll || rect.ul == cand.ur) {
                rect.ul_neighbours.push_back(& cand);
            }
            if (rect.lr == cand.ll || rect.lr == cand.ur) {
                rect.lr_neighbours.push_back(& cand);
            }
            if (rect.ur == cand.ul || rect.ur == cand.lr) {
                rect.ur_neighbours.push_back(& cand);
            }
        }
    }
}

vector <Rectangle> init_rects(vector <vector <int>> & input) {
    vector <Rectangle> rects;
    // add corners (think out it)
    rects.push_back(fill_rect(0, 0, 0, 0));
    rects.push_back(fill_rect(n, 0, n, 0));
    rects.push_back(fill_rect(0, m, 0, m));
    rects.push_back(fill_rect(n, m, n, m));
    // four fake rects added
    p += 4;
    // add all other rectangles
    for (auto c : input) {
        rects.push_back(fill_rect(c[0], c[1], c[2], c[3]));
    }
    return rects;
}

void print_rect(const string & prefix, Rectangle & rect) {
    cout << prefix << rect.x1 << "'" << rect.y1 << "'" << rect.x2 << "'" << rect.y2 << "]" << endl;
}

void check_4d_tree(vector <Rectangle> & rects) {
    bool bad_rect;
    do {
        bad_rect = false;
        for (auto it = rects.begin(); it != rects.end(); ++it) {
            auto rect = * it;
            if (rect.ll_neighbours.size() == 0 ||
                rect.ul_neighbours.size() == 0 ||
                rect.lr_neighbours.size() == 0 ||
                rect.ur_neighbours.size() == 0) {
                //cout << "bad rect found" << endl;
                bad_rect = true;
            }
            // remove rect from neighbours and itself
            if (bad_rect) {
                for (auto ll = rect.ll_neighbours.begin(); ll != rect.ll_neighbours.end(); ++ll) {
                    rect.ll_neighbours.erase(ll);
                    --ll;
                }
                for (auto ul = rect.ul_neighbours.begin(); ul != rect.ul_neighbours.end(); ++ul) {
                    rect.ul_neighbours.erase(ul);
                    --ul;
                }
                for (auto lr = rect.lr_neighbours.begin(); lr != rect.lr_neighbours.end(); ++lr) {
                    rect.lr_neighbours.erase(lr);
                    --lr;
                }
                for (auto ur = rect.ur_neighbours.begin(); ur != rect.ur_neighbours.end(); ++ur) {
                    rect.ur_neighbours.erase(ur);
                    --ur;
                }
                rects.erase(it);
                --it;
            }
        }
    } while (bad_rect);
}

int main() {
    n = 4;
    m = 4;
    //p = 5;
    vector <vector <int>> input;
    input = { {0, 0, 2, 2},
              {0, 2, 2, 4},
              {2, 0, 4, 2},
              {2, 2, 4, 4},
              {0, 0, 4, 3} };
    //cout << input.size() << endl;
    p = input.size();

    // create rects vector
    vector <Rectangle> rects;
    rects = init_rects(input);
    // fill info about neighbours
    fill_neighbours(rects);
    // check if some rects have hang up vertices
    // it means that these rects can be fully removed
    check_4d_tree(rects);
    // if all rects were deleted during the checking, return -1
    if (rects.size() == 4) {
        cout << -1 << endl;
        return -1;
    }

    //int min_count = p + 1;
    for (auto rect : rects) {
        print_rect("[", rect);
        for (auto & n : rect.ll_neighbours) print_rect("  ll [", * n);
        for (auto & n : rect.ul_neighbours) print_rect("  ul [", * n);
        for (auto & n : rect.lr_neighbours) print_rect("  lr [", * n);
        for (auto & n : rect.ur_neighbours) print_rect("  ur [", * n);
        for (auto & solution : rect.solutions) {
            for (auto & r : solution) print_rect("    sol [", * r);
        }
    }
    //int result = (min_count == p + 1) ? -1 : min_count;
    //cout << result << endl;
}
