#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 fragNormal;
out vec2 fragTexCoord;

uniform mat3 normalMatrix; // Matrix for transformation of normal vectors
uniform mat4 mdlMatrix; // Matrix for transformations of model vertices

void main(void)
{
fragNormal = normalMatrix * inNormal;

gl_Position = mdlMatrix * vec4(inPosition, 1.0);

fragTexCoord = inTexCoord;
}