//
// Created by Tim on 20/05/2018.
//

#ifndef BLOCKHOP_UDPSOCKET_H
#define BLOCKHOP_UDPSOCKET_H

#include <string>
#ifdef _WIN32
	#include <winsock2.h>

	typedef SOCKET socket_type;
	typedef int socket_length_type;
#else
	#include <sys/socket.h>
	#include <netinet/in.h> /* sockaddr_in */
	#include <arpa/inet.h>
	#include <unistd.h> /* close() */
	#include <errno.h> /* errno */
    #include <fcntl.h>

	typedef int socket_type;
	typedef socklen_t socket_length_type;

	#define INVALID_SOCKET	(socket_type)(~0)
	#define SOCKET_ERROR	(-1)
#endif

#define DEFAULT_BUFLEN 512

// TODO: Implement NAT Punchthrough

class UDPSocket
{
public:

	// TODO: Implement loose reliability
	// TODO: Change packet payload to more abstract data + size paradigm

	socket_type s{};
	sockaddr_in serverInfo{}, otherInfo{}, natInfo{};
	socket_length_type slen = 0;

	int createSocket();
	int bindServer(unsigned short port);
	void connect(const char *ip, unsigned short port);
	std::string receive();
	void send(sockaddr_in sendToInfo, std::string message);
};

#endif //BLOCKHOP_UDPSOCKET_H
