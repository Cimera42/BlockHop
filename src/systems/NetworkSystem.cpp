//
// Created by Tim on 19/05/2018.
//
#include "NetworkSystem.h"
#include "../ecs/ecsManager.h"
#include "../logger.h"
#include "../components/TransformComponent.h"
#include "../components/NetworkComponent.h"
#include "../utils.h"

#ifdef _WIN32
	#include <winsock2.h>
	#pragma comment(lib, "Ws2_32.lib")
#endif

SYSTEM_EXPORT(NetworkSystem, "networkSystem")

NetworkSystem::NetworkSystem()
{
#ifdef _WIN32
	WSADATA wsaData{};

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iResult != 0)
	{
		Logger() << "WSAStartup failed: " << iResult;
	}
#endif
}

NetworkSystem::~NetworkSystem()
{
#ifdef _WIN32
	shutdown(socket.s, SD_BOTH);
#else
	shutdown(socket.s, SHUT_RDWR);
#endif

	//todo EXIT and JOIN receive thread
	exitReceive = true;
	if(receiveThread.joinable())
		receiveThread.join();

#ifdef _WIN32
	WSACleanup();
#endif
}

void NetworkSystem::setValues(const json &inValues)
{
	socket = UDPSocket();
	socket.slen = sizeof(socket.serverInfo);
	socket.createSocket();
	socket.bindServer(inValues["thisPort"].get<unsigned short>());
	socket.connect(
			inValues["otherIp"].get<std::string>().c_str(),
			inValues["otherPort"].get<unsigned short>()
	);

	exitReceive = false;
	// Lamdba function to use member function
	receiveThread = std::thread([=] { receive(); });
	countdown = 0.01;
}

void NetworkSystem::update(double dt)
{
	countdown -= dt;

	for(auto entity : getEntities())
	{
		TransformComponent* transform = entity->getComponent<TransformComponent>("transformComponent");
		NetworkComponent* network = entity->getComponent<NetworkComponent>("networkComponent");

		if(network->ours && countdown <= 0)
		{
			std::string serialString = transform->serialise();
			serialString = entity->getName() + "=" + serialString;
			socket.send(socket.otherInfo, serialString);
		}
	}
	if(countdown <= 0)
		countdown = 0.01;

	std::lock_guard<std::mutex> lock(messagesMutex);
	for(const auto &messagePair : receivedMessages)
	{
		Entity* entity = ECSManager::i()->findEntity(messagePair.first);
		if(entity)
		{
			TransformComponent* transform = entity->getComponent<TransformComponent>("transformComponent");
			transform->deserialise(messagePair.second);
		}
	}
	receivedMessages.clear();
}

void NetworkSystem::receive()
{
	while(!exitReceive)
	{
		std::string received = socket.receive();
		if(!received.empty())
		{
			std::vector<std::string> parts = splitString(received, '=');
			std::lock_guard<std::mutex> lock(messagesMutex);
			receivedMessages[parts[0]] = parts[1];
		}
	}
}