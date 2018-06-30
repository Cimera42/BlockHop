//
// Created by Jono on 16/03/2017.
//

#include <fstream>
#include "ecsLoader.h"

ECSLoader::ECSLoader() {};
ECSLoader::~ECSLoader() {};

void ECSLoader::readStream(std::string fileName) {

	//Open a stream
	std::ifstream i(fileName);
	if(!i.is_open()) {
		Logger() << "ECSLoader failed as the file '" + fileName + "' could not be opened." << std::endl;
		exit(-4);
	}

	//Attempt to read file into json object
	json j;
	try {
		i >> j;

		//All components should already be exported

		//Just need to sort out our systems that need certain components
		for (auto& sys : j["systems"]) {
			std::vector<std::string> compsToSub = sys["compsToSub"];
			ECSManager::i()->createSystem(sys["name"], compsToSub);
		}

		//Get entities
		for (auto& ent : j["entities"]) {

			//Collate all components for entity
			std::vector<std::string> compNames;
			std::vector<json> compData;
			for(auto &comp : ent["components"]) {
				compNames.push_back(comp["name"]);
				compData.push_back(comp["values"]);
			}

			ECSManager::i()->createEntity(ent["name"], compNames, compData);
		}

	}catch (std::invalid_argument invalidArgument) {
		Logger() << invalidArgument.what();
		Logger()<<"There's something wrong with the file "+fileName+", cannot read all values. Exiting."<<std::endl;
		exit(-5);
	} catch (...) {
		Logger()<<"There's something wrong with the file "+fileName+", cannot read all values. Exiting."<<std::endl;
		exit(-5);
	}

	i.close();
};