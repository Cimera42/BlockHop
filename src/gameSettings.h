//
// Created by Jono on 28/10/2018.
//

#ifndef BLOCKHOP_GAMESETTINGS_H
#define BLOCKHOP_GAMESETTINGS_H

#include "loaders/configAsset.h"

// TODO convert other singletons to https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class GameSettings {
public:
	/* Singleton */
	static GameSettings& get()
	{
		static GameSettings c_instance;
		return c_instance;
	}

private:
	GameSettings() {};

	// static GameSettings c_instance;
	ConfigAsset* clientConfig;
public:
	GameSettings(GameSettings const&) = delete;
	void operator=(GameSettings const&) = delete;

	// Initiate loading
	void loadSettings();

	std::string getWindowName();
	int getWindowWidth();
	int getWindowHeight();

	// TODO methods to update game settings as needed
};


#endif //BLOCKHOP_GAMESETTINGS_H
