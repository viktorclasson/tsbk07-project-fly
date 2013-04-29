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
	// Generate terrain
	vec3 real_position = inPosition;
	real_position.x=inPosition.x + currentPosition.x;
	real_position.z=inPosition.z + currentPosition.z;
	real_position.y = abs((sin(real_position.x/600)*sin(real_position.z/400)*100));

	// Calculate normals
	mat3 normalMatrix1 = mat3(mdlMatrix);
	vec3 u = real_position;
	u.z = u.z + 0.1;
	u.y = abs((sin(u.x/600)*sin(u.z/400)*100));
	u=normalize(u-real_position);
	vec3 v = real_position;
	v.x = v.x + 0.1;
	v.y = abs((sin(v.x/600)*sin(v.z/400)*100));
	v=normalize(v-real_position);
	vec3 calcNormal = cross(u,v);
	worldNormal = normalize(normalMatrix1 * calcNormal);

	
	
	worldPosition = vec3(mdlMatrix * vec4(real_position,1.0));
	texCoord = inTexCoord;
	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(real_position, 1.0);
}

