#version 420

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vPos[3];
in vec2 vUV[3];
in vec3 vNorm[3];
flat in int vMaterialIndex[3];

out vec3 gPos;
out vec2 gUV;
out vec3 gNorm;
flat out int gMaterialIndex;

void main()
{
    vec3 e1 = vPos[1] - vPos[0];
    vec3 e2 = vPos[2] - vPos[0];
    vec3 n = normalize(cross(e1, e2));

    for(int i=0; i < 3; i++)
    {
        gPos = vPos[i];
        gUV = vUV[i];
        gNorm = n;
        gMaterialIndex = vMaterialIndex[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}
