//
// Created by Jono on 28/10/2018.
//

#include "gameSettings.h"
#include "loaders/assetManager.h"

void GameSettings::loadSettings() {
	clientConfig = static_cast<ConfigAsset*>(AssetManager::get().loadSync("./conf/clientConfig.conf"));
	clientConfig->setMutable(true);
}

void GameSettings::saveSettings() {
	clientConfig->save();
}

// Getters to save us having to go access config directly
std::string GameSettings::getWindowName() {
	return clientConfig->config["windowName"];
}

int GameSettings::getWindowWidth() {
	return clientConfig->config["windowWidth"];
}

int GameSettings::getWindowHeight() {
	return clientConfig->config["windowHeight"];
}

bool GameSettings::getWindowFullscreen() {
	return clientConfig->config["windowFullscreen"];
}

// Setters to save us having to access config directly
void GameSettings::setWindowName(std::string name) {
	clientConfig->config["windowName"] = name;
}

void GameSettings::setWindowWidth(int width) {
	clientConfig->config["windowWidth"] = width;
}

void GameSettings::setWindowHeight(int height) {
	clientConfig->config["windowHeight"] = height;
}

void GameSettings::setWindowFullscreen(bool isFullscreen) {
	clientConfig->config["windowFullscreen"] = isFullscreen;
}

void GameSettings::setWindow(std::string name, int width, int height, bool isFullscreen) {
	setWindowName(name);
	setWindowWidth(width);
	setWindowHeight(height);
	setWindowFullscreen(isFullscreen);
}