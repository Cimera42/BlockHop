//
// Created by ${USER} on ${DATE}.
//
#parse("C File Header.h")
#[[#include]]# "${NAME}.h"
#[[#include]]# "../ecs/ecsManager.h"

#set ($cmlname = $NAME.substring(0,1).toLowerCase() + $NAME.substring(1))
SYSTEM_EXPORT(${NAME}, "${cmlname}")

${NAME}::${NAME}() {}
${NAME}::~${NAME}() {}

void ${NAME}::update(double dt) 
{
	for(auto entity : getEntities())
	{
		//auto x = entity->getComponent<XComponent>("xComponent");
	}
}