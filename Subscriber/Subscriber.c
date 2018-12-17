#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  

#include <WinSock2.h>
#include <ws2tcpip.h>

#include "data.h"      /* client.c - Win32 Konsole Version */
#include "SubscriberSy.h"
#include "topicMgmt.h"
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
//typedef enum{false,true} boolean;



boolean getUserSubscription() {
	char c;
	printf("\n Wollen Sie subscribieren: (j/n)\n");

	//..
}


void processRequest(struct genericMsg *req)
{
	static struct list_type *listHeadPtr = NULL;
	struct list_entry *listEntry = NULL;
	struct MCAnnouncement *mcPtr;
	struct  publishMsg *ptr;

	if (req == NULL)
		return;

	if (!listHeadPtr)
		listHeadPtr = init_list();

	switch ((req->header).ReqType) {
	case PUBLISH:
		printf("GOT A PUBLISH MESSAGE\n");

		ptr = (struct publishMsg *)req;
		printf("in process msg:\n packetId %d \t QoS %d\n", ptr->packetId, ptr->qos);
		if ((ptr->payload).payloadType == 0)  //Integer
			printf("Topic %s \t Value %d\n\n", ptr->topic, (ptr->payload).intValue);
		else
			printf("Topic %s \t Value %f\n\n", ptr->topic, (ptr->payload).floatValue);

		if ((listEntry = find_topic(listHeadPtr, ptr->topic)) == NULL)
			printf("should never happen !\n");

		break;
	case MC_ANNOUNCE:
		printf("GOT AN ANNOUNCEMENT MESSAGE\n");
		
		mcPtr = (struct MCAnnouncement *)req;
		printf("in process msg:\n Topic %s \t MC Address %s \n", mcPtr->topic, mcPtr->mcAddress);
		if ((listEntry = find_topic(listHeadPtr, mcPtr->topic)) != NULL)
			printf("We got an announcement although we already joined!!\n");
		else
		{
			if ((add_topic(listHeadPtr, mcPtr->topic, mcPtr->mcAddress)) != 0)
				printf("Subscriber: Add_Topic failed!\n");
			if (getUserSubscription()) joinMCAddress(mcPtr->mcAddress);
			
		}
		break;

	default:
		fprintf(stderr, "processMsg: Not supported message type\n");
	}
}



int main(int argc, char *argv[])
{
	struct genericMsg req;
   //Programmparameter
   //...
	
	
	if (initClient()!=0) printf("Failed in initClient\n");
	for (;;) {
		getRequest(&req);  
		processRequest(&req);
		Sleep(5000);

	}
}
