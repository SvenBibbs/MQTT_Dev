
#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <string.h>
#include <WS2tcpip.h>
#include <time.h>
#include "PublisherSy.h"
#include <windows.h>

#define _CRT_SECURE_NO_WARNINGS
typedef struct sockaddr_in6 ipv6;

int main(int argc, char **argv) {
	int type_flag = 0;											// Flag: Int oder Float senden
	//int type_flag = 0;
	SOCKET publisher;							// Socket
	int serverPort = PORT_PUBLISH;								// Serverport
	publish msg;											// Nachricht
	char *serverAddress = NULL;									// Serverip
	char topicName[TOPIC_LENGTH] = "Test";					// Topic


	// Eingabeparameter prüfen und zuweisen
	if (argc > 1)
	{
		serverAddress = argv[1];
		serverPort = atoi(argv[2]);
	}
	else
	{
		printf("Eingabeparameter: <IP-Addr> <PortNum>");
		exit(1);
	}
	

	// Winsock
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		printf("Winsock konnte nicht gestartet werden.\n");
		exit(1);
	}
	printf("Winsock erfolgreich gestarten... CHECK\n");


	// Socket initialisieren
	publisher = init_socket(AF_INET6, TCP);
	printf("Socket initialisieren... CHECK\n");


	// Mit Server verbinden
	conn_client(publisher, serverAddress, serverPort);
	printf("Serververbindung aufbauen... CHECK\n");


	// Integer oder Float auswählen
	get_type(&type_flag);
	get_topic(topicName, TOPIC_LENGTH);
	

	// Paketdaten festsetzen
	msg.qos = 0;
	msg.packetId = 0;
	msg.retainFlag = 0;
	msg.dupFlag = 0;

	if(type_flag == 1)
	{
		msg.payload.payloadType = VAL_FLOAT;
	}
	else
	{
		msg.payload.payloadType = VAL_INT;
	}

	strcpy_s(msg.topic, sizeof(topicName), topicName);

	// Senden der CONNECT Message an Broker
	msg.header.ReqType = 'C';
	sendTcpMsg(publisher, msg);

	// PUBLISH als Typ einpflegen
	msg.header.ReqType = 'P';

	while(TRUE) {
		if (type_flag == 1) // FLOAT
		{
			get_float_val(&msg.payload.floatValue);
		}
		if (type_flag == 0) // INTEGER
		{
			get_int_val(&msg.payload.intValue);
		}

		sendTcpMsg(publisher, msg);
	}
	return EXIT_SUCCESS;
}