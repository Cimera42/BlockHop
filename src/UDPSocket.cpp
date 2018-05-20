//
// Created by Tim on 20/05/2018.
//

#include "UDPSocket.h"
#include "logger.h"

void errorMessage(const std::string &callerName)
{
#ifdef _WIN32
	Logger() << callerName << " failed with error code : " << WSAGetLastError();
#else
	Logger() << callerName << " failed with error code : " << errno;
#endif
}

int UDPSocket::createSocket()
{
	//Create a socket
	if((s = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP)) == INVALID_SOCKET)
	{
		errorMessage("socket()");
		return 1;
	}
	printf("Socket created.\n");

	return 0;
}

int UDPSocket::bindServer(unsigned short port)
{
	//Prepare the sockaddr_in structure
	memset((char *) &serverInfo, 0, sizeof(serverInfo));
	serverInfo.sin_family = AF_INET;
#ifdef _WIN32
	serverInfo.sin_addr.S_un.S_addr = INADDR_ANY;
#else
	serverInfo.sin_addr.s_addr = INADDR_ANY;
#endif
	serverInfo.sin_port = htons( port );

	//Bind
	if( bind(s ,(sockaddr *) &serverInfo , sizeof(serverInfo)) == SOCKET_ERROR)
	{
		errorMessage("bind()");
		return 1;
	}
	Logger() << "Bind done on " << port;

	return 0;
}

void UDPSocket::connect(const char *ip, unsigned short port)
{
	//setup address structure
	memset((char *) &otherInfo, 0, sizeof(otherInfo));
	otherInfo.sin_family = AF_INET;
	otherInfo.sin_port = htons(port);
#ifdef _WIN32
	otherInfo.sin_addr.S_un.S_addr = inet_addr(ip);
#else
	otherInfo.sin_addr.s_addr = inet_addr(otherIp);
#endif
}

std::string UDPSocket::receive()
{
	char buf[DEFAULT_BUFLEN];
	sockaddr_in receiveInfo{};
	int receiveCode = recvfrom(s, buf, DEFAULT_BUFLEN, 0, (sockaddr *) &receiveInfo, &slen);
	if(receiveCode == SOCKET_ERROR)
	{
		errorMessage("recvfrom()");
		return "";
	}
	else if(receiveCode == 0)
	{
		Logger() << "Socket closed";
		return "";
	}

	return std::string(buf);
}

void UDPSocket::send(sockaddr_in sendToInfo, std::string message)
{
//	std::string otherName;
//	otherName += inet_ntoa(sendToInfo.sin_addr);
//	otherName += ":";
//	otherName += std::to_string(ntohs(sendToInfo.sin_port));
//	Logger() << "Sending " << message << " to " << otherName;
	//send the message
	if(sendto(s, message.c_str(), static_cast<int>(message.length()),
	          0, (sockaddr *) &sendToInfo, slen) == SOCKET_ERROR)
	{
		errorMessage("sendto()");
	}
}