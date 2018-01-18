
/* ----------------------------------------
   シリアル通信プログラム（受信専用） 
   ---------------------------------------- */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE h;

void main() {
	int i=0;
	char sBuf[1]; //ReadFile関数で読み取ったデータの格納場所
	char str[50000];
	int baudRate = 9600; //RS232Cビットレート
	unsigned long nn;
	char fname[256] = "../ReceiveFile/RandomString.txt"; //書き込み先のファイル名
	DCB dcb;
	COMMTIMEOUTS cto;


	setvbuf(stdout, 0, _IONBF, 0); //アンバッファモード

	/* ----------------------------------------------
	   ファイルのクリエイト／オープン
	   ---------------------------------------------- */
	// クリエイトしたファイルのファイルハンドルを返す
	h = CreateFile("COM3", 
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL ); 
	if ( h == INVALID_HANDLE_VALUE ) {
		printf("Open Error!\n");
		exit(1);
	}
	printf("file open !\n");
	/* ----------------------------------------------
	   シリアルポートの状態操作
	   ---------------------------------------------- */
	GetCommState( h, &dcb ); // シリアルポートの状態を取得
	dcb.BaudRate = baudRate;
	SetCommState( h, &dcb ); // シリアルポートの状態を設定

	printf("check sirial states !\n");
	/* ----------------------------------------------
	   シリアルポートのタイムアウト状態操作
	   ---------------------------------------------- */
	GetCommTimeouts( h, &cto ); // タイムアウトの設定状態を取得
	cto.ReadIntervalTimeout = 1000;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 1000;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts( h, &cto ); // タイムアウトの状態を設定

	printf("check sirial time-out !\n");

	/* ----------------------------------------------
	   書き込み先のファイル
	   ---------------------------------------------- */
	/*
	fp = fopen(fname, "w");
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
	*/
	/* ----------------------------------------------
	   受信データの読み込み（１行分の文字列）
	   ---------------------------------------------- */
	while(1) {
		ReadFile( h,	//ファイルのハンドル
			sBuf,	//データバッファ
			1,	//読み取り対象のバイト数
			&nn,	//読み取ったバイト数
			0	//オーバーラップ構造体のバッファ
			);	// シリアルポートに対する読み込み

		//	printf("readfile OK\n");
		if ( nn==1 ) {
			//printf("nn ==1\n");
			//if ( sBuf[0]==10 || sBuf[0]==13 ) { // '\r'や'\n'を受信すると文字列を閉じる
				//printf("find out return key\n");
				//if ( i!=0 ) {
					//str[i] = '\0';
					//i=0;
					//printf("%s\n",str);
					fprintf(fp, "%c",sBuf[0]);
				//}
			//} else { 
			//	str[i] = sBuf[0];
			//	i++;
			//}
		} else {
			printf(" ! nn = %lu\n");
		}
	}

	fclose(fp);
	return 0;
}
