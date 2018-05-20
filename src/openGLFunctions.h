#ifndef OPENGLBITS_H_INCLUDED
#define OPENGLBITS_H_INCLUDED

#include <GL/glew.h>

void glSetActiveTexture(GLenum);
void glSetBindTexture(GLenum, GLuint);
void glSetBindVertexArray(GLuint);
void glSetBindFramebuffer(GLenum, GLuint);
void glSetUseProgram(GLuint);

bool initGLFW();
bool initGLEW();

#endif // OPENGLBITS_H_INCLUDED
