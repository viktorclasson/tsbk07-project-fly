#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

// out
out vec2 texCoord;
out vec3 worldNormal;
out vec3 worldPosition;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	worldNormal = normalize(normalMatrix1 * inNormal);
	
	// Generate terrain
	vec3 test_position = inPosition;
	test_position.y = sin(inPosition.x/40)*sin(inPosition.z/40)*20+sin(inPosition.x/300)*sin(inPosition.z/200)*5;
	
	
	worldPosition = vec3(mdlMatrix * vec4(test_position,1.0));
	texCoord = inTexCoord;
	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(test_position, 1.0);
}

