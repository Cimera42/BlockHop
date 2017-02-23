#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#define GLEW_STATIC
#include <GL/glew.h>

#include <map>

class Shader
{
    GLuint shaderID;
    std::map<const char*, GLint> locations;

    public:
        Shader(const char* vertName, const char* fragName);
        Shader(const char* vertName, const char* fragName, const char* geomName);
        ~Shader();

        void use();
        bool addLoc(const char* name);
        GLint getLoc(const char* name);
};

#endif // SHADER_H_INCLUDED
