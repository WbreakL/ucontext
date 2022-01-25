#include<bits/stdc++.h>


using namespace std;
template<class T>
T feibonaqie(T n) {
    T pre2 = 1, pre1 = 2, cur;
    for (T i = 0;i < n;i++) {
        cur = pre1 + pre2;
        pre2 = pre1;
        pre1 = cur;
        cout << cur << endl;
    }
    return cur;
}


vector<int> reOrderArray(vector<int>& array) {
    // write code here
    for (int i = 0;i < array.size();i++) {
        if ((array[i] & 1) == 0) {
            for (int j = i + 1;j < array.size();j++) {
                if (array[j] & 1 == 1) {
                    array[i] ^= array[j];
                    array[j] ^= array[i];
                    array[i] ^= array[j];

                }
            }
        }
    }
    return array;
}
int main(int argc, char* argv[]) {
    //feibonaqie<long int>(100);
    vector<int> a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    reOrderArray(a);
    if ((a[0] & 1) == 1)
        cout << 100 << endl;
    for (int i = 0;i < a.size();i++) {
        cout << a[i] << endl;
    }
    return 0;
}