#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int max_sum(int M, deque <int> X1_deq) {
    sort(X1_deq.begin(), X1_deq.end());
    int Sum = 0;
    while (X1_deq.size() != 0 && X1_deq.back() > M) X1_deq.pop_back();
    if (X1_deq.size() == 0) {
        return 0;
    }
    for (auto num1 : X1_deq) {
        while (X1_deq.size() != 0 && X1_deq.back() > M - num1) X1_deq.pop_back();
        if (X1_deq.size() == 0) {
            Sum = max(num1, Sum);
            continue;
        }
        for (auto num2 : X1_deq) {
            while (X1_deq.size() != 0 && X1_deq.back() > M - num1 - num2) X1_deq.pop_back();
            if (X1_deq.size() == 0) {
                Sum = max(num1 + num2, Sum);
                continue;
            }
            for (auto num3 : X1_deq) {
                while (X1_deq.size() != 0 && X1_deq.back() > M - num1 - num2 - num3) X1_deq.pop_back();
                if (X1_deq.size() == 0) {
                    Sum = max(num1 + num2 + num3, Sum);
                    continue;
                }
                for (auto num4 : X1_deq) {
                    Sum = max(num1 + num2 + num3 + num4, Sum);
                    continue;
                }
            }
        }
    }
    return Sum;
}

int main() {
    int N, M;
    int T = 0;
    vector <deque <int>> X_vec;
    vector <int> M_vec;
    int X;
    while (true) {
        cin >> N >> M;
        if (N == 0 && M == 0) {
            break;
        }
        if (N <= 0 || N > 1000) {
            cout << "The number of integers must be in range 0 < N <= 1000" << endl;
            exit(EXIT_FAILURE);
        }
        if (M <= 0 || M > 1000000000) {
            cout << "Upper bound must be in range 0 < M <= 1000000000" << endl;
            exit(EXIT_FAILURE);
        }
        M_vec.push_back(M);
        ++T;
        X_vec.resize(T);
        for (int i = 0; i < N; ++i) {
            cin >> X;
            if (X <= 0 || X > 1000000000) {
                cout << "All numbers must be in range 0 < X <= 1000000000" << endl;
                exit(EXIT_FAILURE);
            }
            X_vec[T-1].push_back(X);
        }
    }
    for (int i = 0; i < T; ++i) {
        cout << endl;
        cout << "Case " << i + 1 << ": " << max_sum(M_vec[i], X_vec[i]) << endl;
    }
    return 0;
}
