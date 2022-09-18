#include <iostream>
#include <vector>

using namespace std;

string add_numbers(string A, string B) {
    string C = "";
    int carry = 0;
    int A_dig, B_dig, C_dig;
    for (int i = A.length() - 1, j = B.length() - 1; i >= 0 || j >= 0; --i, --j) {
        A_dig = i >= 0 ? A[i] - '0' : 0;
        B_dig = j >= 0 ? B[j] - '0' : 0;
        if (A_dig < 0 || A_dig > 9 || B_dig < 0 || B_dig > 9) {
            cout << "Input data incorrect, numbers must be decimal, digits in range 0 <= digit <= 9" << endl;
            exit(EXIT_FAILURE);
        }
        C_dig = A_dig + B_dig + carry;
        carry = 0;
        if (C_dig > 9) {
            C_dig -= 10;
            carry = 1;
        }
        C.insert(C.begin(), '0' + C_dig);
    }
    if (carry == 1) {
        C.insert(C.begin(), '1');
    }
    return C;
}

int main() {
    int T;
    cin >> T;
    if (T < 1 || T > 20) {
        cout << "The number of test cases must be in range 1 <= T <= 20" << endl;
        exit(EXIT_FAILURE);
    }
    string A, B;
    vector <string> A_vec, B_vec;
    A_vec.resize(T);
    B_vec.resize(T);
    for (int i = 0; i < T; ++i) {
        cin >> A >> B;
        A_vec[i] = A;
        B_vec[i] = B;
    }
    for (int i = 0; i < T; ++i) {
        cout << endl << "Case " << i + 1 << ":" << endl;
        cout << A_vec[i] << " + " << B_vec[i] << " = " << add_numbers(A_vec[i], B_vec[i]) << endl;
    }
    return 0;
}
