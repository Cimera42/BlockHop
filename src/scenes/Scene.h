//
// Created by Jono on 10/07/2018.
//

#ifndef BLOCKHOP_SCENE_H
#define BLOCKHOP_SCENE_H

#include <chrono>

class Scene {

public:
	Scene();
	virtual ~Scene();

	// TODO:
	// Probably singleton pattern again, with a scene manager (ignore for now)
	// Transition function - between two scenes what do we do (display image? etc)
	// Loading function - How/what do we load? Do we use ECSManager etc?
	// Run function -

	virtual void transition() {};
	virtual void load() {};
	// virtual void run(std::chrono::duration<double> dt) {};
	virtual void runLogic(double) {};
	virtual void runPresentation(double) {};
};


#endif //BLOCKHOP_SCENE_H
