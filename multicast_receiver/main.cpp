#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hRecvSock;
	SOCKADDR_IN addr;
	char buf[BUF_SIZE];
	int strLen;

	if (argc != 3)
	{
		printf("Usage : %s <GroupIP> <PORT> \n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);

	struct ip_mreq joinAddr;
	joinAddr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	joinAddr.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(hRecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&joinAddr, sizeof(joinAddr)) == SOCKET_ERROR)
		ErrorHandling("setsockopt() IP_ADD_MEMBERSHIP error");


	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(argv[2]));
	if (bind(hRecvSock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	BOOL flag_multi_bind = TRUE;
	if (setsockopt(hRecvSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag_multi_bind, sizeof(flag_multi_bind)) == SOCKET_ERROR)
		ErrorHandling("setsockopt() SO_REUSEADDR error");

	while (1)
	{
		strLen = recvfrom(hRecvSock, buf, BUF_SIZE - 1, 0, NULL, 0);
		if (strLen < 0)
			break;
		buf[strLen] = 0;
		fputs(buf, stdout);
	}
	closesocket(hRecvSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}