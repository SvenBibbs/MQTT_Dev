#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <string.h>
#include <WS2tcpip.h>
#include <time.h>
#include "PublisherSy.h"

#pragma comment (lib, "ws2_32.lib")


// Nutzereingaben
void get_type(int *var) {
	do {
		printf_s("Datentyp: (1)-Float; (0)-Integer > ");
		scanf_s("%d", var);
		while (getchar() != '\n');
	} while (*(var) < 0 || *(var) > 1);
}

void get_int_val(int *value)
{
	do
	{
		printf("Integerwert eingeben (Range: -100 bis +100) > ");
		scanf_s("%i", value);
		while (getchar() != '\n');
	} while (*value < -100 || *value > 100);
}

void get_float_val(float *value)
{
	do
	{
		printf("Floatwert eingeben (Range: -100.000 bis +100.000) > ");
		scanf_s("%f", value);
		while (getchar() != '\n');
	} while (*value < -100 || *value > 100);
}

void get_topic(char *topic_name, unsigned int length)
{
	char *eingabe = NULL;
	eingabe = (char*)malloc(sizeof(char) * length);
	do
	{
		printf("Topic eingeben: ");
		fgets(eingabe, length, stdin);
	} while (strlen(eingabe) > length);
	memset(topic_name, 0, strlen(eingabe));
	strncpy(topic_name, eingabe, strlen(eingabe) - 1);
}



/*
Fehlerausgabe
*/

int err_print(int error)
{
	printf("Failure. Error: %i", error);
	Sleep(5000);
		exit(EXIT_FAILURE);
}


// Socket
SOCKET init_socket(int address_family, SOCKET_TYPE sockettype)
{
	SOCKET sock_con = INVALID_SOCKET;

	if ((sock_con = socket(address_family, sockettype, 0)) <= INVALID_SOCKET)
		err_print(WSAGetLastError());

	return sock_con;
}


// TCP-Nachrichten versenden
int sendTcpMsg(SOCKET sock_con, publish msg) {
	if (send(sock_con, &msg, sizeof(msg), 0) <= SOCKET_ERROR)
		err_print(WSAGetLastError());

	return TRUE;
}


//TCP Verbindnung
int conn_client(SOCKET sock_con, char *address, int port) {
	ipv6 socket_struct;
	PCSTR pszAddrString = address;
	memset(&socket_struct, 0, sizeof(socket_struct));
	socket_struct.sin6_port = htons(port);
	socket_struct.sin6_family = AF_INET6;
	if (inet_pton(AF_INET6, pszAddrString, &(socket_struct.sin6_addr)) == SOCKET_ERROR)
	{
		err_print(WSAGetLastError());
	}
	if (connect(sock_con, (ipv6 *)&socket_struct, sizeof(socket_struct)) == SOCKET_ERROR)
	{
		err_print(WSAGetLastError());
	}
	return TRUE;
}
