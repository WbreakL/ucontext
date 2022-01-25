#include <stdio.h>

void check_endian();

int main( void )
{
    check_endian();
    return 0;
}
void check_endian()
{
    union{
        char c; // 访问第一个字节
        int data;
    }un;
    un.data = 0x01020304;
    if(un.c == 0x01 )
    {
        printf( "Big endian!\n" );
    }
    else if( un.c == 0x04 )
    {
        printf( "Little endian!%x\n",*(&(un.c)+1));
    }
    else
    {
        printf("No idea!\n");
    }
}
