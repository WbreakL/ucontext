#include<bits/stdc++.h>


using namespace std;


void reverse(string& s) {
    if (s == " ")
        return;
    int len = s.size();
    for (int i = 0;i < len;i++) {
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] -= 32;
        else
            s[i] += 32;
    }
}
string trans(string s, int n) {
    // write code here
    string tmp = "";
    stack<string> st;
    for (int i = 0;i < n;i++) {
        if (s[i] != ' ')
            tmp += s[i];
        else {
            st.push(tmp);
            st.push(" ");
            tmp = "";
        }
    }
    st.push(tmp);
    cout << st.size() << endl;
    // while (st.size()) {
    //     cout << st.top() << endl;
    //     st.pop();

    // }
    string ans = "";
    while (st.size()) {
        reverse(st.top());
        ans += st.top();
        st.pop();
    }
    return ans;
}



int main(int argc, char* argv[]) {

    string res = "this is a simple";
    int len = res.size();
    cout << trans(res, len) << endl;
    return 0;
}