#include <stdio.h>
int main(void)
{

	int i;
	int arr[10];
	int q;
	double w;
	char e;
	int a = 0x12345678;
	printf("%p\n", arr);
	for (i = 0; i < 10; i++) {
		printf("%p\n", &(arr[i]));
	}
	printf("---------------\n");

	for (i = 0; i < 10; i++) {
		printf("%p\n", arr + i);
	}
	printf("---------------\n");
	printf("%p\n", &q);
	printf("%p\n", &w);
	printf("%p\n", &e);
	printf("%x\n", *(char*)&a);
	printf("%x\n", *(((char*)&a) + 1));
	return 0;
}
