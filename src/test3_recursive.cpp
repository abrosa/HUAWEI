#include <iostream>
#include <vector>
#include <array>
#include <set>

#define SIZE_X 3
#define SIZE_Y 3

using namespace std;

typedef vector <array <int, 2>> result_type;

class Rectangle {
    public:
        int x1;
        int y1;
        int x2;
        int y2;
};

class Score {
    public:
        int square;
        int counter;
};

class Vertex {
    public:
        int x;
        int y;
};

bool operator==(const Vertex & v1, const Vertex & v2) {
    return v1.x == v2.x && v1.y == v2.y;
};

bool operator!=(const Vertex & v1, const Vertex & v2) {
    return v1.x != v2.x || v1.y != v2.y;
};

result_type add_results(result_type results1, result_type results2) {
    // will add content
    result_type results;
    for (auto & result1 : results1) {
        for (auto & result2 : results2) {
            results.push_back({result1[0] + result2[0], result1[1] + result2[1]});
        }
    }
    return results;
}

result_type pack_results(result_type results, int & map_size, int & rects_size) {
    // collect uniq squares
    set <int> uniq_squares;
    for (auto & result : results) {
        uniq_squares.insert(result[0]);
    }

    // merge good and remove bad results
    int min_counter;
    result_type new_results;
    for (auto & result : results) {
        auto square = result[0];
        auto counter = result[1];
        if (square <= map_size && square >= 1 && counter <= rects_size && counter >= 1) {
            min_counter = rects_size;
            for (auto & curr_square : uniq_squares) {
                min_counter = min(min_counter, counter);
            }
            new_results.push_back({square, min_counter});
        }
    }
    return new_results;
}

