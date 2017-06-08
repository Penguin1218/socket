#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1316
void ErrorHandling(char* message0);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET sock;
	char message[BUF_SIZE] = { 0, };
	int strLen;

	SOCKADDR_IN servAddr;
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)
		ErrorHandling("WSAStartup Error");

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr));	//connected udp socket, this function doesn't need to be implemented.

#if 1	//Application code
	FILE* fp;
	if ((fp = fopen("resource\\00_cgv.ts", "rb")) == NULL)
		ErrorHandling("fopen() error");
	char buf[BUF_SIZE] = { 0, };

	while (1)
	{
		fread(buf, 1, 1316, fp);
		send(sock, buf, strlen(buf), 0);
		//strLen = recv(sock, message, sizeof(message) - 1, 0);
		message[strLen] = 0;
		//printf("Message from server : %s", message);
	}
#endif
#if 0	//Original Sample code
	while (1)
	{
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		memset(message, '1', 2145);
		
		send(sock, message, strlen(message), 0);
		strLen = recv(sock, message, sizeof(message) - 1, 0);
		message[strLen] = 0;
		printf("Message from server : %s", message);
	}
#endif
	closesocket(sock); 
	WSACleanup();
	return 0;
}
void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}