#include<stdio.h>


//using namespace std;




int main(int argc, char* argv[]) {

    FILE* fd = fopen("test.c", "w+");
    for (int i = 0;i < 2000;i++) {

        int n = 0;
        int tmp=i;
        while (tmp != 0) {
            tmp/= 10;
            n++;
        }
        fprintf(fd, "if(i==%d)\n", i);
        fprintf(fd, "printf(\"%d是%d位数\");            \n",i,n);
    }


    return 0;
}