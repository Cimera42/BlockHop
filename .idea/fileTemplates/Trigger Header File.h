//
// Created by ${USER} on ${DATE}.
//
#parse("C File Header.h")
#set ($capsProjName = $PROJECT_NAME.toUpperCase())
#set ($capsName = $NAME.toUpperCase())
#set ($INCLUDE_GUARD = $capsProjName + "_" + $capsName + "_H")
#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

#[[#include]]# "../ecs/trigger.h"

class ${NAME} : public Trigger<${NAME}>
{
	// egRunnerFunction(System* s, Entity e);
public:
	${NAME}();
	~${NAME}();
	void setValues(json inValues);

	void runSystemFunction(System* s);
	bool entityCheck(System* s, Entity* e);
};

#[[#endif]]# //${INCLUDE_GUARD}