
#include<bits/stdc++.h>
using namespace std;

template<class T>
void swap(T a, T b) {
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}