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
	//char str[65536];
	int baudRate = 115200; //RS232Cビットレート
	FILE *fp; //送信ファイルのポインタ
	unsigned long nn, sSize;
	unsigned int ret;
	char fname[256] = "../SendFile/RandomString.txt"; //送信ファイル名
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
		printf("Open Error !\n");
		exit(1);
	}
	printf("file open !\n");
	/* ----------------------------------------------
	   シリアルポートの状態s操作
	   ---------------------------------------------- */
	GetCommState( h, &dcb ); // シリアルポートの状態を取得	
	dcb.BaudRate = baudRate;
	dcb.DCBlength = sizeof(DCB);//DCBのサイズ
	dcb.ByteSize = 8;//データサイズ:8bit
	dcb.fBinary = TRUE;//バイナリモード:通常TRUE
	dcb.fParity = NOPARITY;//パリティビット:パリティビットなし
	dcb.StopBits = TWOSTOPBITS;//ストップビット:1bitdRate = baudRate;
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
		printf("QUIT");
		return -1;
	} else {
		printf("[%s] file discovered!\n", fname);
	}
	
	/* ----------------------------------------------
	   受信データの読み込み（１行分の文字列）
	   ---------------------------------------------- */
	
	fgets(sBuf, sizeof(sBuf), fp);
	sSize = strlen(sBuf);
	
	for(i=0;i<sSize;i++) {
	ret = WriteFile( h,		//ファイルのハンドル
		   	sBuf+i,	//送信データのバッファ
		   	1, //sSize,	//送信データのバイト数？
		   	&nn,		//送信データのバイト数
		   	NULL
		 	);		//シリアルポートへ出力
	//printf("%c", sBuf[i]);
	if(ret == FALSE) {
		printf("WriteFile failed !");
		//break;
	}
	}

	//if (sSize != nn) {
		printf("\nsSize(%d) = ", sSize);
	//}
	//printf("sBuf =\n%s\n", sBuf);

	fclose(fp);
	return 0;
}
