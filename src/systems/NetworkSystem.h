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

	// TODO: Restructure networking system

	/*
	 * -- Sending --
	 * Network - Systems: High priority transfer queue (actual PQueue impl if necessary) + synchronisation-required data queue
	 * Network - Socket : Direct transfer packets + prepare and transfer synch-required data to socket.send
	 * Socket  - Socket : Break down packets into max payload size, appened necessary data for required networking paradigms
	 *
	 * -- Receiving --
	 * Socket  - Socket : Recombine packets and place in receive queue
	 * Network - Socket : Take packets from receive queue and handle
	 * Network - Systems: Drop packets into component specific data queue, synchronisation-required data should be hotswapped if newer
	 */

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
