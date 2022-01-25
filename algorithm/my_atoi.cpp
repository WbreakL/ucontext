#include<bits/stdc++.h>


using namespace std;



int my_atoi(const char* pstr)
{
    int Ret_Integer = 0;
    int Integer_sign = 1;

    /*
    * 判断指针是否为空
    */
    if (pstr == NULL)
    {
        printf("Pointer is NULL\n");
        return 0;
    }

    /*
    * 跳过前面的空格字符
    */
    while (*pstr == ' ')
    {
        pstr++;
    }

    /*
    * 判断正负号
    * 如果是正号，指针指向下一个字符
    * 如果是符号，把符号标记为Integer_sign置-1，然后再把指针指向下一个字符
    */
    if (*pstr == '-')
    {
        Integer_sign = -1;
    }
    if (*pstr == '-' || *pstr == '+')
    {
        pstr++;
    }

    /*
    * 把数字字符串逐个转换成整数，并把最后转换好的整数赋给Ret_Integer
    */
    while (*pstr >= '0' && *pstr <= '9')
    {
        Ret_Integer = Ret_Integer * 10 + *pstr - '0';
        pstr++;
    }
    Ret_Integer = Integer_sign * Ret_Integer;

    return Ret_Integer;
}


int main(int argc, char* argv[]) {

    const char* str = "1234";
    int a = my_atoi(str);
    cout << a << endl;
    return 0;
}