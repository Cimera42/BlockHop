//
// Created by Jono on 28/10/2018.
//

#ifndef BLOCKHOP_GAMESETTINGS_H
#define BLOCKHOP_GAMESETTINGS_H

#include "loaders/configAsset.h"

// Note: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
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

	// Non-client controlled settings
	const double updateTimestep = 1.0f/10.0f; //1.0f/60.0f

	// Initiate loading / saving
	void loadSettings();
	void saveSettings();

	// Getters...
	std::string getWindowName();
	int getWindowWidth();
	int getWindowHeight();
	bool getWindowFullscreen();
	bool getVSync();

	// Setters...
	void setWindowName(std::string name);
	void setWindowWidth(int width);
	void setWindowHeight(int height);
	void setWindowFullscreen(bool isFullscreen);
	void setVSync(bool isVSync);
	// Ability to set multiple at once for convenience
	void setWindow(std::string name, int width, int height, bool isFullscreen, bool isVSync);
};


#endif //BLOCKHOP_GAMESETTINGS_H
