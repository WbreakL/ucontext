#include<stdio.h>
int main() {
    int a;
    scanf("%d", &a);
    int res = a & 1;
    if (!res)
        printf("true");
    else
        printf("false");
    return 0;
}