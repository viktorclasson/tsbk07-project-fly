#version 130

in vec3 inPosition;
in vec3 inNormal;

out vec4 fragPosition;
out vec3 fragNormal;

uniform mat3 normalMatrix; // Matrix for transformation of normal vectors
uniform mat4 mdlMatrix; // Matrix for transformations of model vertices

void main(void)
{
vec4 position = mdlMatrix * vec4(inPosition, 1.0);

fragPosition = position;
fragNormal = normalMatrix * inNormal;

gl_Position = position;
}
