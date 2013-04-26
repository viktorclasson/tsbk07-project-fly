#version 150

out vec4 out_Color;
in vec3 fragNormal;
in vec2 fragTexCoord;
uniform sampler2D texUnit;

void main(void)
{
const vec3 light = vec3(0.58, 0.58, 0.58);
float shade = clamp(clamp(dot(normalize(fragNormal), light),0,1) + 0.5, 0, 1);
vec4 texColor = texture(texUnit, fragTexCoord);
out_Color = texColor*shade;
}