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
uniform vec3 currentPosition;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	worldNormal = normalize(normalMatrix1 * inNormal);
	
	// Generate terrain
	vec3 test_position = inPosition;
	test_position.x=inPosition.x + currentPosition.x;
	test_position.z=inPosition.z + currentPosition.z;
	test_position.y = abs((sin(test_position.x/600)*sin(test_position.z/400)*100));

	
	
	worldPosition = vec3(mdlMatrix * vec4(test_position,1.0));
	texCoord = inTexCoord;
	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(test_position, 1.0);
}

