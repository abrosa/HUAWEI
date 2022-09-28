#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Vertex {
    public:
        int xy;
        set <int> solution;
};

void merge_solutions(int & n, int & m, int & p, set <int> & solution1, set <int> & solution2) {
    // merge solution1 to solution2 in-place
    int x1, y1, x2, y2, square1, square2, counter1, counter2;
    uint32_t new_square, new_counter, new_score;
    for (auto & s1 : solution1) {
        for (auto & s2 : solution2) {
            new_square = s1 % 1024 + s2 % 1024;
            new_counter = s1 / 1024 + s2 / 1024;
            if (new_square <= n * m && new_counter <= p) {
                solution2.insert(s1 + s2);
            }
        }
    }
}

uint32_t pack_solution(int & square, int & counter, int & x, int & y) {
    uint32_t solution;
    // square 1..900 -> 10bit
    // counter 1..100 -> 7bit
    // x 0..30 -> 5bit
    // y 0..30 -> 5bit
    // 5bit is still free
    solution = ((square & 1023) << 22) | ((counter & 127) << 15) | ((x & 31) << 10) | ((y & 31) << 5);
    return solution;
}

void unpack_solution(uint32_t & solution, int & square, int & counter, int & x, int & y) {
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
    Vertex vert_ll, vert_ul, vert_lr;
    // solutions for children Vertices
    int sol_ul, sol_lr;
    // iterate through input rects
    for (auto & x : input) {
        int x1 = x[0], y1 = x[1], x2 = x[2], y2 = x[3];
        vert_ul.xy = x1 * 32 + y2;
        sol_ul = 1 * 1024 + (x2 - x1) * (y2 - y1);
        vert_ul.solution.insert(sol_ul);
        vert_lr.xy = x2 * 32 + y1;
        sol_lr = 1 * 1024 + (x2 - x1) * (y2 - y1);
        vert_lr.solution.insert(sol_lr);
        bool ul_exists = false, lr_exists = false; 
        // check if vertices already exist
        for (auto & v : vertices) {
            if (v.xy == vert_ul.xy) {
                ul_exists = true;
                v.solution.insert(sol_ul);
            }
            if (v.xy == vert_lr.xy) {
                lr_exists = true;
                v.solution.insert(sol_lr);
            }
        }
        // push new vertices
        if (! ul_exists) {
            vertices.push_back(vert_ul);
        }
        if (! lr_exists) {
            vertices.push_back(vert_lr);
        }
    }
    // second iteration. now all vertices are exist
    // and already have all base solutions inside
    for (auto & x : input) {
        int x1 = x[0], y1 = x[1], x2 = x[2], y2 = x[3];
        vert_ll.xy = x1 * 32 + y1;
        vert_ul.xy = x1 * 32 + y2;
        vert_lr.xy = x2 * 32 + y1;
        // all vertices are already exist
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            auto vertex = * it;
            // search for parent vertex
            if (vert_ll.xy == vertex.xy) {
                bool info_moved = false;
                for (auto & child : vertices) {
                    // search for child vertex
                    if (child.xy == vert_ul.xy) {
                        //merge_solutions(n, m, p, vertex.solution, child.solution);
                        info_moved = true;
                    }
                    if (child.xy == vert_lr.xy) {
                        //merge_solutions(n, m, p, vertex.solution, child.solution);
                        info_moved = true;
                    }
                }
                if (info_moved) {
                    //vertices.erase(it);
                    //--it;
                }
            }
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
    for (auto v : vertices) {
        for (auto x : v.solution) {
            cout << x / 1024 << "'" << x % 1024 << ",";
        }
        cout << endl;
    }
}
