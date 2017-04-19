#version 420

layout(location=0) in vec2 vertPos;
layout(location=1) in vec2 vertUV;

out vec2 vUV;

uniform mat4 modelMat;
uniform mat4 projMat;

void main() 
{
    vUV = vec2(vertUV.x,1-vertUV.y);
	gl_Position = projMat * modelMat * vec4(vertPos, 0, 1);
}
