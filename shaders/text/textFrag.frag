#version 420

in vec2 vUV;

out vec4 outColour;

uniform vec4 u_colour;
uniform float u_min;
uniform float u_max;
uniform sampler2DArray textureSampler;

void main() 
{
    float dist = texture(textureSampler, vec3(vUV,0)).r;
    float calpha = smoothstep(u_min, u_max, dist);
    outColour = vec4(u_colour.rgb, calpha * u_colour.a);
}
