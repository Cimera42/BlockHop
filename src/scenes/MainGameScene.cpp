//
// Created by Jono on 10/07/2018.
//

#include "MainGameScene.h"
#include "../openGLFunctions.h"
#include "../ecs/ecsManager.h"
#include "../ecs/ecsLoader.h"
#include "../components/TeamComponent.h"

MainGameScene::MainGameScene() {}
MainGameScene::~MainGameScene() {}

void MainGameScene::load() {
	ECSLoader ecsLoader = ECSLoader();
	ecsLoader.readStream("maingame.json");

	//Get 'special' entities
	/*mapEntity = ECSManager::get().findEntity("Map");
	mines.push_back(ECSManager::get().findEntity("Mine1"));
	mines.push_back(ECSManager::get().findEntity("Mine2"));
	mines.push_back(ECSManager::get().findEntity("Mine3"));*/
}

void MainGameScene::runLogic(double dt, double alpha) {
	//Run update on our systems
	for(auto sysTuple : ECSManager::get().getLogicSystems())
	{
		auto system = std::get<1>(sysTuple);
		//Get time difference for updating systems
		system->update(dt, alpha);
	}

	//Finally update our 'scores' and see if we're at endgame
	/*int team1Count = 0;
	int team2Count = 0;
	for(auto mine : mines) {
		TeamComponent* teamComponent = mine->getComponent<TeamComponent>();
		if(teamComponent) {
			if(teamComponent->team == TEAM_ONE) {
				team1Count++;
			} else if(teamComponent->team == TEAM_TWO) {
				team2Count++;
			}
		}
	}

	team1Resources += (double) team1Count * resourcePerSec * dt.count();
	team2Resources += (double) team2Count * resourcePerSec * dt.count();

	if(team1Resources > resourceEndGoal) {
		//Transition away to end scene/end game
		std::cout<< "team 1 wins" << std::endl;
	}

	if(team2Resources > resourceEndGoal) {
		//Transition away to end scene/end game
		std::cout<< "team 2 wins" << std::endl;
	}*/
}

void MainGameScene::runPresentation(double dt, double alpha) {
	glClearColor(0.55f, 0.65f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Run update on our systems
	for(auto sysTuple : ECSManager::get().getPresentationSystems())
	{
		auto system = std::get<1>(sysTuple);
		//Get time difference for updating systems
		system->update(dt, alpha);
	}
}