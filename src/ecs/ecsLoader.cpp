//
// Created by Jono on 16/03/2017.
//

#include <fstream>
#include "ecsLoader.h"
#include "../logger.h"

ECSLoader::ECSLoader() {};
ECSLoader::~ECSLoader() {};

void ECSLoader::readStream(std::string fileName) {

    //Open a stream
    std::ifstream i(fileName);

    //Attempt to read file into json object
    json j;

    try {
        i >> j;

        //Get components first
        for (auto& comp : j["components"]) {
            std::cout << comp << '\n';
            //ECSManager::i()->createComponent(comp, j1)
            //TODO: Just realised we dont need to use createComponent here because we can
            // just create entities and supply with component data
        }

        //Get systems

        //Get entities

    } catch (...) {
        Logger()<<"This didnt work"<<std::endl;
    }





};