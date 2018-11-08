//
// Created by Tim on 2/09/2018.
//

#ifndef BLOCKHOP_SOUNDCOMPONENT_H
#define BLOCKHOP_SOUNDCOMPONENT_H

#include "../ecs/component.h"
#include <string>
#include <soloud/soloud_wav.h>

class SoundComponent : public Component<SoundComponent>
{
public:
	SoundComponent();
	~SoundComponent();
	void setValues(json inValues);

	std::string filename;
	SoLoud::Wav wav;
	SoLoud::handle soundHandle;

	friend std::ostream &operator<< (std::ostream &os, SoundComponent const &c) {
		os << "SoundComponent";
		return os;
	}
};

#endif //BLOCKHOP_SOUNDCOMPONENT_H