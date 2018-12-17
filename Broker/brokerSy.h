#include <WinSock2.h>
#include <errno.h>

int add_client(SOCKET sock, int sockType);
int exitServer();
unsigned int fill_set(fd_set *fds);
SOCKET get_sender(fd_set *fds);
int initServer(char *Server, char *PublisherPort, char *SubscriberPort);
void newMCAddr(char *lastMCAddr);
int remove_client(SOCKET sock);
void sendUDPMsg(struct genericMsg* annMsg, char *mcGroupAddr, char *port);