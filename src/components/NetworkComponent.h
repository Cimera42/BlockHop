//
// Created by Tim on 20/05/2018.
//

#ifndef BLOCKHOP_NETWORKCOMPONENT_H
#define BLOCKHOP_NETWORKCOMPONENT_H

#include "../ecs/component.h"

class NetworkComponent : public Component
{
	static bool exported;
public:
	NetworkComponent() = default;
	~NetworkComponent() = default;

	bool ours;

	void setValues(const json &inValues);

	friend std::ostream &operator<< (std::ostream &os, NetworkComponent const &c) {
		os << "NetworkComponent";
		return os;
	}
};

#endif //BLOCKHOP_NETWORKCOMPONENT_H