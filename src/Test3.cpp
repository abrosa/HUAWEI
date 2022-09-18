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
        array <int, 4> xrect;
        // candidate = square + counter * MAX_XY
        set <int> candidate;
};

bool is_valid(int n, int m, int p, int candidate) {
    return ((candidate % MAX_XY) <= n * m) && ((candidate / MAX_XY) <= p);
}

void push_xrect(int n, int m, int p, auto &xrect, auto &xrects, auto &solutions) {
    // set for collecting base vertices
    set <int> sbases;

    // refresh vertices set
    sbases.clear();
    for (auto solution : solutions) {
        sbases.insert(solution.xrect[0]);
    }

    bool ll = sbases.contains(xrect[0]);
    bool lr = sbases.contains(xrect[1]);
    bool ul = sbases.contains(xrect[2]);
    bool ur = sbases.contains(xrect[3]);

    // data from children f for flag c for candidate V for Vendetta
    bool ll_f = false, lr_f = false, ul_f = false, ur_f = false;
    set <int> ll_c, lr_c, ul_c, ur_c;

    // check possible solutions
    if (ll || lr || ul || ur) {
        for (auto it = solutions.begin(); it != solutions.end(); ++it) {
            auto solution = *it;
            if (solution.xrect[0] == xrect[0]) {
                ll_f = true;
                ll_c = solution.candidate;
            } else if (solution.xrect[0] == xrect[1]) {
                lr_f = true;
                lr_c = solution.candidate;
            } else if (solution.xrect[0] == xrect[2]) {
                ul_f = true;
                ul_c = solution.candidate;
            } else if (solution.xrect[0] == xrect[3]) {
                ur_f = true;
                ur_c = solution.candidate;
            }
        }
    }

    Solution new_sol;

    new_sol.xrect = {xrect[0], xrect[1], xrect[2], xrect[3]};
    const int curr_cand = xrect[4] + 1 * MAX_XY;
    new_sol.candidate = {curr_cand};

    // nothing in solutions, push just one (new) rect as solution
    if (!lr_f && !ul_f && !ur_f) {
    // just one additional way, increment lr and push as new solution
    } else if (lr_f && !ul_f && !ur_f) {
        for (int c : lr_c)
            if (is_valid(n, m, p, c + curr_cand))
                new_sol.candidate.insert(c + curr_cand);
    // just one additional way, increment ul and push as new solution
    } else if (!lr_f && ul_f && !ur_f) {
        for (int c : ul_c)
            if (is_valid(n, m, p, c + curr_cand))
                new_sol.candidate.insert(c + curr_cand);
    // just one additional way, increment ur and push as new solution
    } else if (!lr_f && !ul_f && ur_f) {
        for (int c : ur_c)
            if (is_valid(n, m, p, c + curr_cand))
                new_sol.candidate.insert(c + curr_cand);
    // two additional ways, push (new) (new + ul + ur) (new + ul) (new + lr)
    } else if (!lr_f && ul_f && ur_f) {
        for (int i : ul_c) {
            if (is_valid(n, m, p, i + curr_cand))
                new_sol.candidate.insert(i + curr_cand);
            for (int j : ur_c) {
                if (is_valid(n, m, p, i + j + curr_cand))
                    new_sol.candidate.insert(i + j + curr_cand);
            }
        }
        for (int c : ur_c)
            if (is_valid(n, m, p, c + curr_cand))
                new_sol.candidate.insert(c + curr_cand);
    // two additional ways, push (new) (new + lr + ur) (new + lr) (new + ur)
    } else if (lr_f && !ul_f && ur_f) {
        for (int i : lr_c) {
            if (is_valid(n, m, p, i + curr_cand))
                new_sol.candidate.insert(i + curr_cand);
            for (int j : ur_c) {
                if (is_valid(n, m, p, i + j + curr_cand))
                    new_sol.candidate.insert(i + j + curr_cand);
            }
        }
        for (int c : ur_c)
            if (is_valid(n, m, p, c + curr_cand))
                new_sol.candidate.insert(c + curr_cand);
    // two additional ways, push (new) (new + lr + ul) (new + lr) (new + ul)
    } else if (lr_f && ul_f && !ur_f) {
        for (int i : lr_c) {
            if (is_valid(n, m, p, i + curr_cand))
                new_sol.candidate.insert(i + curr_cand);
            for (int j : ul_c) {
                if (is_valid(n, m, p, i + j + curr_cand))
                    new_sol.candidate.insert(i + j + curr_cand);
            }
        }
        for (int c : ul_c)
            if (is_valid(n, m, p, c + curr_cand))
                new_sol.candidate.insert(c + curr_cand);
    // three additional ways, push (new) (new + lr + ul + ur)
    // (new + ul + ur) (new + lr + ur) (new + lr + ul) (new + lr) (new + ul) (new + ur)
    } else if (lr_f && ul_f && ur_f) {
        for (int i : lr_c) {
            if (is_valid(n, m, p, i + curr_cand))
                new_sol.candidate.insert(i + curr_cand);
            for (int j : ul_c) {
                if (is_valid(n, m, p, i + j + curr_cand))
                    new_sol.candidate.insert(i + j + curr_cand);
                for (int k : ur_c) {
                    if (is_valid(n, m, p, i + j + k + curr_cand))
                        new_sol.candidate.insert(i + j + k + curr_cand);
                }
            }
        }
        for (int i : ul_c) {
            if (is_valid(n, m, p, i + curr_cand))
                new_sol.candidate.insert(i + curr_cand);
            for (int j : ur_c) {
                if (is_valid(n, m, p, i + j + curr_cand))
                    new_sol.candidate.insert(i + j + curr_cand);
            }
        }
        for (int i : lr_c) {
            for (int j : ur_c) {
                if (is_valid(n, m, p, i + j + curr_cand))
                    new_sol.candidate.insert(i + j + curr_cand);
            }
        }
        for (int c : ur_c)
            if (is_valid(n, m, p, c + curr_cand))
                new_sol.candidate.insert(c + curr_cand);
    } else {
        cout << " shouldn't be here " << endl;
    }

    // if ll exists, add new candidates to it
    for (auto solution : solutions) {
        if (solution.xrect[0] == xrect[0]) {
            solution.candidate.insert(new_sol.candidate.begin(), new_sol.candidate.end());
        }
    }

    // if ll doesn't exist, push new solution
    if (!ll_f) {
        solutions.push_back(new_sol);
    }
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


    // main loop will work until all xrects will be moved to solutions
    while (xrects.size() != 0) {
        // refresh set
        xbases.clear();
        for (auto xrect : xrects) {
            xbases.insert(xrect[0]);
        }
        // will try to move xrects without children from "xrects" to "solutions"
        for (auto it = xrects.begin(); it != xrects.end(); ++it) {
            auto xrect = *it;
            bool lr = xbases.contains(xrect[1]);
            bool ul = xbases.contains(xrect[2]);
            bool ur = xbases.contains(xrect[3]);
            if (!lr && !ul && !ur) {
                push_xrect(n, m, p, xrect, xrects, solutions);
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
        for (auto candidate : solution.candidate) {
            map_square = candidate % MAX_XY;
            counter = candidate / MAX_XY;
            if (solution.xrect[0] == 0 && map_square == n * m) {
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
