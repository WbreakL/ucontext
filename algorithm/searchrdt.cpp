#include<bits/stdc++.h>

using namespace std;

int main() {
    int n;
    cin >> n;
    //cout << n << endl;
    vector<int> vc;
    vector<int> vu;
    vector<int> vd;
    vector<int> dp;
    for (int i = 0;i < n;i++) {
        int tmpc;
        cin >> tmpc;
        vc.push_back(tmpc);
    }

    for (int i = 0;i < n;i++) {
        int tmpu, tmpd;
        cin >> tmpu >> tmpd;
        vu.push_back(tmpu);
        vd.push_back(tmpd);
    }

    for (int i = 0;i < n;i++) {
        int upsum = 0, downsum = 0;
        for (int u = 0;u < i;u++) {
            upsum += vc[u] * (i - u) * vu[u];
        }
        for (int d = i + 1;d < n;d++) {
            downsum += vc[d] * (d - i) * vd[d];
        }
        dp.push_back(upsum + downsum);
    }
    sort(dp.begin(), dp.end());

    cout << dp[0] << endl;

    return 0;
}