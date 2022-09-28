#include <iostream>
#include <vector>
#include <array>
#include <set>

using namespace std;

class Vertex {
    public:
        int x;
        int y;
        set <uint32_t> score;
        bool processed = false;
};

class xRectangle {
    public:
        int x1;
        int y1;
        int x2;
        int y2;
};

/*
vector <set <int>> merge_scores(int & n, int & m, int & p, vector <set <int>> & s1, vector <set <int>> & s2) {
    // merge score1 to score2
    vector <set <int>> s0;

    for (int i = 0; i < p + 1; ++i) {
        //if (s1[i-1] >= s1[i]) continue;
        //s0[i] = max(s0[i], s1[i]);
        for (int j = 0; j < p + 1; ++j) {
            //if (s2[j-1] >= s2[j]) continue;
            //s0[j] = max(s0[j], s2[j]);
            if (s1[i] + s2[j] <= n * m && i + j < p + 1) {
                s0[i + j] = max(s0[i + j], s1[i] + s2[j]);
            }
        }
    }
    for (int i = p; i > 0; --i) {
        if (s0[i-1] >= s0[i]) {
            s0[i] = 0;
        }
    }
    return s0;
}

bool is_child(xRectangle & xrect1, xRectangle & xrect2) {
    return xrect2.ll == xrect1.ul || xrect2.ll == xrect1.lr;
}
*/
int walkthrough(int & n, int & m, int & p, vector <xRectangle> & xrects) {
    vector <Vertex> vertices;
    for (auto & xrect : xrects) {
        int square = (xrect.x2-xrect.x1)*(xrect.y2-xrect.y1);
        Vertex ll, ul, lr, ur;
            ll.x = xrect.x1;
            ll.y = xrect.y1;
            ll.score.insert(0);
            ul.x = xrect.x1;
            ul.y = xrect.y2;
            ul.score.insert((1<<16)+square);
            lr.x = xrect.x2;
            lr.y = xrect.y1;
            lr.score.insert((1<<16)+square);
            ur.x = xrect.x2;
            ur.y = xrect.y2;
            ur.score.insert((1<<16)+square);
        vertices.push_back(ll);
        vertices.push_back(ul);
        vertices.push_back(lr);
        vertices.push_back(ur);
    }
    for (auto & v1 : vertices) {
        for (auto & v2 : vertices) {
            if () {
            }
        }
    }
/*

    vector <int> result = {0};

    for (auto & xrect : xrects) {
        for (auto & child : xrects) {
            if (is_child(xrect, child)) {
                child.score = merge_scores(n, m, p, xrect.score, child.score);
            }
        }
        result = merge_scores(n, m, p, xrect.score, result);
    }
    {auto y = result; cout << "result <"; for (auto & x : y) cout << x << "'"; cout << ">" << endl;}

    int score = 0;
    while (score < result.size()) {
        ++score;
        if (result[score] == n * m) {
            return score;
        }
    }
*/    return -1;
}

void init_xrect(int x1, int y1, int x2, int y2, vector <xRectangle> & xrects) {
    xRectangle xrect;
    xrect.x1 = x1;
    xrect.y1 = y1;
    xrect.x2 = x2;
    xrect.y2 = y2;
    xrects.push_back(xrect);
}

int main() {
    // map size
    int n = 3, m = 5, p = 4;
    // vector for data
    vector <xRectangle> xrects;
/*
    for (int i = 0; i < n; i += 3){
        for (int j = 0; j < m; j += 3){
            init_xrect(i, j, i + 3, j + 3, xrects, p);
        }
    }
*/
/*
    init_xrect(0, 0, 1, 1, xrects, p);
    init_xrect(0, 1, 1, 5, xrects, p);
    init_xrect(1, 0, 3, 1, xrects, p);
    init_xrect(1, 1, 3, 5, xrects, p);
*/
/*
    n = 3; m = 3; p = 5;
    init_xrect(0, 0, 2, 1, xrects, p);
    init_xrect(0, 1, 1, 3, xrects, p);
    init_xrect(1, 1, 2, 2, xrects, p);
    init_xrect(1, 2, 3, 3, xrects, p);
    init_xrect(2, 0, 3, 2, xrects, p);
*/

    n = 5; m = 5; p = 45;
    init_xrect(0, 0, 1, 1, xrects);
    init_xrect(0, 1, 1, 2, xrects);
    init_xrect(0, 2, 1, 3, xrects);
    init_xrect(0, 3, 1, 4, xrects);
    init_xrect(0, 4, 1, 5, xrects);
    init_xrect(1, 0, 2, 1, xrects);
    init_xrect(1, 1, 2, 2, xrects);
    init_xrect(1, 2, 2, 3, xrects);
    init_xrect(1, 3, 2, 4, xrects);
    init_xrect(1, 4, 2, 5, xrects);
    init_xrect(2, 0, 3, 1, xrects);
    init_xrect(2, 1, 3, 2, xrects);
    init_xrect(2, 2, 3, 3, xrects);
    init_xrect(2, 3, 3, 4, xrects);
    init_xrect(2, 4, 3, 5, xrects);
    init_xrect(3, 0, 4, 1, xrects);
    init_xrect(3, 1, 4, 2, xrects);
    init_xrect(3, 2, 4, 3, xrects);
    init_xrect(3, 3, 4, 4, xrects);
    init_xrect(3, 4, 4, 5, xrects);
    init_xrect(4, 0, 5, 1, xrects);
    init_xrect(4, 1, 5, 2, xrects);
    init_xrect(4, 2, 5, 3, xrects);
    init_xrect(4, 3, 5, 4, xrects);
    init_xrect(4, 4, 5, 5, xrects);
    init_xrect(0, 0, 2, 1, xrects);
    init_xrect(0, 1, 2, 2, xrects);
    init_xrect(0, 2, 2, 3, xrects);
    init_xrect(0, 3, 2, 4, xrects);
    init_xrect(0, 4, 2, 5, xrects);
    init_xrect(1, 0, 3, 1, xrects);
    init_xrect(1, 1, 3, 2, xrects);
    init_xrect(1, 2, 3, 3, xrects);
    init_xrect(1, 3, 3, 4, xrects);
    init_xrect(1, 4, 3, 5, xrects);
    init_xrect(2, 0, 4, 1, xrects);
    init_xrect(2, 1, 4, 2, xrects);
    init_xrect(2, 2, 4, 3, xrects);
    init_xrect(2, 3, 4, 4, xrects);
    init_xrect(2, 4, 4, 5, xrects);
    init_xrect(3, 0, 5, 1, xrects);
    init_xrect(3, 1, 5, 2, xrects);
    init_xrect(3, 2, 5, 3, xrects);
    init_xrect(3, 3, 5, 4, xrects);
    init_xrect(3, 4, 5, 5, xrects);

    cout << walkthrough(n, m, p, xrects) << endl;
}
