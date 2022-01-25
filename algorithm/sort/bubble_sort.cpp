#include<bits/stdc++.h>


using namespace std;

void bubble_sort(int* arr, int len) {
    for (int i = 0;i < len - 1;++i) {
        for (int j = 0;j < len - i - 1;j++) {
            if (arr[j] > arr[j + 1]) {
                arr[j] = arr[j] ^ arr[j + 1];
                arr[j + 1] = arr[j] ^ arr[j + 1];
                arr[j] = arr[j] ^ arr[j + 1];
            }
        }
    }
}


class Solution {
public:

    int StrToInt(string str) {
        int sign = 1, num = 0;
        if (str == "0" || str.empty())
            return 0;
        for (int i = 0;i < str.size();i++) {
            if ((str[i] <= '0' && str[i] >= '9') || i != '-' || i != '+') {
                return 0;
            }
            if (str[i] == '-')
                sign = -1;
            num = num * 10 + i - '0';
        }
        return num;


    }
};
int main(int argc, char* argv[]) {

    vector<vector<int>> a;
    return 0;
}