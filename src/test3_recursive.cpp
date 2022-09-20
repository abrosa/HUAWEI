#include <iostream>
#include <vector>
#include <array>
#include <set>

#define SIZE_X 3
#define SIZE_Y 3

/*
TODO:
convert coordinates from {x, y} to uint_32 x + (y << 16)
convert score from {square, counter} to uint_32 square + (counter << 16)
done
*/

using namespace std;

//typedef vector <array <int, 2>> result_type;
typedef vector <int> result_type;

//class Rectangle {
//    public:
//        int x1;
//        int y1;
//        int x2;
//        int y2;
//};

//class Score {
//    public:
//        int square;
//        int counter;
//};

//class Vertex {
//    public:
//        int x;
//        int y;
//};

//bool operator==(const Vertex & v1, const Vertex & v2) {
//    return v1.x == v2.x && v1.y == v2.y;
//};

//bool operator!=(const Vertex & v1, const Vertex & v2) {
//    return v1.x != v2.x || v1.y != v2.y;
//};

result_type add_results(result_type results1, result_type results2) {
    // will add content
    result_type results;
    for (int & result1 : results1) {
        int s1 = result1 % 1024;
        int c1 = result1 / 1024;
        for (int & result2 : results2) {
            int s2 = result1 % 1024;
            int c2 = result1 / 1024;
            results.push_back((s1 + s2) + 1024 * (c1 + c2));
        }
    }
    return results;
}

result_type pack_results(result_type results, int & map_size, int & rects_size) {
    // collect uniq squares
    set <int> uniq_squares;
    for (auto & result : results) {
        uniq_squares.insert(result%1024);
    }

    // merge good and remove bad results
    int min_counter;
    result_type new_results;
    for (auto & result : results) {
        auto square = result%1024;
        auto counter = result/1024;
        if (square <= map_size && square >= 1 && counter <= rects_size && counter >= 1) {
            min_counter = rects_size;
            for (auto & curr_square : uniq_squares) {
                min_counter = min(min_counter, counter);
            }
            new_results.push_back(square + 1024 * min_counter);
        }
    }
    return new_results;
}

