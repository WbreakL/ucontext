#include<bits/stdc++.h>


using namespace std;

void select_sort(int* arr, int len) {
    for (int i = 0;i < len - 1;++i) {//i<len-1,排到倒数第二个就不用再用内层循环了
        for (int j = i;j < len;++j) {
            if (arr[j] < arr[i]) {
                arr[j] = arr[j] ^ arr[j + 1];
                arr[j + 1] = arr[j] ^ arr[j + 1];
                arr[j] = arr[j] ^ arr[j + 1];
            }
        }
    }
}



int main(int argc, char* argv[]) {

    return 0;
}