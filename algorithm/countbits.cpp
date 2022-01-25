#include<bits/stdc++.h>


using namespace std;

int counti(int i) {
    int counts = 0;
    while (i != 0)
    {

        if (i & 1 == 1) {
            counts += 1;
            i >> 1;
        }/* code */
    }
    return counts;

}

vector<int> countbits(int num) {
    vector<int> vc(num + 1, 0);
    // for (int i = 1;i <= num;++i) {
    //     vc[i] = counti(i);
    // }
    for (int i = 1;i <= num;++i) {
        vc[i] = vc[i & (i - 1)] + 1;
    }
    return vc;
}


int main(int argc, char* argv[]) {
    vector<int>::iterator iter;
    vector<int> vc = countbits(5);
    for (iter = vc.begin();iter < countbits(5).end();iter++) {
        cout << *iter << endl;
    }
    return 0;
}