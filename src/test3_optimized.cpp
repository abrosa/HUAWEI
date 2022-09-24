//#include <algorithm>
#include <iostream>
#include <vector>
//#include <array>
#include <set>

using namespace std;

class Solutions {
    public:
        // map size
        int n;
        int m;
        // coordinates
        uint8_t x1;
        uint8_t y1;
        uint8_t x2;
        uint8_t y2;
        // rect with packed coordinates
        uint32_t rect;
        // rect square and counter = 1?
        uint16_t square;
        uint16_t counter;
        // solution with square and counter packed
        uint32_t sol;
        // set of all solutions
        set <uint32_t> sols;
        // info about children
        bool has_children;
        set <uint32_t> children;
        // info about parents
        bool has_parents;
        set <uint32_t> parents;
        // if this rect started at {0, 0}
        bool is_root;
        // if this rect ended at {n, m}
        bool is_leaf;
};

void add_sols(int & n, int & m, uint32_t & curr_rect, uint32_t & child_rect, vector <Solutions> & vsols) {
    // add child solutions to curr
    // x is current object
    for (auto & x : vsols) {
    if (curr_rect == x.rect) {
        // y is child object
        for (auto & y : vsols) {
        if (child_rect == y.rect) {
            // z is one of child solutions
            for (auto & z : y.sols) {
                // insert to current all solutions from children
                x.sols.insert(x.sol + z);
            }
        }
        }
    }
    }
}

void merge_solutions(int n, int m, int p, int x1, int y1, set <uint32_t> & solutions1, set <uint32_t> & solutions2) {
    // merge second solutions to the first one
    // just copy second
    solutions1.insert(solutions2.begin(), solutions2.end());
    // iterate through first
    for (auto & solution1 : solutions1) {
        // iterate through second
        for (auto & solution2 : solutions2) {
            // check if solutions sum will be correct
            uint32_t both = solution1 + solution2;
            uint16_t square = both & 65535;
            uint16_t counter = both >> 16;
            if (counter > 0 && counter <= p && square > 0 && square <= (n - x1) * (m - y1)) {
                solutions1.insert(both);
            }
        }
    }
}


bool is_child(uint32_t curr_rect, uint32_t child_rect) {
    uint8_t curr_x1 = curr_rect  >> 24;
    uint8_t curr_y1 = curr_rect  >> 16 & 255;
    uint8_t curr_x2 = curr_rect  >> 8 & 255;
    uint8_t curr_y2 = curr_rect  & 255;
    uint8_t child_x1 = child_rect >> 24;
    uint8_t child_y1 = child_rect >> 16 & 255;
    //uint8_t child_x2 = child_rect >> 8 & 255;
    //uint8_t child_y2 = child_rect & 255;
    if (((curr_x2 == child_x1 && curr_y1 == child_y1) ||
         (curr_x1 == child_x1 && curr_y2 == child_y1))) {
        return true;
    }
    return false;
}

bool is_parent(uint32_t curr_rect, uint32_t parent_rect) {
    uint8_t curr_x1 = curr_rect >> 24;
    uint8_t curr_y1 = curr_rect >> 16 & 255;
    //uint8_t curr_x2 = curr_rect >> 8 & 255;
    //uint8_t curr_y2 = curr_rect & 255;
    uint8_t parent_x1 = parent_rect >> 24;
    uint8_t parent_y1 = parent_rect >> 16 & 255;
    uint8_t parent_x2 = parent_rect >> 8 & 255;
    uint8_t parent_y2 = parent_rect & 255;
    if (((curr_x1 == parent_x2 && curr_y1 == parent_y1) ||
         (curr_x1 == parent_x1 && curr_y1 == parent_y2))) {
        return true;
    }
    return false;
}

void collect_info(int n, int m, vector <uint32_t> & rects, vector <Solutions> & vsols) {
    // collect info for Solutions class
    vsols.clear();
    for (auto & x : rects) {
        Solutions curr;
        curr.n = n;
        curr.m = m;
        curr.has_children = false;
        curr.has_parents = false;
        curr.children.clear();
        curr.parents.clear();
        curr.rect = x;
        curr.x1 = x >> 24;
        curr.y1 = x >> 16 & 255;
        curr.x2 = x >> 8 & 255;
        curr.y2 = x & 255;
        curr.is_root = ((x >> 16) == 0);
        curr.is_leaf = curr.x2 == n && curr.y2 == m;
        curr.square = (curr.x2 - curr.x1) * (curr.y2 - curr.y1);
        curr.counter = 1;
        curr.sol = curr.square + (curr.counter << 16);
        curr.sols = {curr.sol};
        for (auto & y : rects) {
            if (is_child(x, y)) {
                curr.has_children = true;
                curr.children.insert(y);
            }
            if (is_parent(x, y)) {
                curr.has_parents = true;
                curr.parents.insert(y);
            }
        }
        vsols.push_back(curr);
    }
}

void update_info(int n, int m, vector <uint32_t> & rects, vector <Solutions> & vsols) {
    // collect info for Solutions class
    for (auto & x : vsols) {
        x.has_children = false;
        x.has_parents = false;
        x.children.clear();
        x.parents.clear();
        for (auto & y : rects) {
            if (is_child(x.rect, y)) {
                x.has_children = true;
                x.children.insert(y);
            }
            if (is_parent(x.rect, y)) {
                x.has_parents = true;
                x.parents.insert(y);
            }
        }
    }
}

set <uint32_t> get_sols(uint32_t & rect, vector <Solutions> & vsols) {
    set <uint32_t> sols;
    for (auto & vsol : vsols) {
        if (vsol.rect == rect) {
            sols = vsol.sols;
        }
    }
    return sols;
}

void set_sols(uint32_t & rect, set <uint32_t> & sols, vector <Solutions> & vsols) {
    for (auto & vsol : vsols) {
        if (vsol.rect == rect) {
            vsol.sols = sols;
        }
    }
}

void print_rect(uint32_t x) {
    cout << int(x>>24) << "'" << int(x>>16&255) << "'" << int(x>>8&255) << "'" << int(x&255);
}

void remove_rect(uint32_t rect, vector <uint32_t> & rects) {
    for (auto it = rects.begin(); it != rects.end(); ++it) {
        auto curr = * it;
        if (curr == rect) {
            rects.erase(it);
            --it;
        }
    }
}

int walkthrough(int & n, int & m, vector <uint32_t> & rects) {
    int p = rects.size();
    vector <Solutions> vsols;
    collect_info(n, m, rects, vsols);
    set <uint32_t> old_sols;
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto & x : vsols) {
            if (! x.has_children) {
                if (x.has_parents || x.is_root) {
                    for (auto & y : vsols) {
                        old_sols = y.sols;
                        if (x.parents.contains(y.rect)) {
                            merge_solutions(n, m, p, y.x1, y.y1, y.sols, x.sols);
                        }
                        if (y.sols != old_sols) changed = true;
                    }
                    if (! x.is_root) {
                        remove_rect(x.rect, rects);
                    }
                }
            }
        }
        update_info(n, m, rects, vsols);
    } 

    // Iterate through solutions to find the best
    int min_count = p + 1;
    for (auto & solution : vsols) {
        if (! solution.is_root) continue;
        for (auto & curr_solution : solution.sols) {
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
    int n = 10, m = 10;
    // Fill rects
    vector <uint32_t> rects;
    for (int i = 0; i < n; i += 1){
        for (int j = 0; j < m; j += 1){
            pack_rect(i, j, i + 1, j + 1, rects);
        }
    }
    // Print result
    cout << walkthrough(n, m, rects) << endl;
}
