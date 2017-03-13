#include "shader.h"
#include "openGLFunctions.h"
#include "loadShader.h"
#include "logger.h"

Shader::Shader(const char* vertName, const char* fragName)
{
    shaderID = loadShader(vertName, fragName);
}
Shader::Shader(const char* vertName, const char* fragName, const char* geomName)
{
    shaderID = loadShaderG(vertName, fragName, geomName);
}

Shader::~Shader()
{
    glDeleteShader(shaderID);
}

void Shader::use()
{
    glSetUseProgram(shaderID);
}

bool Shader::addLoc(const char* name)
{
    GLint loc = glGetUniformLocation(shaderID, name);
    if(loc >= 0)
    {
        locations[name] = loc;
        return true;
    }
    return false;
}

GLint Shader::getLoc(const char* name)
{
    auto iter = locations.find(name);
    if(iter != locations.end())
        return iter->second;
    return -1;
}
