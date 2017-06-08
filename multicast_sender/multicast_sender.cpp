#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define TTL 64
#define BUF_SIZE 1316

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSendSock;
	SOCKADDR_IN mulAddr;
	int timeLive = TTL;
	FILE* fp;
	char buf[BUF_SIZE] = { 0, };

	if (argc != 3)
	{
		printf("Usage : %s <GroupIP> <PORT> \n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	hSendSock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&mulAddr, 0, sizeof(mulAddr));
	mulAddr.sin_family = AF_INET;
	mulAddr.sin_addr.s_addr = inet_addr(argv[1]);
	mulAddr.sin_port = htons(atoi(argv[2]));

	setsockopt(hSendSock, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&timeLive, sizeof(timeLive));
	//if ((fp = fopen("resource\\news.txt", "r")) == NULL)
	
	//aaa_cnn_1920_1080i_48000_AAC_ADTS
	//aaa_kbs_1920_1080i_48000_AAC_ADTS	
	//aaa_jtbc_1920_1080i_48000_AC3
	//00_cgv
	//00_cgv_2
	//00_kbs2
	//00_mnet
	//00_tvn
	//amadeus
	//0_kbs_drama
	//0_test_video
	if ((fp = fopen("resource\\0_test_video.ts", "rb")) == NULL)	
		ErrorHandling("fopen() error");
	//char single[1];
	bool flag = false;
	int count = 0;
	while (!feof(fp))
	{
		//single[0] = fgetc(fp);
		//sendto(hSendSock, single, 1, 0, (SOCKADDR*)&mulAddr, sizeof(mulAddr));
		//fgets(buf, BUF_SIZE, fp);
		fread(buf, 1, 1316, fp);
		sendto(hSendSock, buf, BUF_SIZE, 0, (SOCKADDR*)&mulAddr, sizeof(mulAddr));
		//sendto(hSendSock, buf, strlen(buf), 0, (SOCKADDR*)&mulAddr, sizeof(mulAddr));

		count++;
		count %= 20;
		if (count == 0)
		{
			Sleep(1);
		}
			
		//Sleep(1);
	}
	closesocket(hSendSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}