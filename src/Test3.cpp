#include <iostream>
#include <vector>
#include <array>
#include <set>

// these limits will be used to mapping
// the pair of coordinates x,y to one int
// the pair of square and counter to one int

#define MAX_P 100
#define MAX_X 32
#define MAX_Y 32
#define MAX_XY 1024

using namespace std;

class Solution {
    public:
        array <int, 2> xy;
        vector <array <int, 2>> sc;
};

bool is_valid(int n, int m, int p, int candidate) {
    return ((candidate % MAX_XY) <= n * m) && ((candidate / MAX_XY) <= p);
}

void push_xrect(int n, int m, int p, auto &xrect, auto &same, auto &xrects, auto &solutions) {

    // data from children c for candidate V for Vendetta
    vector <array <int, 2>> ll_c, lr_c, ul_c, ur_c;
    bool ll_f = false, lr_f = false, ul_f = false, ur_f = false; 

    for (auto old_sol : solutions) {
        if        (old_sol.xy[0] == xrect[0] % 32 && old_sol.xy[1] == xrect[0] / 32) {
            ll_f = true;
            ll_c = old_sol.sc;
        } else if (old_sol.xy[0] == xrect[1] % 32 && old_sol.xy[1] == xrect[1] / 32) {
            lr_f = true;
            lr_c = old_sol.sc;
        } else if (old_sol.xy[0] == xrect[2] % 32 && old_sol.xy[1] == xrect[2] / 32) {
            ul_f = true;
            ul_c = old_sol.sc;
        } else if (old_sol.xy[0] == xrect[3] % 32 && old_sol.xy[1] == xrect[3] / 32) {
            ur_f = true;
            ur_c = old_sol.sc;
        }
    }

    Solution new_sol;

    new_sol.xy = {xrect[0] % 32, xrect[0] / 32};
    new_sol.sc.push_back({xrect[4], 1});

    for (auto same_rect : same) {
        new_sol.sc.push_back({same_rect[4], 1});
    }

    if (lr_f) for (auto i : lr_c) new_sol.sc.push_back({i[0] + xrect[4], i[1] + 1});
    if (ul_f) for (auto j : ul_c) new_sol.sc.push_back({j[0] + xrect[4], j[1] + 1});
    if (ur_f) for (auto k : ur_c) new_sol.sc.push_back({k[0] + xrect[4], k[1] + 1});

    if (lr_f && ul_f) for (auto i : lr_c) for (auto j : ul_c) new_sol.sc.push_back({i[0] + j[0] + xrect[4], i[1] + j[1] + 1});
    if (ul_f && ur_f) for (auto j : ul_c) for (auto k : ur_c) new_sol.sc.push_back({j[0] + k[0] + xrect[4], j[1] + k[1] + 1});
    if (ur_f && lr_f) for (auto k : ur_c) for (auto i : lr_c) new_sol.sc.push_back({k[0] + i[0] + xrect[4], k[1] + i[1] + 1});

    if (lr_f && ul_f && ur_f) for (auto i : lr_c) for (auto j : ul_c) for (auto k : ur_c) new_sol.sc.push_back({i[0] + j[0] + k[0] + xrect[4], i[1] + j[1] + k[1] + 1});

    if (!ll_f) {
        solutions.push_back(new_sol);
    }

    // if ll exists, add new candidates to it
    for (auto &solution : solutions) {
        if (solution.xy[0] == xrect[0] % 32 && solution.xy[1] == xrect[0] / 32) {
            solution.sc.insert(solution.sc.end(), new_sol.sc.begin(), new_sol.sc.end());
        }
    }
/*
    // merge similar solutions
    for (auto &solution1 : solutions) {
        for (auto &solution2 : solutions) {
            if ((solution1.xy == solution2.xy) && (solution1.sc != solution2.sc)) {
                solution1.sc.insert(solution1.sc.end(), solution2.sc.begin(), solution2.sc.end());
                solution2.sc.clear();
            }
        }
    }
*/
    set <int> good_squares;
    vector <array <int, 2>> good_cand;
    vector <array <int, 2>> unpacked;
    int curr_square;
    int curr_counter;
    int x0, y0;
    // remove bad candidates from solutions
    for (auto &solution : solutions) {
        unpacked.clear();
        for (auto x : solution.sc) {
             unpacked.push_back({x[0], x[1]});
        }
        good_squares.clear();
        for (auto x : unpacked) {
            good_squares.insert(x[0]);
        }
        good_cand.clear();
        for (auto x : good_squares) {
            curr_counter = 100500;
            for (auto y : unpacked) {
                if (x == y[0]) {
                    curr_counter = min(curr_counter, y[1]);
                }
            }
            x0 = solution.xy[0];
            y0 = solution.xy[1];
            if (x <= (n - x0) * (m - y0) && curr_counter <= (n - x0) * (m - y0)) {
                good_cand.push_back({x, curr_counter});
            }
        }
        solution.sc = good_cand;
    }
/*
// debug print
    for (auto solution : solutions) {
        cout << endl << solution.xy[0] << "." << solution.xy[1] << " ";
        for (auto x : solution.sc) {
             cout << x[1] << "'" << x[0] << " ";
        }
    }
    cout << endl;
// debug print
*/
}

