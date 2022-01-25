#include<bits/stdc++.h>


using namespace std;

void insert_sort_switch(int* arr, int len) {//来回交换
    int i = 1;//无序数组的起始位置
    for (int i = 1;i < len;++i) {
        int j = i - 1;
        while (arr[j + 1] < arr[j] && j >= 0) {
            swap(arr[j], arr[j + 1]);
        }
    }
}

void insert_sort(int* arr, int len) {//来回交换
    int i = 1;//无序数组的起始位置
    for (int i = 1;i < len;++i) {
        int j = i - 1;
        int temp = arr[i];
        while (temp < arr[j] && j >= 0) {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = temp;
    }
}



int main(int argc, char* argv[]) {

    return 0;
}