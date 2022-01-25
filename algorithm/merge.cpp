#include<bits/stdc++.h>


using namespace std;


void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    int pos = m-- + n-- - 1;
    while (m >= 0 && n >= 0) {
        nums1[pos--] = nums1[m] > nums2[n] ? nums1[m--] : nums2[n--];//没有用到不会被求值
    }
    while (n >= 0) {
        nums1[pos--] = nums2[n--];
    }
}


int main(int argc, char* argv[]) {

    return 0;
}