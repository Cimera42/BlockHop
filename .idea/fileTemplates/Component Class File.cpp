//
// Created by ${USER} on ${DATE}.
//
#parse("C File Header.h")
#[[#include]]# "${NAME}.h"
#[[#include]]# "../ecs/ecsManager.h"

#set ($cmlname = $NAME.substring(0,1).toLowerCase() + $NAME.substring(1))
COMPONENT_EXPORT(${NAME}, "${cmlname}")

${NAME}::${NAME}() {}
${NAME}::~${NAME}() {}

void ${NAME}::setValues(const json &inValues) 
{
	//Will throw if incorrect/should automatically be caught by ECSManager

}