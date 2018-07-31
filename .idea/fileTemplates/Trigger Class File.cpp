//
// Created by ${USER} on ${DATE}.
//
#parse("C File Header.h")
#[[#include]]# "${NAME}.h"
#[[#include]]# "../ecs/ecsManager.h"

#set ($cmlname = $NAME.substring(0,1).toLowerCase() + $NAME.substring(1))
TRIGGER_EXPORT(${NAME}, "${cmlname}")

${NAME}::${NAME}() {
	// Register runner functions
	// addTriggerRunner("identityX", MODE, static_cast<RunTrigFunc>(&$NAME::func));
}
${NAME}::~${NAME}() {}

void ${NAME}::setValues(json inValues) {}

void runSystemFunction(System* s) {};

bool entityCheck(System* s, Entity* e) {
	return false;
};