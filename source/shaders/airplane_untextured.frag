#version 130

out vec4 out_Color;
in vec3 fragNormal;

uniform vec3 ambientLightLevel;
uniform vec3 sourceLightLevel;
uniform vec3 sourceDirection;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;
uniform float Tr;

void main(void)
{
const vec3 light = vec3(0.58, 0.58, 0.58);
float shade = clamp(clamp(dot(normalize(fragNormal), light),0,1) + 0.5, 0, 1);
out_Color = vec4(shade, shade, shade, 1.0);
}
