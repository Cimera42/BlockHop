//
// Created by ${USER} on ${DATE}.
//
#parse("C File Header.h")
#set ($capsProjName = $PROJECT_NAME.toUpperCase())
#set ($capsName = $NAME.toUpperCase())
#set ($INCLUDE_GUARD = $capsProjName + "_" + $capsName + "_H")
#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

#[[#include]]# "../ecs/component.h"

class ${NAME} : public Component
{
	static bool exported;
public:
	${NAME}();
	~${NAME}();
	void setValues(const json &inValues);

	friend std::ostream &operator<< (std::ostream &os, ${NAME} const &c) {
		os << "${NAME}";
		return os;
	}
};

#[[#endif]]# //${INCLUDE_GUARD}