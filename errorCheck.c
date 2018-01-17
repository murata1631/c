#include <stdio.h>
#include <string.h>

void to_binString(char *ch, char *output)
{
    unsigned int val;
    char str[8];
    int i;
    //char *p;

    val = (int)*ch;
	//printf("ch = %d\n", *ch);
	//printf("val = %d\n", val);
    //p = &str[0];

    if( !val ) {
        //return -1;
    }else {
    	for(i = 0; i < 8; i++) {
	    //printf("i = %d\n", i);
		//printf("val = %d\n", val); 
    	    	if( (val % 2) == 0 ) {  // val は偶数か？
			printf("gu:0 ");
		    	//str[7-i] = "0";    
		   	 sprintf(str[], "1%s", str);  //  偶数の場合
		} else {
			printf("ki: 1 ");
			//str[7-i] = "1";
			sprintf(str[], "1%s", str);  //  奇数の場合
		}
		val = val >> 1;
	}
	printf("\nstr = %s\n", str); 
	output = str;
    }
    //return 0;
}

int diff_count(char sStr[8], char rStr[8])
{ 
	int i;
	int count;

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
	char *schp, *rchp; //sch,rchのポインタ
	char sbin[8], rbin[8]; //2進数変換した文字の格納場所
	char *sbinp, *rbinp;
	int diffcnt = 0; //誤りビット数のカウント用
	char c;
	int flag=0;
	int i;

	setvbuf(stdout, 0, _IONBF, 0); //アンバッファモード
	
	schp = &sch;
	rchp = &rch;

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
	while(sch != EOF) {
		printf("number %d\n", i);

		sch = fgetc(sfp);
		printf("fgetc(sfp) = %c\n", sch);
		rch = fgetc(rfp);
		printf("fgetc(rfp) = %c\n", rch);

		to_binString(schp, sbinp);
		printf("to_binstring(sch) = %c\n", *sbinp);
		to_binString(rchp, rbinp);
		printf("to_binstring(rch) = %c\n", *rbinp);

		diffcnt += diff_count(sbinp, rbinp);
		printf("diff_count =\n", diffcnt);

		i++;
	}

	fclose(sfp);
	fclose(rfp);

	printf("Differendes between \n [%s] and [%s]\n:%d", sfname, rfname, diffcnt);
	return 0;
}

