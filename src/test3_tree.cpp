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
    // add corners
    rects.push_back(fill_rect(0, 0, 0, 0));
    rects.push_back(fill_rect(n, 0, n, 0));
    rects.push_back(fill_rect(0, m, 0, m));
    rects.push_back(fill_rect(n, m, n, m));
    // add all other rectangles
    for (auto c : input) {
        rects.push_back(fill_rect(c[0], c[1], c[2], c[3]));
    }
    return rects;
}

void print_rect(const string & prefix, Rectangle & rect) {
    cout << prefix << rect.x1 << "'" << rect.y1 << "'" << rect.x2 << "'" << rect.y2 << "]" << endl;
}

int main() {
    n = 5;
    m = 5;
    p = 45;
    vector <vector <int>> input;
    input = { {0, 0, 1, 1},
              {0, 1, 1, 2},
              {0, 2, 1, 3},
              {0, 3, 1, 4},
              {0, 4, 1, 5},
              {1, 0, 2, 1},
              {1, 1, 2, 2},
              {1, 2, 2, 3},
              {1, 3, 2, 4},
              {1, 4, 2, 5},
              {2, 0, 3, 1},
              {2, 1, 3, 2},
              {2, 2, 3, 3},
              {2, 3, 3, 4},
              {2, 4, 3, 5},
              {3, 0, 4, 1},
              {3, 1, 4, 2},
              {3, 2, 4, 3},
              {3, 3, 4, 4},
              {3, 4, 4, 5},
              {4, 0, 5, 1},
              {4, 1, 5, 2},
              {4, 2, 5, 3},
              {4, 3, 5, 4},
              {4, 4, 5, 5},
              {0, 0, 2, 1},
              {0, 1, 2, 2},
              {0, 2, 2, 3},
              {0, 3, 2, 4},
              {0, 4, 2, 5},
              {1, 0, 3, 1},
              {1, 1, 3, 2},
              {1, 2, 3, 3},
              {1, 3, 3, 4},
              {1, 4, 3, 5},
              {2, 0, 4, 1},
              {2, 1, 4, 2},
              {2, 2, 4, 3},
              {2, 3, 4, 4},
              {2, 4, 4, 5},
              {3, 0, 5, 1},
              {3, 1, 5, 2},
              {3, 2, 5, 3},
              {3, 3, 5, 4},
              {3, 4, 5, 5} };

    vector <Rectangle> rects;
    rects = init_rects(input);
    fill_neighbours(rects);

    cout << rects.size() << endl;
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
