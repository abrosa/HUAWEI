#include <iostream>
#include <vector>
#include <set>

#include "test0.hpp"

using namespace std;

class Vertex {
    public:
        int xy;
        set <uint32_t> solution;
};

void merge_solutions(int & n, int & m, int & p, set <uint32_t> & solution1, set <uint32_t> & solution2) {
    // merge solution1 to solution2 in-place
    int x1, y1, x2, y2, square1, square2, counter1, counter2;
    uint32_t new_solution;
    for (auto s1 : solution1) {
        unpack_solution(s1, square1, counter1, x1, y1);
        for (auto s2 : solution2) {
            unpack_solution(s2, square2, counter2, x2, y2);
            new_solution = pack_solution(square1 + square2, counter1 + counter2, x2, y2);
            if (square1 + square2 <= n * m && counter1 + counter2 <= p && x2 <= n && y2 <= m) {
                if ((x2 - x1) * (y2 - y1) == square1) {
                    solution2.insert(new_solution);
                }
            }
        }
    }
}

uint32_t pack_solution(int square, int counter, int x, int y) {
    uint32_t solution;
    // square 1..900 -> 10bit
    // counter 1..100 -> 7bit
    // x 0..30 -> 5bit
    // y 0..30 -> 5bit
    // 5bit is still free
    solution = ((square & 1023) << 22) | ((counter & 127) << 15) | ((x & 31) << 10) | ((y & 31) << 5);
    return solution;
}

void unpack_solution(const uint32_t & solution, int & square, int & counter, int & x, int & y) {
    // 10 bit -> square 1..900
    //  7 bit -> counter 1..100
    //  5 bit -> x 0..30
    //  5 bit -> y 0..30
    //  5 bit -> not used
    square = solution >> 22 & 1023;
    counter = solution >> 15 & 127;
    x = solution >> 10 & 31;
    y = solution >> 5 & 31;
}

vector <Vertex> init_vertices(int & n, int & m, int & p, vector <vector <int>> & input, vector <Vertex> & vertices) {
    // vertices for children Vertices
    Vertex vert_ll, vert_ul, vert_lr, vert_ur;
    // solutions for children Vertices
    int sol_ll, sol_ul, sol_lr, sol_ur;
    int x1, y1, x2, y2;
    int square;
    // iterate through input rects
    for (auto & rect : input) {
        x1 = rect[0]; y1 = rect[1]; x2 = rect[2]; y2 = rect[3];
        vert_ll.xy = x1 * 32 + y1;
        vert_ul.xy = x1 * 32 + y2;
        vert_lr.xy = x2 * 32 + y1;
        vert_ur.xy = x2 * 32 + y2;
        square = (x2 - x1) * (y2 - y1);
        sol_ll = pack_solution(square, 1, x1, y1);
        sol_ul = pack_solution(square, 1, x1, y2);
        sol_lr = pack_solution(square, 1, x2, y1);
        sol_ur = pack_solution(square, 1, x2, y2);
        vert_ll.solution.insert(sol_ll);
        vert_ul.solution.insert(sol_ul);
        vert_lr.solution.insert(sol_lr);
        vert_ur.solution.insert(sol_ur);
        bool ll_exists = false, ul_exists = false; 
        bool lr_exists = false, ur_exists = false; 
        // check if vertices already exist
        for (auto & v : vertices) {
            if (v.xy == vert_ll.xy) {
                ll_exists = true;
                v.solution.insert(sol_ll);
            }
            if (v.xy == vert_ul.xy) {
                ul_exists = true;
                v.solution.insert(sol_ul);
            }
            if (v.xy == vert_lr.xy) {
                lr_exists = true;
                v.solution.insert(sol_lr);
            }
            if (v.xy == vert_ur.xy) {
                ur_exists = true;
                v.solution.insert(sol_ur);
            }
        }
        // push new vertices
        if (! ll_exists) vertices.push_back(vert_ll);
        if (! ul_exists) vertices.push_back(vert_ul);
        if (! lr_exists) vertices.push_back(vert_lr);
        if (! ur_exists) vertices.push_back(vert_ur);
    }
    // second iteration. now all vertices are exist
    // and already have all base solutions inside
    //for (auto & v : vertices) {
    //    int square, counter, x, y;
    //    set <uint32_t> tmp;
    //    for (auto & sol : v.solution) {
    //        unpack_solution(sol, square, counter, x, y);
    //        if (v.xy == x * 32 + y) {
    //            tmp.insert(sol);
    //        }
    //    }
    //    v.solution = tmp;
    //}
    for (auto & x : input) {
        int x1 = x[0], y1 = x[1], x2 = x[2], y2 = x[3];
        vert_ll.xy = x1 * 32 + y1;
        vert_ul.xy = x1 * 32 + y2;
        vert_lr.xy = x2 * 32 + y1;
        vert_ur.xy = x2 * 32 + y2;
        bool info_moved = true;
        int counter = 10;
        while (counter --> 0) {
        //while (info_moved) {
            // all vertices are already exist
            for (auto it = vertices.begin(); it != vertices.end(); ++it) {
                auto vertex = * it;
                info_moved = false;
                // search for parent vertex ll || ur
                if (vert_ll.xy == vertex.xy || vert_ur.xy == vertex.xy) {
                    for (auto & child : vertices) {
                        // search for child vertex
                        if (child.xy == vert_ul.xy || child.xy == vert_lr.xy) {
                            merge_solutions(n, m, p, vertex.solution, child.solution);
                            info_moved = true;
                        }
                    }
                }
                // search for parent vertex ul || lr
                if (vert_ul.xy == vertex.xy || vert_lr.xy == vertex.xy) {
                    for (auto & child : vertices) {
                        // search for child vertex
                        if (child.xy == vert_ll.xy || child.xy == vert_ur.xy) {
                            merge_solutions(n, m, p, vertex.solution, child.solution);
                            info_moved = true;
                        }
                    }
                }
                if (info_moved) {
                    //vertices.erase(it);
                    //--it;
                }
            }
        }
    }
    //for (auto & v : vertices) {
    //    int square, counter, x, y;
    //    set <uint32_t> tmp;
    //    for (auto & sol : v.solution) {
    //        unpack_solution(sol, square, counter, x, y);
    //        if (v.xy == x * 32 + y) {
    //            tmp.insert(sol);
    //        }
    //    }
    //    v.solution = tmp;
    //}
    return vertices;
}

int main() {
/*
    int n = 3, m = 3, p = 8;
    vector <vector <int>> input;
    input = { {0, 0, 1, 1},
              {0, 1, 1, 2},
              {1, 0, 2, 1},
              {1, 1, 2, 2},
              {0, 0, 2, 1},
              {2, 0, 3, 2},
              {0, 1, 1, 3},
              {2, 1, 3, 3} };
*/
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
    //cout << vertices.size() << endl;
    int min_count = p + 1;
    for (auto v : vertices) {
        //cout << "vertex " << v.xy / 32 << "'" << v.xy % 32 << " ";
        //cout << "solutions ";
        for (auto solution : v.solution) {
            int square, counter, x, y;
            unpack_solution(solution, square, counter, x, y);
            if (square == n * m) {
                min_count = min(min_count, counter);
            }
            //cout << square << "'" << counter << "'" << x << "'" << y << " ";
        }
        //cout << endl;
    }
    int result = (min_count == p + 1) ? -1 : min_count;
    cout << result << endl;
}
