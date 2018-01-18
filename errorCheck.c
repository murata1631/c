#include <stdio.h>
#include <string.h>

void to_binString(char *ch, char *output)
{
	unsigned int val;
	int i;

	val = (int)*ch;

	if( !val ) {
        //return -1;
	}else {
		for(i = 0; i < 8; i++) {
    			if( (val % 2) == 0 ) {  // val は偶数か？
				//printf("gu:0 ");
				output[7-i] = 0x30;    
				 //sprintf(output, "1%s", output);  //  偶数の場合
			} else {
				//printf("ki: 1 ");
				output[7-i] = 0x31;
				//sprintf(output, "1%s", output);  //  奇数の場合
			}
			val = val >> 1;
		}
		//for(i=0;i<10;i++){
		//	printf("\noutput[%d] = %c\n", i, output[i]); 
		//}
	}
	//return 0;
}

int diff_count(char *sStr, char *rStr)
{ 
	int i=0;
	int count=0;
	while(sStr[i]!='\0' && rStr[i]!='\0') {
		if(sStr[i] != rStr[i]){
			count++;
		}
		i++;
	}
	
	return count;
  }

int main(void)
{
	FILE *sfp, *rfp; //送受信ファイルのポインタ
	char sfname[256] = "../SendFile/RandomString.txt"; //送信ファイル名
	char rfname[256] = "../ReceiveFile/RandomString.txt"; //受信ファイル名
	char sch, rch; //読みだした文字の格納
	char sbin[9], rbin[9]; //2進数変換した文字の格納場所
	int diffcnt = 0; //誤りビット数のカウント用
	char c;
	int flag=0;
	int i;

	setvbuf(stdout, 0, _IONBF, 0); //アンバッファモード
	
	i = 0;
	while(1) {
		if(i == 0) {
			printf("FileName = [%s] : (y / n)", rfname);
			scanf("%s%*c", &c);
			if(c == 'y') break; break;
		}
		printf("input ReceiveFileName :");
		scanf("%s%*c", rfname + 15);
		printf("FileName = [%s] : (y / n)", rfname);
		scanf("%s%*c", &c);
		if(c == 'y') break;
	}
	putchar('\n');

	sfp = fopen(sfname, "r");
	rfp = fopen(rfname, "r");
	

	if(sfp == NULL) {
		printf("[%s] not found. \n", sfname); 
		flag += 1;
	}
	if(rfp == NULL) {
		printf("[%s] not found. \n", rfname); 
		flag += 2;
	}
	switch(flag) {
		case 3: printf("EOF case3\n");
			return -1;

		case 2: fclose(sfp);
			printf("EOR case2\n");
			return -1;

		case 1: fclose(rfp);
			printf("EOF case1\n");
			return -1;

		default: break;
	}

	i = 0;
	while(1) {
		//printf("number %d\n", i);

		sch = fgetc(sfp);
		if(sch == EOF) break; //EOF を検出したらbreakする
		//printf("fgetc(sfp) = %c\n", sch);
		rch = fgetc(rfp);
		//printf("fgetc(rfp) = %c\n", rch);
		
		to_binString(&sch, sbin);
		to_binString(&rch, rbin);
		//printf("to_binstring(sch) = %s\n", sbin);
		//printf("to_binstring(rch) = %s\n", rbin);

		diffcnt += diff_count(sbin, rbin);
		//printf("diff_count = %d\n\n", diffcnt);

		i++;
	}

	fclose(sfp);
	fclose(rfp);

	printf("Differendes between [%s] and [%s]\n:%d\n", sfname+3, rfname+3, diffcnt);
	return 0;
}

