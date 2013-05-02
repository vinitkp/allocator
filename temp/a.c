#include<stdio.h>
#define ALIGNM 8

#define ALIGN(size) ((ALIGNM + size-1) & ~0x7)

#define SIZE (ALIGN(sizeof(size_t)))
int main(){
	printf("%ld",(SIZE));
	return 0;

return 0;
}

