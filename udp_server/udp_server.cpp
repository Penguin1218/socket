#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#define BUF_SIZE 1316
void ErrorHandling(char* message0);

int main(int argc, char * argv[])
{
	WSADATA wsaData;
	SOCKET servSock;
	char message[BUF_SIZE];
	int strLen;
	int clntAddrSize;

	SOCKADDR_IN servAddr, clntAddr;
	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	servSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("UDP Socket createion error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

#if 1	//Test Code
	FILE* fp;
	char buf[BUF_SIZE] = { 0, };
	Sleep(2000);
	if ((fp = fopen("resource\\0c.264", "rb")) == NULL)	
		ErrorHandling("fopen() error");


	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	memset(&clntAddr, 0, sizeof(clntAddr));
	clntAddr.sin_family = AF_INET;
	clntAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clntAddr.sin_port = htons(3400);

	while (!feof(fp))
	{
		fread(buf, 1, 1316, fp);
		
		sendto(servSock, buf, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));
		Sleep(1);
	}
#endif
#if 0	//Original sample code
	while (1)
	{
		clntAddrSize = sizeof(clntAddr);
		strLen = recvfrom(servSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &clntAddrSize);
		sendto(servSock, message, strLen, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));
	}
#endif
	closesocket(servSock);
	WSACleanup();
	return 0;
}
void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}