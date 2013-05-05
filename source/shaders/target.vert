#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

// out
out vec2 texCoord;
out vec3 worldNormal;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	worldNormal = normalize(normalMatrix1 * inNormal);
	texCoord = inTexCoord;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
