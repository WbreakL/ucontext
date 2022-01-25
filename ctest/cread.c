#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h> 
int main(void)
{
    int fd = open("temp.txt", O_WRONLY);
    printf("%d \n", fd);
    char buf[100] = "hello,wordfgfdgdfgbdfbdfbdfgbdfgbgfdbdfvvdfgfgv";
    int num = 0;
    while ((num = write(fd, buf + num, sizeof(buf) - num)) != 0)
    {
        printf("读到的字符数为:%d \n", num);
        printf("%s \n", buf);
        memset(buf, 0, sizeof(buf));
    }
    printf("文件读取结束了 \n");
    return 0;
}