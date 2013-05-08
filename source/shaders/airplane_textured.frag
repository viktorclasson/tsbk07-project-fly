#version 130

out vec4 out_Color;
in vec3 fragNormal;
in vec2 fragTexCoord;
uniform sampler2D texUnit;

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
vec2 invTexCoord = vec2(fragTexCoord.s, 1 - fragTexCoord.t);
vec4 texColor = texture(texUnit, invTexCoord);
out_Color = texColor*shade;
//out_Color = vec4(0.7, 0.1, 0.3, 1.0)*shade;
}
