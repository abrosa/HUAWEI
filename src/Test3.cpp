#include <iostream>
#include <vector>
#include <array>
#include <set>

using namespace std;

class Vertex {
    public:
        int x0;
        int y0;
};

bool operator==(const Vertex& v1, const Vertex& v2) {
    return v1.x0 == v2.x0 && v1.y0 == v2.y0;
};

bool operator!=(const Vertex& v1, const Vertex& v2) {
    return v1.x0 != v2.x0 || v1.y0 != v2.y0;
};

class Score {
    public:
        int s;
        int c;
};

Score operator+(const Score& sc1, const Score& sc2) {
    return Score({sc1.s + sc2.s, sc1.c + sc2.c});
};

class Solution: public Vertex {
    public:
        Vertex xy;
        vector <Score> sc;
};

class Xrectangle {
    public:
        Vertex ll;
        Vertex lr;
        Vertex ul;
        Vertex ur;
        int s;
};

class Rectangle {
    public:
        int x1;
        int y1;
        int x2;
        int y2;
};

void push_xrect(int n, int m, int p, auto &xrect, auto &xrects, auto &solutions) {

    // data from children c for candidate V for Vendetta
    vector <Score> ll_c, lr_c, ul_c, ur_c;
    bool ll_f = false, lr_f = false, ul_f = false, ur_f = false; 

    for (auto old_sol : solutions) {
        if        (old_sol.xy == xrect.ll) {
            ll_f = true;
            ll_c = old_sol.sc;
        } else if (old_sol.xy == xrect.lr) {
            lr_f = true;
            lr_c = old_sol.sc;
        } else if (old_sol.xy == xrect.ul) {
            ul_f = true;
            ul_c = old_sol.sc;
        } else if (old_sol.xy == xrect.ur) {
            ur_f = true;
            ur_c = old_sol.sc;
        }
    }

    Solution new_sol;

    new_sol.xy = xrect.ll;
    Score curr_rect = {xrect.s, 1};
    new_sol.sc.push_back(curr_rect);

    if (lr_f) for (auto i : lr_c) new_sol.sc.push_back(i + curr_rect);
    if (ul_f) for (auto j : ul_c) new_sol.sc.push_back(j + curr_rect);
    if (ur_f) for (auto k : ur_c) new_sol.sc.push_back(k + curr_rect);

    if (lr_f && ul_f) for (auto i : lr_c) for (auto j : ul_c) new_sol.sc.push_back(i + j + curr_rect);
    if (ul_f && ur_f) for (auto j : ul_c) for (auto k : ur_c) new_sol.sc.push_back(j + k + curr_rect);
    if (ur_f && lr_f) for (auto k : ur_c) for (auto i : lr_c) new_sol.sc.push_back(k + i + curr_rect);

    if (lr_f && ul_f && ur_f) for (auto i : lr_c) for (auto j : ul_c) for (auto k : ur_c) new_sol.sc.push_back(i + j + k + curr_rect);

    // add new solution
    if (!ll_f) {
        solutions.push_back(new_sol);
    }

    // if ll exists, add new candidates to it
    for (auto &solution : solutions) {
        if (solution.xy == xrect.ll) {
            solution.sc.insert(solution.sc.end(), new_sol.sc.begin(), new_sol.sc.end());
        }
    }

    set <int> good_squares;
    vector <Score> good_cand;
    vector <Score> unpacked;
    int curr_square;
    int curr_counter;
    int x0, y0;

    // remove bad candidates from solutions
    for (auto &solution : solutions) {
        unpacked.clear();
        for (Score x : solution.sc) {
             unpacked.push_back(x);
        }
        // set for storing sqares of rectangles
        good_squares.clear();
        for (Score x : unpacked) {
            good_squares.insert(x.s);
        }
        good_cand.clear();
        for (int x : good_squares) {
            curr_counter = 100500;
            for (Score y : unpacked) {
                if (x == y.s) {
                    curr_counter = min(curr_counter, y.c);
                }
            }
            x0 = solution.xy.x0;
            y0 = solution.xy.y0;
            if (x <= (n - x0) * (m - y0) && curr_counter <= (n - x0) * (m - y0)) {
                good_cand.push_back({x, curr_counter});
            }
        }
        solution.sc = good_cand;
    }
}

int build_map(int n, int m, int p, auto &rects) {
    /*
    Pre-calculation of rects data:
    original data
    rect = {x1, y1, x2, y2}
    extended to
    xrect = {lower_left, lower_right, upper_left, upper_right, square}
    lower_left  = x1, y1
    lower_right = x2, y1
    upper_left  = x1, y2
    upper_right = x2, y2
    square      = (x2 - x1) * (y2 - y1)
    */
    vector <Xrectangle> xrects;
    int x1, y1, x2, y2;
    for (auto rect : rects) {
        x1 = rect[0]; y1 = rect[1]; x2 = rect[2]; y2 = rect[3];
        xrects.push_back({{x1, y1}, {x2, y1}, {x1, y2}, {x2, y2}, (x2 - x1) * (y2 - y1)});
    }

    // vector for storing all solutions
    vector <Solution> solutions;

    // set for collecting base vertices
    vector <Vertex> xbases;

    // main loop
    while (xrects.size() != 0) {
        // refresh set
        xbases.clear();
        for (Xrectangle xrect : xrects) {
            xbases.push_back(xrect.ll);
        }

        // flags if children rectangles are present
        bool ll, lr, ul, ur; 

        // will try to move xrects without children from "xrects" to "solutions"
        for (auto it = xrects.begin(); it != xrects.end(); ++it) {
            auto xrect = *it;
            for (auto xbase : xbases) {
                ll = xbase == xrect.ll;
                lr = xbase == xrect.lr;
                ul = xbase == xrect.ul;
                ur = xbase == xrect.ur;
            }
            if (ll && !lr && !ul && !ur) {
                push_xrect(n, m, p, xrect, xrects, solutions);
                xrects.erase(it);
                --it;
            }
        }
    }

    // calculate best solution
    int result = 100500;
    Vertex root = {0, 0};
    for (auto solution : solutions) {
        for (Score candidate : solution.sc) {
            if (solution.xy == root && candidate.s == n * m) {
                result = min(candidate.c, result);
            }
        }
    }
    return result == 100500 ? -1 : result;
}

int main() {
    int T;
    cin >> T;
    if (T < 1 || T > 10) {
        cout << "The number of test cases must be in range 1 <= T <= 10" << endl;
        exit(EXIT_FAILURE);
    }
    int n, m, p;
    int x1, y1, x2, y2;
    vector <int> n_vec, m_vec, p_vec;
    vector <vector <array <int, 4>>> coords;
    coords.resize(T);
    for (int i = 0; i < T; ++i) {
        cin >> n >> m >> p;
        if (n < 1 || n > 30 || m < 1 || m > 30) {
            cout << "Map size must be in range 1 <= n, m <= 30" << endl;
            exit(EXIT_FAILURE);
        }
        if (p < 1 || p > 100) {
            cout << "Number of pieces must be in range 1 <= p <= 100" << endl;
            exit(EXIT_FAILURE);
        }
        n_vec.push_back(n);
        m_vec.push_back(m);
        p_vec.push_back(p);
        coords[i].resize(p);
        for (int j = 0; j < p; ++j) {
            cin >> x1 >> y1 >> x2 >> y2;
            coords[i][j] = {x1, y1, x2, y2};
        }
    }
    cout << endl;
    for (int i = 0; i < T; ++i) {
        cout << build_map(n_vec[i], m_vec[i], p_vec[i], coords[i]) << endl;
    }
    return 0;
}
