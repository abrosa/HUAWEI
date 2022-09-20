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

typedef vector <uint32_t> result_type;

result_type add_results(result_type results1, result_type results2) {
    // will add content
    result_type results;
    for (uint32_t & result1 : results1) {
        uint32_t s1 = result1 & 0x0000FFFF;
        uint32_t c1 = result1 >> 16;
        for (uint32_t & result2 : results2) {
            uint32_t s2 = result1 & 0x0000FFFF;
            uint32_t c2 = result1 >> 16;
            results.push_back(s1 + s2 + ((c1 + c2) << 16));
        }
    }
    return results;
}

result_type pack_results(result_type results, uint32_t & map_size, uint32_t & rects_size) {
    // collect uniq squares
    set <uint32_t> uniq_squares;
    for (auto & result : results) {
        uniq_squares.insert(result & 0x0000FFFF);
    }

    // merge good and remove bad results
    uint32_t min_counter;
    result_type new_results;
    for (auto & result : results) {
        auto square = result & 0x0000FFFF;
        auto counter = result >> 16;
        if (square <= map_size && square != 0 && counter <= rects_size && counter != 0) {
            min_counter = rects_size;
            for (auto & curr_square : uniq_squares) {
                min_counter = min(min_counter, counter);
            }
            new_results.push_back(square + (min_counter << 16));
        }
    }
    return new_results;
}

result_type walkthrough(uint32_t & n, uint32_t & m, uint32_t & i, vector <array <uint32_t, 4>> & xrects) {
    // vertices of current rectangle
    uint32_t ll_rect, lr_rect, ul_rect, ur_rect;
//    for (uint32_t i = 0; i < xrects.size(); ++i) {
        ll_rect = xrects[i][0];
        lr_rect = xrects[i][1];
        ul_rect = xrects[i][2];
        ur_rect = xrects[i][3];
//    }
    // lists of child rectangles
    vector <uint32_t> lr_index, ul_index, ur_index;
    for (uint32_t i = 0; i < xrects.size(); ++i) {
        if (lr_rect == ll_rect) lr_index.push_back(i);
        if (ul_rect == ll_rect) ul_index.push_back(i);
        if (ur_rect == ll_rect) ur_index.push_back(i);
    }

    // recursion magic
    uint32_t map_size = n * m;
    uint32_t rects_size = xrects.size();
    uint32_t x1 = xrects[i][0] & 0x0000FFFF;
    uint32_t y1 = xrects[i][0] >> 16;
    uint32_t x2 = xrects[i][3] & 0x0000FFFF;
    uint32_t y2 = xrects[i][3] >> 16;
    uint32_t square = (x2 - x1) * (y2 - y1);

    result_type current_rect;
    current_rect = {square + (1 << 16)};

    if        (lr_index.size() == 0 && ul_index.size() == 0 && ur_index.size() == 0) {
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
        lr_ul_ur_res.push_back(square + (1 << 16));
        for (auto & result1 : lr_tmp) {
        lr_ul_ur_res.push_back((result1 & 0x0000FFFF + square) + (((result1 >> 16) + 1) << 16));
        for (auto & result2 : ul_tmp) {
        lr_ul_ur_res.push_back((result1 & 0x0000FFFF + result2 & 0x0000FFFF + square) + (((result1 >> 16) + (result2 >> 16) + 1) << 16));
        for (auto & result3 : ur_tmp) {
        lr_ul_ur_res.push_back((result2 & 0x0000FFFF + square) + (((result2 >> 16 + 1)) << 16));
        lr_ul_ur_res.push_back((result3 & 0x0000FFFF + square) + (((result3 >> 16 + 1)) << 16));
        lr_ul_ur_res.push_back((result2 & 0x0000FFFF + result3 & 0x0000FFFF + square) + (((result2 >> 16) + (result3 >> 16) + 1) << 16));
        lr_ul_ur_res.push_back((result1 & 0x0000FFFF + result3 & 0x0000FFFF + square) + (((result1 >> 16) + (result3 >> 16) + 1) << 16));
        lr_ul_ur_res.push_back((result1 & 0x0000FFFF + result2 & 0x0000FFFF + result3 & 0x0000FFFF + square) + (((result1 >> 16) + (result2 >> 16) + (result3 >> 16) + 1) << 16));
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
    vector <array <uint32_t, 4>> rects;
    array <uint32_t, 4> coords;

    // constants
    uint32_t n = 3;
    uint32_t m = 3;
    uint32_t p = 5;

    //rects.clear();
    coords = {0, 0, 2, 1}; rects.push_back(coords);
    coords = {2, 0, 3, 2}; rects.push_back(coords);
    coords = {0, 1, 1, 3}; rects.push_back(coords);
    coords = {1, 2, 3, 3}; rects.push_back(coords);
    coords = {1, 1, 2, 2}; rects.push_back(coords);

    vector <array <uint32_t, 4>> xrects;
    array <uint32_t, 4> vertices;
    for (uint32_t i = 0; i < rects.size(); ++i) {
        auto rect = rects[i];
        vertices = {rect[0] + (rect[1] << 16),
                    rect[2] + (rect[1] << 16),
                    rect[0] + (rect[3] << 16),
                    rect[2] + (rect[3] << 16)};
        xrects.push_back(vertices);
    }

    // some magic
    uint32_t min_count = p;
    for (uint32_t i = 0; i < xrects.size(); ++i) {
        if (xrects[i][0] == 0) {
            // call recurrent function
            uint32_t ind = i;
            result_type tree_results = walkthrough(n, m, ind, xrects);
            for (auto & result : tree_results) {
                uint32_t square = result & 0x0000FFFF;
                uint32_t counter = result >> 16;
                if (square == n * m) {
                    min_count = min(min_count, counter);
                }
            }
        }
    }
    cout << min_count << endl;
}
