#include<bits/stdc++.h>


using namespace std;

class base {};
class son :public base {
    int a;
};


int main(int argc, char* argv[]) {
    base x;
    son y;
    cout << sizeof(x) << "\t" << sizeof(y) << endl;
    //int() = 2;
    return 0;
}