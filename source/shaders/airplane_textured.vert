#version 130

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec4 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;

uniform mat3 normalMatrix; // Matrix for transformation of normal vectors
uniform mat4 mdlMatrix; // Matrix for transformations of model vertices

void main(void)
{
vec4 position = mdlMatrix * vec4(inPosition, 1.0);

fragPosition = position;
fragNormal = normalMatrix * inNormal;
fragTexCoord = inTexCoord;

gl_Position = position;
}
