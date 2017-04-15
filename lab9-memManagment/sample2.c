#include <stdio.h>
#include <stdlib.h>

int x = 1;

int main()
{
	int a, b, c, d;
	char ch[10];

	int *y = malloc(16);

	printf("%p\n", &a);
	printf("%p\n", &b);
	printf("%p\n", &c);
	printf("%p\n", &d);
	printf("%p\n", &x);
	printf("%p\n", y);

	scanf("%s", ch);
}