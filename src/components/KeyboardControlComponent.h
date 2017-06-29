//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_KEYBOARDCONTROLCOMPONENT_H
#define BLOCKHOP_KEYBOARDCONTROLCOMPONENT_H

#include "../ecs/component.h"

class KeyboardControlComponent : public Component
{
	static bool exported;

	int forwardKey, backKey, leftKey, rightKey, upKey, downKey;
	
	int convertStringCode(std::string val);
	
public:
	KeyboardControlComponent();
	~KeyboardControlComponent();
	void setValues(json inValues);

	int getForwardKey() const;
	int getBackKey() const;
	int getLeftKey() const;
	int getRightKey() const;
	int getUpKey() const;
	int getDownKey() const;

	friend std::ostream &operator<< (std::ostream &os, KeyboardControlComponent const &c) {
		os << "KeyboardControlComponent";
		return os;
	}
};

#endif //BLOCKHOP_KEYBOARDCONTROLCOMPONENT_H