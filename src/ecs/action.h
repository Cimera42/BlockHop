//
// Created by Jono on 30/06/2018.
//

#ifndef BLOCKHOP_ACTION_H
#define BLOCKHOP_ACTION_H

#include <json.hpp>

class Action {
public:
	Action();
	~Action();


	template<typename T>
	static Trigger* create() {return new T; };
	virtual void setValues(json inValues) = 0;
	void setName(std::string inName) {name = inName; };
	std::string getName() {return name; };

};


#endif //BLOCKHOP_ACTION_H