int build_map(int n, int m, int p, auto &rects) {
    /*
    Pre-calculation of rects data:
    original data
    rect = {x1, y1, x2, y2}
    extended to
    xrect = {lower_left, lower_right, upper_left, upper_right, square}
    lower_left  = x1 + y1 * 32
    lower_right = x2 + y1 * 32
    upper_left  = x1 + y2 * 32
    upper_right = x2 + y2 * 32
    square      = (x2 - x1) * (y2 - y1)
    */
    vector <array <int, 5>> xrects;
    int x1, y1, x2, y2;
    int lower_left, lower_right, upper_left, upper_right, square;
    for (auto rect : rects) {
        x1 = rect[0]; y1 = rect[1]; x2 = rect[2]; y2 = rect[3];
        lower_left = x1 + y1 * MAX_X; lower_right = x2 + y1 * MAX_X;
        upper_left = x1 + y2 * MAX_X; upper_right = x2 + y2 * MAX_X;
        square = (x2 - x1) * (y2 - y1);
        xrects.push_back({lower_left, lower_right, upper_left, upper_right, square});
    }

    vector <Solution> solutions;

    // set for collecting base vertices
    set <int> xbases;

    while (xrects.size() != 0) {

        // refresh set
        xbases.clear();
        for (auto xrect : xrects) {
            xbases.insert(xrect[0]);
        }

        // not-processed xrects with the same base vertex
        vector <array <int, 5>> same;
        // will try to move xrects without children from "xrects" to "solutions"
        for (auto it = xrects.begin(); it != xrects.end(); ++it) {
            auto xrect = *it;
            for (auto it2 = xrects.begin(); it2 != xrects.end(); ++it2) {
                auto xrect2 = *it2;
                if (xrect[0] == xrect2[0] && (xrect[1] != xrect2[1] || xrect[2] != xrect2[2] || xrect[3] != xrect2[3])) {
                    same.push_back(xrect2);
                }
            }
            bool ll = xbases.contains(xrect[0]);
            bool lr = xbases.contains(xrect[1]);
            bool ul = xbases.contains(xrect[2]);
            bool ur = xbases.contains(xrect[3]);
            if (ll && !lr && !ul && !ur) {
                push_xrect(n, m, p, xrect, same, xrects, solutions);
                xrects.erase(it);
                --it;
            }
        }
    }

    // calculate best solution
    int result = MAX_P;
    int map_square = 0;
    int counter = 0;
    for (auto solution : solutions) {
        for (auto candidate : solution.sc) {
            map_square = candidate[0];
            counter = candidate[1];

            if (solution.xy[0] == 0 && solution.xy[1] == 0 && map_square == n * m) {
                result = min(counter, result);
            }
        }
    }
    return result == MAX_P ? -1 : result;
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
