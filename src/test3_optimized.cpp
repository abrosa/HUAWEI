#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Vertex {
    public:
        int x;
        int y;

bool operator == (const Vertex & v) {
    return x == v.x && y == v.y;
}

bool operator != (const Vertex & v) {
    return x != v.x || y != v.y;
}
};

class Score {
    public:
        Vertex v;
        vector <int> score;
};

class xRectangle {
    public:
        int x1;
        int y1;
        int x2;
        int y2;
        Vertex ll;
        Vertex ul;
        Vertex lr;
        Vertex ur;
        int square;
        int counter;
        Score score;
        bool processed;

bool operator == (const xRectangle & r) {
    return x1 == r.x1 && y1 == r.y1 && x2 == r.x2 && y2 == r.y2;
}

bool operator != (const xRectangle & r) {
    return x1 != r.x1 || y1 != r.y1 || x2 != r.x2 || y2 != r.y2;
}
};


xRectangle merge_scores(xRectangle & full_map, xRectangle & xrect1, xRectangle & xrect2) {
    // merge xrect1 to xrect2 and return as xrect0
    int n = full_map.x2;
    int m = full_map.y2;
    int p = full_map.counter;
    int square = full_map.square;
    // if xrects are the same, just return xrect2
    if (xrect1 == xrect2) return xrect2;
    // start merging
    vector <int> s0(p + 1, 0);
    vector <int> s1 = xrect1.score.score;
    vector <int> s2 = xrect2.score.score;
    // use shorter variables
    for (int k = 0; k < p + 1; ++k) {
        for (int i = 0; i < p + 1; ++i) {
            s0[i] = max(s0[i], s1[i]);
            for (int j = 0; j < p + 1; ++j) {
                s0[j] = max(s0[j], s2[j]);
                if (i + j == k) {
                    if (s1[i] + s2[j] <= square) {
                        s0[k] = max(s0[k], s1[i] + s2[j]);
                    }
                }
            }
        }
    }
    xRectangle xrect0 = xrect2;
    xrect0.square = xrect1.square + xrect2.square;
    xrect0.score.score = s0;
    return xrect0;
}

bool is_ll(xRectangle & xrect) {
    return xrect.x1 == 0 && xrect.y1 == 0;
}

bool is_ur(xRectangle & full_map, xRectangle & xrect) {
    return xrect.ur == full_map.ur && xrect.ur == full_map.ur;
}

int walkthrough(vector <xRectangle> & xrects) {
    xRectangle full_map = xrects.back();
    xrects.pop_back();
    int n = full_map.x2;
    int m = full_map.y2;
    full_map.counter = xrects.size();
    // collect info about vertices
    vector <Vertex> vertices;
    for (auto & xrect : xrects) {
        vertices.push_back(xrect.ll);
        vertices.push_back(xrect.ul);
        vertices.push_back(xrect.lr);
        vertices.push_back(xrect.ur);
    }
    // check input data
    bool ll = false, ul = false, lr = false, ur = false;
    for (auto & vertex : vertices) {
        if (vertex == full_map.ll) ll = true;
        if (vertex == full_map.ul) ul = true;
        if (vertex == full_map.lr) lr = true;
        if (vertex == full_map.ur) ur = true;
    }
    // all corners must be presented
    if (!(ll && ul && lr && ur)) return -1;
    for (auto & xrect : xrects) {
        for (auto & child : xrects) {
            if (xrect.ll == child.ul || xrect.ll == child.lr ||
                xrect.ul == child.ll || xrect.ul == child.ur ||
                xrect.lr == child.ll || xrect.lr == child.ur ||
                xrect.ur == child.ul || xrect.ur == child.lr) {
                child = merge_scores(full_map, xrect, child);
            }
        }
        {auto y = xrect.score.score; cout << "xrect.score.score <"; for (auto & x : y) cout << x << "'"; cout << ">" << endl;}
    }
    return -1;
}

void init_xrect(int x1, int y1, int x2, int y2, vector <xRectangle> & xrects, int p) {
    xRectangle xrect;
    xrect.x1 = x1;
    xrect.y1 = y1;
    xrect.x2 = x2;
    xrect.y2 = y2;
    xrect.ll.x = x1;
    xrect.ll.y = y1;
    xrect.ul.x = x1;
    xrect.ul.y = y2;
    xrect.lr.x = x2;
    xrect.lr.y = y1;
    xrect.ur.x = x2;
    xrect.ur.y = y2;
    xrect.square = (x2 - x1) * (y2 - y1);
    xrect.counter = p;
    xrect.score.v.x = x1;
    xrect.score.v.y = y1;
    vector <int> score(p + 1, 0);
    xrect.score.score = score;
    xrect.score.score[1] = xrect.square;
    xrect.processed = false;
    xrects.push_back(xrect);
}

int main() {
    // map size
    int n = 3, m = 5, p = 4;
    // vector for data
    vector <xRectangle> xrects;
/*
    for (int i = 0; i < n; i += 1){
        for (int j = 0; j < m; j += 1){
            pack_rect(i, j, i + 1, j + 1, rects);
        }
    }
*/
    init_xrect(0, 0, 1, 1, xrects, p);
    init_xrect(0, 1, 1, 5, xrects, p);
    init_xrect(1, 0, 3, 1, xrects, p);
    init_xrect(1, 1, 3, 5, xrects, p);

    // last element is the full map
    init_xrect(0, 0, n, m, xrects, p);


    // try to resolve
    cout << walkthrough(xrects) << endl;
}
