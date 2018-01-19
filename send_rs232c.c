/* ----------------------------------------
   シリアル通信プログラム（送信専用） 
   ---------------------------------------- */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HANDLE h;

int main(void) {
	int i=0;
	char sBuf[65536]; //SendFile関数で送信するデータのバッファ
	//char str[50000];
	int baudRate = 9600; //RS232Cビットレート
	FILE *fp; //送信ファイルのポインタ
	unsigned long nn, sSize;
	unsigned int ret;
	char fname[256] = "../SendFile/RandomString.bin"; //送信ファイル名
	DCB dcb;
	COMMTIMEOUTS cto;


	setvbuf(stdout, 0, _IONBF, 0); //アンバッファモード

	/* ----------------------------------------------
	   ファイルのクリエイト／オープン
	   ---------------------------------------------- */
	// クリエイトしたファイルのファイルハンドルを返す
	h = CreateFile("COM4", 
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL ); 
	if ( h == INVALID_HANDLE_VALUE ) {
		printf("Open Error !\n");
		exit(1);
	}
	printf("file open !\n");
	/* ----------------------------------------------
	   シリアルポートの状態s操作
	   ---------------------------------------------- */
	GetCommState( h, &dcb ); // シリアルポートの状態を取得
	dcb.BaudRate = baudRate;
	SetCommState( h, &dcb ); // シリアルポートの状態を設定

	printf("sirialport states setup !\n");
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

	printf("sirial time-out setup !\n");

	/* ----------------------------------------------
	   送信ファイル
	   ---------------------------------------------- */
	
	fp = fopen(fname, "rb");
	if(fp == NULL) {
		printf("[%s] file not found!\n", fname);
		//printf("Create New File? (y / n)\n");
		//fflush(0);
		printf("EOF");
		return -1;
	} else {
		printf("[%s] file discovered!\n", fname);
	}
	
	/* ----------------------------------------------
	   受信データの読み込み（１行分の文字列）
	   ---------------------------------------------- */
	
	fgets(sBuf, sizeof(sBuf), fp);
	sSize = strlen(sBuf);
	
	
	ret = WriteFile( h,		//ファイルのハンドル
		   	sBuf,	//送信データのバッファ
		   	sSize,	//送信データのバイト数？
		   	&nn,		//送信データのバイト数
		   	NULL
		 	);		//シリアルポートへ出力

	if(ret == FALSE) {
		printf("WriteFile failed !");
		//break;
	}
	if (sSize != nn) {
		printf("sSize(%d) != nn(%d)", sSize, nn);
	}

	/*
	while(1) {
		ReadFile( h,	//ファイルのハンドル
			sBuf,	//データバッファ
			1,	//読み取り対象のバイト数
			&nn,	//読み取ったバイト数
			NULL	//オーバーラップ構造体のバッファ
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
	*/

	fclose(fp);
	return 0;
}
