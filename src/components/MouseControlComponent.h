//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_MOUSECONTROLCOMPONENT_H
#define BLOCKHOP_MOUSECONTROLCOMPONENT_H

#include "../ecs/component.h"

class MouseControlComponent : public Component
{
    static bool exported;
    double xSensitivity, ySensitivity;
    
public:
    MouseControlComponent();
    ~MouseControlComponent();
    void setValues(json inValues);

	double getXSensitivity() const;
	double getYSensitivity() const;

	friend std::ostream &operator<< (std::ostream &os, MouseControlComponent const &c) {
        os << "MouseControlComponent";
        return os;
    }
};

#endif //BLOCKHOP_MOUSECONTROLCOMPONENT_H