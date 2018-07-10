//
// Created by Jono on 10/07/2018.
//

#ifndef BLOCKHOP_TEAMCOMPONENT_H
#define BLOCKHOP_TEAMCOMPONENT_H

#include "../ecs/component.h"

enum teams {
	TEAM_ONE = 0,
	TEAM_TWO = 1
};

class TeamComponent : public Component
{
	static bool exported;
public:
	TeamComponent();
	~TeamComponent();
	void setValues(json inValues);

	int team;

	friend std::ostream &operator<< (std::ostream &os, TeamComponent const &c) {
		os << "TeamComponent";
		return os;
	}
};

#endif //BLOCKHOP_TEAMCOMPONENT_H