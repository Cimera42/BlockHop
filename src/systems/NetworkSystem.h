//
// Created by Tim on 19/05/2018.
//
#ifndef BLOCKHOP_NETWORKSYSTEM_H
#define BLOCKHOP_NETWORKSYSTEM_H

#include "../ecs/system.h"
#include "../UDPSocket.h"
#include <thread>
#include <mutex>
#include <map>

class NetworkSystem : public System
{
	static bool exported;
public:
	NetworkSystem();
	~NetworkSystem();

	bool exitReceive;

	float countdown;
	std::thread receiveThread;
	UDPSocket socket;

	std::mutex messagesMutex;
	std::map<std::string,std::string> receivedMessages;

	void setValues(const json &inValues) override;
	void update(double dt) override;

	void receive();
};


#endif //BLOCKHOP_NETWORKSYSTEM_H
