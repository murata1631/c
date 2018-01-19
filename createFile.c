#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(void){
	FILE *fp; //FILE型構造体
	char fname[] = "../SendFile/RandomString.bin";
	char c;
	char str;
	int i;
	
	setvbuf(stdout, 0, _IONBF, 0); //アンバッファモード
	
	printf("a binary = %b%c\n", 'a');

	fp = fopen(fname, "r");
	if(fp == NULL) {
		printf("[%s] file not found!\n", fname);
		printf("Create New File? (y / n)\n");
		//fflush(0);
		scanf("%c%*c", &c);
		if(c == 'y') {
			fp = fopen(fname, "w");
			printf("[%s] file create!\n", fname);
		} else {
			fclose(fp);
			printf("EOF\n");
			return -1;
		}
	} else {
		printf("[%s] file found out!\n", fname);
		fclose(fp);
		printf("overwrite ? (y / n)\n");
		//fflush(0);
		scanf("%c", &c);
		if(c == 'y') {
			fp = fopen(fname, "w");
			if(fp == NULL) return -1;
			printf("mode: overwite\n");
		} else {
			printf("EOF\n");
			return -1;

		}
	}
	
	srand((unsigned)time(NULL));
	for(i = 0;i < 4000;i++) {
		str=rand()%94+33;
		fwrite(&str, sizeof(char), 1, fp);
	}

	//printf("write to file:\n");
	//scanf("%s", str);
	//printf("the following string are wrote to file : %s\n", &str);
	printf("written to [%s]", fname);
	fclose(fp);

	return 0;
}


