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

class xRectangle {
    public:
        Vertex ll;
        Vertex ul;
        Vertex lr;
        Vertex ur;
        vector <int> score;
};


vector <vector <int>> merge_scores(int & n, int & m, int & p, vector <vector <int>> & s1, vector <vector <int>> & s2) {
    // merge score1 to score2
    vector <vector<int>> s0;

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

int walkthrough(int & n, int & m, int & p, vector <xRectangle> & xrects) {

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
    return -1;
}

void init_xrect(int x1, int y1, int x2, int y2, vector <xRectangle> & xrects, int p) {
    xRectangle xrect;
    xrect.ll.x = x1;
    xrect.ll.y = y1;
    xrect.ul.x = x1;
    xrect.ul.y = y2;
    xrect.lr.x = x2;
    xrect.lr.y = y1;
    xrect.ur.x = x2;
    xrect.ur.y = y2;
    vector <int> score(p + 1, 0);
    xrect.score = score;
    xrect.score[1] = (x2-x1)*(y2-y1);
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
    init_xrect(0, 0, 1, 1, xrects, p);
    init_xrect(0, 1, 1, 2, xrects, p);
    init_xrect(0, 2, 1, 3, xrects, p);
    init_xrect(0, 3, 1, 4, xrects, p);
    init_xrect(0, 4, 1, 5, xrects, p);
    init_xrect(1, 0, 2, 1, xrects, p);
    init_xrect(1, 1, 2, 2, xrects, p);
    init_xrect(1, 2, 2, 3, xrects, p);
    init_xrect(1, 3, 2, 4, xrects, p);
    init_xrect(1, 4, 2, 5, xrects, p);
    init_xrect(2, 0, 3, 1, xrects, p);
    init_xrect(2, 1, 3, 2, xrects, p);
    init_xrect(2, 2, 3, 3, xrects, p);
    init_xrect(2, 3, 3, 4, xrects, p);
    init_xrect(2, 4, 3, 5, xrects, p);
    init_xrect(3, 0, 4, 1, xrects, p);
    init_xrect(3, 1, 4, 2, xrects, p);
    init_xrect(3, 2, 4, 3, xrects, p);
    init_xrect(3, 3, 4, 4, xrects, p);
    init_xrect(3, 4, 4, 5, xrects, p);
    init_xrect(4, 0, 5, 1, xrects, p);
    init_xrect(4, 1, 5, 2, xrects, p);
    init_xrect(4, 2, 5, 3, xrects, p);
    init_xrect(4, 3, 5, 4, xrects, p);
    init_xrect(4, 4, 5, 5, xrects, p);
    init_xrect(0, 0, 2, 1, xrects, p);
    init_xrect(0, 1, 2, 2, xrects, p);
    init_xrect(0, 2, 2, 3, xrects, p);
    init_xrect(0, 3, 2, 4, xrects, p);
    init_xrect(0, 4, 2, 5, xrects, p);
    init_xrect(1, 0, 3, 1, xrects, p);
    init_xrect(1, 1, 3, 2, xrects, p);
    init_xrect(1, 2, 3, 3, xrects, p);
    init_xrect(1, 3, 3, 4, xrects, p);
    init_xrect(1, 4, 3, 5, xrects, p);
    init_xrect(2, 0, 4, 1, xrects, p);
    init_xrect(2, 1, 4, 2, xrects, p);
    init_xrect(2, 2, 4, 3, xrects, p);
    init_xrect(2, 3, 4, 4, xrects, p);
    init_xrect(2, 4, 4, 5, xrects, p);
    init_xrect(3, 0, 5, 1, xrects, p);
    init_xrect(3, 1, 5, 2, xrects, p);
    init_xrect(3, 2, 5, 3, xrects, p);
    init_xrect(3, 3, 5, 4, xrects, p);
    init_xrect(3, 4, 5, 5, xrects, p);

    cout << walkthrough(n, m, p, xrects) << endl;
}