result_type walkthrough(int & n, int & m, int & i, vector <Rectangle> & rects) {
    // emergency out
    if (i < 0 || i >= rects.size()) {
        result_type null_result = {{0, 0}};
        return null_result;
    }

    // vertices of current rectangle
    Vertex ll_rect, lr_rect, ul_rect, ur_rect;
    lr_rect = {rects[i].x2, rects[i].y1};
    ul_rect = {rects[i].x1, rects[i].y2};
    ur_rect = {rects[i].x2, rects[i].y2};

    // lists of child rectangles
    vector <int> lr_index, ul_index, ur_index;
    for (int i = 0; i < rects.size(); ++i) {
        ll_rect = {rects[i].x1, rects[i].y1};
        if (lr_rect == ll_rect) lr_index.push_back(i);
        if (ul_rect == ll_rect) ul_index.push_back(i);
        if (ur_rect == ll_rect) ur_index.push_back(i);
    }

    // recursion magic
    int map_size = n * m;
    int rects_size = rects.size();
    int square = (rects[i].x2 - rects[i].x1) * (rects[i].y2 - rects[i].y1);
    result_type current_rect;
    current_rect = {{square, 1}};

    if (lr_index.size() == 0 && ul_index.size() == 0 && ur_index.size() == 0) {
        return current_rect;
    } else if (lr_index.size() != 0 && ul_index.size() == 0 && ur_index.size() == 0) {
        result_type lr_tmp; for (auto & lr_i : lr_index) lr_tmp = walkthrough(n, m, lr_i, rects);
        return add_results(current_rect, lr_tmp);
    } else if (lr_index.size() == 0 && ul_index.size() != 0 && ur_index.size() == 0) {
        result_type ul_tmp; for (auto & ul_i : ul_index) ul_tmp = walkthrough(n, m, ul_i, rects);
        return add_results(current_rect, ul_tmp);
    } else if (lr_index.size() == 0 && ul_index.size() == 0 && ur_index.size() != 0) {
        result_type ur_tmp; for (auto & ur_i : ur_index) ur_tmp = walkthrough(n, m, ur_i, rects);
        return add_results(current_rect, ur_tmp);
    } else if (lr_index.size() == 0 && ul_index.size() != 0 && ur_index.size() != 0) {
        result_type ul_tmp; for (auto & ul_in : ul_index) ul_tmp = walkthrough(n, m, ul_in, rects);
        ul_tmp = add_results(current_rect, ul_tmp);
        result_type ur_tmp; for (auto & ur_in : ur_index) ur_tmp = walkthrough(n, m, ur_in, rects);
        ur_tmp = add_results(current_rect, ur_tmp);
        return add_results(ul_tmp, ur_tmp);
    } else if (lr_index.size() != 0 && ul_index.size() == 0 && ur_index.size() != 0) {
        result_type lr_tmp; for (auto & lr_in : lr_index) lr_tmp = walkthrough(n, m, lr_in, rects);
        lr_tmp = add_results(current_rect, lr_tmp);
        result_type ur_tmp; for (auto & ur_in : ur_index) ur_tmp = walkthrough(n, m, ur_in, rects);
        ur_tmp = add_results(current_rect, ur_tmp);
        return add_results(lr_tmp, ur_tmp);
    } else if (lr_index.size() != 0 && ul_index.size() != 0 && ur_index.size() == 0) {
        result_type lr_tmp; for (auto & lr_in : lr_index) lr_tmp = walkthrough(n, m, lr_in, rects);
        lr_tmp = add_results(current_rect, lr_tmp);
        result_type ul_tmp; for (auto & ul_in : ul_index) ul_tmp = walkthrough(n, m, ul_in, rects);
        ul_tmp = add_results(current_rect, ul_tmp);
        return add_results(lr_tmp, ul_tmp);
    } else if (lr_index.size() != 0 && ul_index.size() != 0 && ur_index.size() != 0) {
        result_type lr_tmp; for (auto & lr_ind : lr_index) lr_tmp = walkthrough(n, m, lr_ind, rects);
        result_type ul_tmp; for (auto & ul_ind : ul_index) ul_tmp = walkthrough(n, m, ul_ind, rects);
        result_type ur_tmp; for (auto & ur_ind : ur_index) ur_tmp = walkthrough(n, m, ur_ind, rects);
        // will merge it
        result_type lr_ul_ur_res;
        for (auto & result1 : lr_tmp) {
        for (auto & result2 : ul_tmp) {
        for (auto & result3 : ur_tmp) {
        lr_ul_ur_res.push_back({result1[0] + square, result1[1] + 1});
        lr_ul_ur_res.push_back({result2[0] + square, result2[1] + 1});
        lr_ul_ur_res.push_back({result3[0] + square, result3[1] + 1});
        lr_ul_ur_res.push_back({result2[0] + result3[0] + square, result2[1] + result3[1] + 1});
        lr_ul_ur_res.push_back({result1[0] + result3[0] + square, result1[1] + result3[1] + 1});
        lr_ul_ur_res.push_back({result1[0] + result2[0] + square, result1[1] + result2[1] + 1});
        lr_ul_ur_res.push_back({square, 1});
        lr_ul_ur_res.push_back({result1[0] + result2[0] + result3[0] + square, result1[1] + result2[1] + result3[1] + 1});
        }
        }
        }
        return pack_results(lr_ul_ur_res, map_size, rects_size);
    } else {
        // shouldn't be here
        return current_rect;
    }
} 

int main() {
    // Working data
    vector <Rectangle> rects;
    for (int i = 0; i < SIZE_X; ++i) {
        for (int j = 0; j < SIZE_Y; ++j) {
            rects.push_back({i, j, i + 1, j + 1});
        }
    }

    // constants
    int n = SIZE_X;
    int m = SIZE_Y;
    int p = SIZE_X * SIZE_Y;

    // some magic
    int min_count = p;
    for (int i = 0; i < rects.size(); ++i) {
        if (rects[i].x1 == 0 && rects[i].y1 == 0) {
            // call recurrent function
            int ind = i;
            result_type tree_results = walkthrough(n, m, ind, rects);
            for (auto & result : tree_results) {
                int square = result[0];
                int counter = result[1];
                if (square == n * m) {
                    min_count = min(min_count, counter);
                }
            }
        }
    }
    cout << min_count << endl;
}
