//
// Created by Jono on 30/06/2018.
//

#ifndef BLOCKHOP_ACTION_H
#define BLOCKHOP_ACTION_H

#include <json.hpp>
using json = nlohmann::json;
class Entity;
class System;
class Trigger;

class Action {
public:
	Action();
	~Action();

	template<typename T>
	static Action* create() {return new T; };
	virtual void setValues(json inValues) = 0;
	void setName(std::string inName) {name = inName; };
	std::string getName() {return name; };

	virtual void runAction(System *s, Entity *e, Trigger *t){};

private:
	std::string name;
};


#endif //BLOCKHOP_ACTION_H
