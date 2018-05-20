#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <map>

class Shader
{
	GLuint shaderID;
	//Was const char*, but produced weirdness when moving rendering to boneModel.cpp
	//`Find`ing "boneMats" resulted in "cannot access memory at 0x5" for the mat name
	std::map<std::string, GLint> locations;

	public:
		Shader(const char* vertName, const char* fragName);
		Shader(const char* vertName, const char* fragName, const char* geomName);
		~Shader();

		void use();
		bool addLoc(const char* name);
		GLint getLoc(const char* name) const;
};

#endif // SHADER_H_INCLUDED
