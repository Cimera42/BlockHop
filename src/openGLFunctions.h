#ifndef OPENGLBITS_H_INCLUDED
#define OPENGLBITS_H_INCLUDED

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void glSetActiveTexture(GLenum);
void glSetBindTexture(GLenum, GLuint);
void glSetBindVertexArray(GLuint);
void glSetBindFramebuffer(GLenum, GLuint);
void glSetUseProgram(GLuint);

bool initGLFW();
bool initGLEW(bool printExtensions);

#endif // OPENGLBITS_H_INCLUDED
