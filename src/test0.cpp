#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Solution {
    public:
        int counter;
        int square;
};

class Vertex {
    public:
        int xy;
        vector <Solution> solution;
        vector <Vertex> children;
};

/*
void merge_scores(int & n, int & m, int & p, xRectangle & xrect1, xRectangle & xrect2) {
    // merge score1 to score2
    int x1, y1, x2, y2, square1, square2, counter1, counter2;
    uint32_t new_square, new_counter, new_score;
    for (auto it1 = xrect1.score.begin(); it1 != xrect1.score.end(); ++it1) {
        auto s1 = * it1;
        for (auto & s2 : xrect2.score) {
            // unpack scores
            x1 = s1 >> 27;
            y1 = s1 >> 22 & 31;
            square1 = s1 >> 12 & 1023;
            counter1 = s1 >> 5 & 127;
            x2 = s2 >> 27;
            y2 = s2 >> 22 & 31;
            square2 = s2 >> 12 & 1023;
            counter2 = s2 >> 5 & 127;
            if (x1 == (xrect2.ll >> 5) && y1 == (xrect2.ll & 31)) {
                new_square = square1 + square2;
                new_counter = counter1 + counter2;
                new_score = (x2 << 27) + (y2 << 22) + (new_square << 12) + (new_counter << 5);
                if (new_square <= n * m && new_counter <= p) {
                    xrect2.score.insert(new_score);
//                    xrect1.score.erase(it1);
  //                  --it1;
                }
            }
        }
    }
}

bool is_child(xRectangle & xrect1, xRectangle & xrect2) {
    return xrect2.ll == xrect1.ul || xrect2.ll == xrect1.lr;
}
*/
void walkthrough(int & n, int & m, int & p, vector <Vertex> & vertices) {
    int square;
    int counter;
    int min_counter = p + 1;
//    Rectangle result;
    int index = 50;
/*
    while (index --> 0) {
        for (int i = 0; i < xrects.size(); ++i) {
            for (int j = 0; j < xrects.size(); ++j) {
                if (xrects[i].is_ll) {
                    merge_scores(n, m, p, xrects[i], xrects[i]);
                }
                if (xrects[j].is_ur) {
                    merge_scores(n, m, p, xrects[j], xrects[j]);
                }
                if (is_child(xrects[i], xrects[j])) {
                    merge_scores(n, m, p, xrects[i], xrects[j]);
                }
            }
            merge_scores(n, m, p, xrects[i], result);
        }
    }
    for (auto & x : xrects) {
        for (auto & y : x.score) {
            cout << int(y >> 27) << "'" << int(y >> 22 & 31) << "'" << int(y >> 12 & 1023) << "'" << int(y >> 5 & 127) << " ";
        }
        cout << endl;
    }
    min_counter = p + 1;
    for (auto & result : xrects) {
        for (auto & score : result.score) {
            square = score >> 12 & 1023;
            counter = score >> 5 & 127;
            if (square == n * m) {
                cout << counter << endl;
                min_counter = min(min_counter, counter);
                break;
            }
        }
    }
*/
    min_counter = (min_counter == p + 1) ? -1 : min_counter;
    cout << min_counter << endl;
}

// recursive?
vector <Vertex> init_vertices(int & n, int & m, int & p, vector <vector <int>> & input, vector <Vertex> & vertices) {
    // vertices for children Vertices
    Vertex vert_ul, vert_lr;
    // solutions for children Vertices
    Solution sol_ul, sol_lr;
    // iterate through input rects
    for (auto & x : input) {
        int x1 = x[0], y1 = x[1], x2 = x[2], y2 = x[3];
        vert_ul.xy = x1 * 32 + y2;
        sol_ul.counter = 1;
        sol_ul.square = (x2 - x1) * (y2 - y1);
        vert_lr.xy = x2 * 32 + y1;
        sol_lr.counter = 1;
        sol_lr.square = (x2 - x1) * (y2 - y1);
        bool ul_exists = false, lr_exists = false; 
        for (auto & v : vertices) {
            if (v.xy == vert_ul.xy) {
                v.solution.push_back(sol_ul);
                ul_exists = true;
                continue;
            }
            if (v.xy == vert_lr.xy) {
                v.solution.push_back(sol_lr);
                lr_exists = true;
                continue;
            }
        }
        if (! ul_exists) {
            vertices.push_back(vert_ul);
        }
        if (! lr_exists) {
            vertices.push_back(vert_lr);
        }
    }
    return vertices;
}

int main() {
    int n = 5, m = 5, p = 45;
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

    vector <Vertex> vertices;
    vertices = init_vertices(n, m, p, input, vertices);
    cout << vertices.size() << endl;
    //walkthrough(n, m, p, rects);
}
