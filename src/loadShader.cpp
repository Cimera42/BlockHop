#include "loadShader.h"
#include "logger.h"
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <map>

/*void loadBinaryShaderPart(GLuint partID, const char * path)
{
    std::vector<char> shaderCode;
    std::ifstream stream(path, std::ios::binary);
    if(stream.is_open())
    {
        shaderCode = std::vector<char>((std::istreambuf_iterator<char>(stream)),
									   (std::istreambuf_iterator<char>()));
    }

    GLint compileResult = GL_FALSE;
    int compileLogLength;

    const char * codePointer = shaderCode.data();
	glShaderBinary(1, &partID, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, codePointer, sizeof(codePointer));
	glSpecializeShader(partID, "main", 0, nullptr, nullptr);

    glGetShaderiv(partID, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(partID, GL_INFO_LOG_LENGTH, &compileLogLength);
    std::vector<char> compileLog(compileLogLength);
    glGetShaderInfoLog(partID, compileLogLength, nullptr, &compileLog[0]);
    if (compileLog.size() > 1)
        Logger(1) << compileLog.data();

    std::vector<char>().swap(compileLog);
}*/

void loadShaderPart(GLuint partID, const char * path)
{
    std::string shaderCode;
    std::ifstream stream(path, std::ios::in);
    if(stream.is_open())
    {
        std::string line;
        while(std::getline(stream, line))
            shaderCode += "\n" + line;
        stream.close();
    }

    GLint compileResult = GL_FALSE;
    int compileLogLength;

    const char * codePointer = shaderCode.c_str();
    glShaderSource(partID, 1, &codePointer, nullptr);
    glCompileShader(partID);

    glGetShaderiv(partID, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(partID, GL_INFO_LOG_LENGTH, &compileLogLength);
    std::vector<char> compileLog(compileLogLength);
    glGetShaderInfoLog(partID, compileLogLength, nullptr, &compileLog[0]);
    if (compileLog.size() > 1)
        Logger(1) << compileLog.data();

    std::vector<char>().swap(compileLog);
}

void createProgram(GLuint progID)
{
    try
    {
        glLinkProgram(progID);
        GLint compileResult = 0;
        int compileLogLength = 0;

            glGetProgramiv(progID, GL_LINK_STATUS, &compileResult);
            glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &compileLogLength);
            std::vector<char> compileLog(compileLogLength);
            glGetProgramInfoLog(progID, compileLogLength, nullptr, &compileLog[0]);
            if (compileLog.size() > 1)
                Logger(1) << compileLog.data();

            std::vector<char>().swap(compileLog);
    }
    catch (std::bad_alloc bad)
    {
        std::cerr<<"Bad allocation from compileLog."<<std::endl;
    }
}

GLuint loadShader(const char * vertPath, const char * fragPath)
{
    Logger(1) << "Creating Shader";
    Logger(1) << "Vertex: " << vertPath;
    GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
        loadShaderPart(vertID, vertPath);

    Logger(1) << "Fragment: " << fragPath;
    GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
        loadShaderPart(fragID, fragPath);

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertID);
    glAttachShader(programID, fragID);

    createProgram(programID);

    glDeleteShader(vertID);
    glDeleteShader(fragID);

    return programID;
}

GLuint loadShaderG(const char * vertPath, const char * fragPath, const char * geoPath)
{
    Logger(1) << "Creating Shader";
    Logger(1) << "Vertex: " << vertPath;
    GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
        loadShaderPart(vertID, vertPath);

    Logger(1) << "Fragment: " << fragPath;
    GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
        loadShaderPart(fragID, fragPath);

    Logger(1) << "Geometry: " << geoPath;
    GLuint geoID = glCreateShader(GL_GEOMETRY_SHADER);
        loadShaderPart(geoID, geoPath);

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertID);
    glAttachShader(programID, fragID);
    glAttachShader(programID, geoID);

    createProgram(programID);

    glDeleteShader(vertID);
    glDeleteShader(fragID);
    glDeleteShader(geoID);

    return programID;
}

void deleteShader(GLuint inShader)
{
    glDeleteProgram(inShader);
}