result_type walkthrough(int & n, int & m, int & i, vector <array <int, 4>> & xrects) {
    // vertices of current rectangle
    int ll_rect, lr_rect, ul_rect, ur_rect;
//    for (int i = 0; i < xrects.size(); ++i) {
        ll_rect = xrects[i][0];
        lr_rect = xrects[i][1];
        ul_rect = xrects[i][2];
        ur_rect = xrects[i][3];
//    }
    // lists of child rectangles
    vector <int> lr_index, ul_index, ur_index;
    for (int i = 0; i < xrects.size(); ++i) {
        if (lr_rect == ll_rect) lr_index.push_back(i);
        if (ul_rect == ll_rect) ul_index.push_back(i);
        if (ur_rect == ll_rect) ur_index.push_back(i);
    }

    // recursion magic
    int map_size = n * m;
    int rects_size = xrects.size();
    int x1 = xrects[i][0] % 1024;
    int y1 = xrects[i][0] / 1024;
    int x2 = xrects[i][3] % 1024;
    int y2 = xrects[i][3] / 1024;
    int square = (x2 - x1) * (y2 - y1);

    result_type current_rect;
    current_rect = {{square, 1}};

    if (lr_index.size() == 0 && ul_index.size() == 0 && ur_index.size() == 0) {
        return current_rect;
    } else if (lr_index.size() != 0 && ul_index.size() == 0 && ur_index.size() == 0) {
        result_type lr_tmp; for (auto & lr_i : lr_index) lr_tmp = walkthrough(n, m, lr_i, xrects);
        return add_results(current_rect, lr_tmp);
    } else if (lr_index.size() == 0 && ul_index.size() != 0 && ur_index.size() == 0) {
        result_type ul_tmp; for (auto & ul_i : ul_index) ul_tmp = walkthrough(n, m, ul_i, xrects);
        return add_results(current_rect, ul_tmp);
    } else if (lr_index.size() == 0 && ul_index.size() == 0 && ur_index.size() != 0) {
        result_type ur_tmp; for (auto & ur_i : ur_index) ur_tmp = walkthrough(n, m, ur_i, xrects);
        return add_results(current_rect, ur_tmp);
    } else if (lr_index.size() == 0 && ul_index.size() != 0 && ur_index.size() != 0) {
        result_type ul_tmp; for (auto & ul_in : ul_index) ul_tmp = walkthrough(n, m, ul_in, xrects);
        ul_tmp = add_results(current_rect, ul_tmp);
        result_type ur_tmp; for (auto & ur_in : ur_index) ur_tmp = walkthrough(n, m, ur_in, xrects);
        ur_tmp = add_results(current_rect, ur_tmp);
        return pack_results(add_results(ul_tmp, ur_tmp), map_size, rects_size);
    } else if (lr_index.size() != 0 && ul_index.size() == 0 && ur_index.size() != 0) {
        result_type lr_tmp; for (auto & lr_in : lr_index) lr_tmp = walkthrough(n, m, lr_in, xrects);
        lr_tmp = add_results(current_rect, lr_tmp);
        result_type ur_tmp; for (auto & ur_in : ur_index) ur_tmp = walkthrough(n, m, ur_in, xrects);
        ur_tmp = add_results(current_rect, ur_tmp);
        return pack_results(add_results(lr_tmp, ur_tmp), map_size, rects_size);
    } else if (lr_index.size() != 0 && ul_index.size() != 0 && ur_index.size() == 0) {
        result_type lr_tmp; for (auto & lr_in : lr_index) lr_tmp = walkthrough(n, m, lr_in, xrects);
        lr_tmp = add_results(current_rect, lr_tmp);
        result_type ul_tmp; for (auto & ul_in : ul_index) ul_tmp = walkthrough(n, m, ul_in, xrects);
        ul_tmp = add_results(current_rect, ul_tmp);
        return pack_results(add_results(lr_tmp, ul_tmp), map_size, rects_size);
    } else if (lr_index.size() != 0 && ul_index.size() != 0 && ur_index.size() != 0) {
        result_type lr_tmp; for (auto & lr_ind : lr_index) lr_tmp = walkthrough(n, m, lr_ind, xrects);
        result_type ul_tmp; for (auto & ul_ind : ul_index) ul_tmp = walkthrough(n, m, ul_ind, xrects);
        result_type ur_tmp; for (auto & ur_ind : ur_index) ur_tmp = walkthrough(n, m, ur_ind, xrects);
        // will merge it
        result_type lr_ul_ur_res;
        lr_ul_ur_res.push_back(square + 1024);
        for (auto & result1 : lr_tmp) {
        lr_ul_ur_res.push_back((result1%1024 + square) + 1024 * (result1/1024 + 1));
        for (auto & result2 : ul_tmp) {
        lr_ul_ur_res.push_back((result1%1024 + result2%1024 + square) + 1024 * (result1/1024 + result2/1024 + 1));
        for (auto & result3 : ur_tmp) {
        lr_ul_ur_res.push_back((result2%1024 + square) + 1024 * (result2/1024 + 1));
        lr_ul_ur_res.push_back((result3%1024 + square) + 1024 * (result3/1024 + 1));
        lr_ul_ur_res.push_back((result2%1024 + result3%1024 + square) + 1024 * (result2/1024 + result3/1024 + 1));
        lr_ul_ur_res.push_back((result1%1024 + result3%1024 + square) + 1024 * (result1/1024 + result3/1024 + 1));
        lr_ul_ur_res.push_back((result1%1024 + result2%1024 + result3%1024 + square) + 1024 * (result1/1024 + result2/1024 + result3/1024 + 1));
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
    vector <array <int, 4>> rects;
    array <int, 4> coords;

    // constants
    int n = 3;
    int m = 3;
    int p = 5;

    //rects.clear();
    coords = {0, 0, 2, 1}; rects.push_back(coords);
    coords = {2, 0, 3, 2}; rects.push_back(coords);
    coords = {0, 1, 1, 3}; rects.push_back(coords);
    coords = {1, 2, 3, 3}; rects.push_back(coords);
    coords = {1, 1, 2, 2}; rects.push_back(coords);

    vector <array <int, 4>> xrects;
    array <int, 4> vertices;
    for (int i = 0; i < rects.size(); ++i) {
        auto rect = rects[i];
        vertices = {rect[0]+rect[1]*1024, rect[2]+rect[1]*1024, rect[0]+rect[3]*1024, rect[2]+rect[3]*1024};
        xrects.push_back(vertices);
    }

    // some magic
    int min_count = p;
    for (int i = 0; i < xrects.size(); ++i) {
        if (xrects[i][0] == 0) {
            // call recurrent function
            int ind = i;
            result_type tree_results = walkthrough(n, m, ind, xrects);
            for (auto & result : tree_results) {
                int square = result % 1024;
                int counter = result / 1024;
                if (square == n * m) {
                    min_count = min(min_count, counter);
                }
            }
        }
    }
    cout << min_count << endl;
}
