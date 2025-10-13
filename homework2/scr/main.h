#include <iostream>
using namespace std;

void ps(int S[], int n, int sub[], int sz, int idx) {
    if (idx == n) {
        cout << "{";
        for (int i = 0; i < sz; ++i) {
            cout << sub[i];
            if (i != sz - 1) cout << ",";
        }
        cout << "}" << endl;
        return;
    }
    ps(S, n, sub, sz, idx + 1);
    sub[sz] = S[idx];
    ps(S, n, sub, sz + 1, idx + 1);
}

int main() {
    int S[] = {1, 2, 3};
    int n = 3;
    int sub[3];
    ps(S, n, sub, 0, 0);
    return 0;
}
