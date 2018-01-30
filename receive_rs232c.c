
/* ----------------------------------------
   シリアル通信プログラム（受信専用） 
   ---------------------------------------- */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE h;

int main(void) {
	int i=0;
	char rBuf[65536]; //ReadFile関数で読み取ったデータの格納場所
	char c;
	//char str[50000];
	int baudRate = 115200; //RS232Cビットレート
	FILE *fp; //受信ファイルのポインタ
	unsigned long nn, rSize;
	unsigned long rn = 0; //受信バッファの配列番号を指定
	unsigned int ret;
	int flag = 0; //受信終了の判定に使用
	char fname[256] = "../ReceiveFile/RandomString.txt"; //書き込み先のファイル名
	DCB dcb; //シリアル通信構成情報を記録する構造体の生成
	COMMTIMEOUTS cto;


	setvbuf(stdout, 0, _IONBF, 0); //アンバッファモード

	/* ----------------------------------------------
	   ファイルのクリエイト／オープン
	   }
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
	printf("Open Error!\n");
	exit(1);
}
printf("file open !\n");
/* ----------------------------------------------
   シリアルポートの状態操作
   ---------------------------------------------- */
GetCommState( h, &dcb ); // シリアルポートの状態を取得
dcb.BaudRate = baudRate;
dcb.DCBlength = sizeof(DCB);//DCBのサイズ
dcb.ByteSize = 8;//データサイズ:8bit
dcb.fBinary = TRUE;//バイナリモード:通常TRUE
dcb.fParity = NOPARITY;//パリティビット:パリティビットなし
dcb.StopBits = ONESTOPBIT;//ストップビット:1bit
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

fp = fopen(fname, "rb");
if(fp == NULL) {
	printf("[%s] file not found!\n", fname);
	printf("Create New File? (y / n)\n");
	//fflush(0);
	scanf("%c%*c", &c);
	if(c == 'y') {
		fp = fopen(fname, "wb");
		printf("[%s] file create!\n", fname);
	} else {
		fclose(fp);
		printf("QUIT\n");
		return -1;
	}
} else {
	printf("find out [%s] file !\n", fname);
	fclose(fp);
	printf("overwrite ? (y / n)\n");
	//fflush(0);
	scanf("%c", &c);
	if(c == 'y') {
		fp = fopen(fname, "wb");
		if(fp == NULL) return -1;
		printf("mode: overwite\n");
	} else {
		printf("QUIT\n");
		return -1;

	}
}

/* ----------------------------------------------
   受信データの読み込み
   ---------------------------------------------- */
rSize = sizeof(rBuf);
while(1) {
	//rSize = GetFileSize(h,NULL);
	ret = ReadFile( h,	//ファイルのハンドル
			rBuf + rn,	//データバッファ
			1, //rSize - rn, //GetFileSize(h, NULL),	//読み取り対象のバイト数
			&nn,	//読み取ったバイト数
			NULL	//オーバーラップ構造体のバッファ
		      );	// シリアルポートに対する読み込み

	//printf("%c", rBuf[rn]);
	rn += nn;  //受信データバッファの配列番号を調整

	//printf("GetFileSize = %d\n", rSize);
	if (ret == FALSE) {
		printf("ReadFile failed !");
		break;
	}
	/*if(rSize != nn) {
	  printf("rSize(%d) != nn(%d)\n", rSize, nn); //デバッグ用
	  printf("rn = %d\n", rn);
	  }*/

	if(nn != 0) {
		flag = 1;
	}
	if(flag == 1 && nn == 0) { //受信終了
		//printf("rBuf = %s", rBuf);i
		printf("rn = %d", rn);

		fprintf(fp, "%s",rBuf);
		break;
	}


	//1bitずつ受信するときに使うかも？
	//if ( nn==1 ) {
	//printf("nn ==1\n");
	//if ( sBuf[0]==10 || sBuf[0]==13 ) { // '\r'や'\n'を受信すると文字列を閉じる
	//printf("find out return key\n");
	//if ( i!=0 ) {
	//str[i] = '\0';
	//i=0;
	//printf("%s\n",str);
	//}
	//} else { 
	//	str[i] = sBuf[0];
	//	i++;
	//}
	//} else {
	//	printf(" ! nn = %lu\n");
	//}
}

fclose(fp);
return 0;
}
