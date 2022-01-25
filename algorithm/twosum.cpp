#include<bits/stdc++.h>


using namespace std;

vector<int> twosum(vector<int>& number, int target) {
    int l = 0, r = number.size() - 1, sum;
    while (l < r) {
        sum = number[l] + number[r];
        if (sum == target)
            break;
        if (sum < target)
            ++l;
        else
            --r;
    }
    return vector<int>{l + 1, r + 1};
}
