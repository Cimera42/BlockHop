//
// Created by ${USER} on ${DATE}.
//
#parse("C File Header.h")
#set ($capsProjName = $PROJECT_NAME.toUpperCase())
#set ($capsName = $NAME.toUpperCase())
#set ($INCLUDE_GUARD = $capsProjName + "_" + $capsName + "_H")
#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

#[[#include]]# "../ecs/system.h"

class ${NAME} : public System 
{
	static bool exported;
public:
	${NAME}();
	~${NAME}();

	void update(double dt);
};

#[[#endif]]# //${INCLUDE_GUARD}