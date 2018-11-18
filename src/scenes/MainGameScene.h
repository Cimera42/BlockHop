//
// Created by Jono on 10/07/2018.
//

#ifndef BLOCKHOP_MAINGAMESCENE_H
#define BLOCKHOP_MAINGAMESCENE_H

#include "scene.h"
#include <vector>

class Entity;

class MainGameScene : public Scene {
public:
	MainGameScene();
	~MainGameScene();

	double team1Resources = 0;
	double team2Resources = 0;

	const int resourceEndGoal = 2000;
	const int resourcePerSec = 10;

	//Special map handles (no need to waste cycles lets be real)
	Entity* mapEntity;
	std::vector<Entity*> mines;
	//Entity* team1BossEntity;
	//Entity* team2BossEntity;
	//Entity* cameraEntity;

	void load();
	void runLogic(double dt, double alpha);
	void runPresentation(double dt, double alpha);
};


#endif //BLOCKHOP_MAINGAMESCENE_H
