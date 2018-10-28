//
// Created by Jono on 28/10/2018.
//

#include "gameSettings.h"
#include "loaders/assetManager.h"

void GameSettings::loadSettings() {
	clientConfig = static_cast<ConfigAsset*>(AssetManager::get().loadSync("./conf/clientConfig.conf"));
}

std::string GameSettings::getWindowName() {
	return clientConfig->config["windowName"];
}

int GameSettings::getWindowWidth() {
	return clientConfig->config["windowWidth"];
}

int GameSettings::getWindowHeight() {
	return clientConfig->config["windowHeight"];
}
