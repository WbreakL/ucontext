#include<bits/stdc++.h>


using namespace std;





int main(int argc, char* argv[]) {
    // long long n;
    // cin >> n;

    // long long a[n], b[n], ans[n];
    // ans[0] = 1;
    // for (int i = 1;i < n;i++) {
    //     cin >> a[i++];
    // }
    // for (int i = 1;i < n;i++) {
    //     cin >> b[i++];
    // }
    // for (int i = 1;i < n;i++) {
    //     ans[i] = ans[i - 1] * a[i];
    // }
    // for (int i = 0;i < n;i++) {
    //     cout << ans[i];
    // }

    long long n, m;
    cin >> n >> m;
    long long thing[m][3];
    for (int i = 0;i < m;i++) {
        for (int j = 0;j < 3;j++) {
            cin >> thing[i][j];
        }
    }
    vector<deque<int>> windows;
    windows.resize(n + 1);
    for (int i = 0;i < m;i++) {
        for (int j = 0;i < 3;j++) {
            if (thing[i][0] == 1) {
                windows[thing[i][1]].push_back(thing[i][2]);
            }
            else {
                if (!windows[thing[i][1]].empty()) {
                    int tmp = windows[thing[i][1]].front();
                    windows[thing[i][1]].pop_front();
                    cout << tmp << endl;
                }

                else
                    cout << -1 << endl;
            }
        }

    }


    return 0;
}