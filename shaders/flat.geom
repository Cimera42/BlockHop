#version 420

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout(location=0) in vec3 inPos[3];
layout(location=1) in vec2 inUV[3];
layout(location=2) in vec3 inNorm[3];
layout(location=3) flat in int inMaterialIndex[3];

layout(location=0) out vec3 outPos;
layout(location=1) out vec2 outUV;
layout(location=2) out vec3 outNorm;
layout(location=3) flat out int outMaterialIndex;

void main()
{
    vec3 e1 = inPos[1] - inPos[0];
    vec3 e2 = inPos[2] - inPos[0];
    vec3 n = normalize(cross(e1, e2));

    for(int i=0; i < 3; i++)
    {
        outPos = inPos[i];
        outUV = inUV[i];
        outNorm = n;
        outMaterialIndex = inMaterialIndex[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}
