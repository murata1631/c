#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(void)
{
	unsigned char str[800];
	int i;

	srand((unsigned)time(NULL));
	for(i=0;i<100;i++){
		str[i]=rand()%94+33;
	}
	
	printf("str = %s\n", str);
printf("strlen(str) = %d", strlen(str));

	return 0;
}
