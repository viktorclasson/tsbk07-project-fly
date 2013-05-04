#version 130

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

float heightfunction(float x, float z)
{
  float y = abs((sin(x/600)*sin(z/400)*100)+(5.0*sin(z/100.0-0.5)*2.0*sin(x/40.0)))-1.5;
  int integer = 70;
  if (y>integer)
    y=y*pow(y-integer+1,0.6*sin(x/1000)*sin(z/5000));

  return y;
}

void main(void)
{
	// Generate terrain
	vec3 real_position = inPosition;
	real_position.x=(inPosition.x*currentPosition.y/20.0) + currentPosition.x;
	real_position.z=(inPosition.z*currentPosition.y/20.0) + currentPosition.z;
	//real_position.y = abs((sin(real_position.x/600)*sin(real_position.z/400)*100)+(5.0*sin(real_position.z/100.0-0.5)*2.0*sin(real_position.x/40.0)))-1.5;
	real_position.y=heightfunction(real_position.x,real_position.z);
	// Calculate normals, one way of doing it, thera are probably better!
	mat3 normalMatrix1 = mat3(mdlMatrix);
	vec3 u = real_position;
	u.z = u.z + 0.1;
	// u.y = abs((sin(u.x/600)*sin(u.z/400)*100));
	u.y = heightfunction(u.x,u.z);
	u=normalize(u-real_position);
	vec3 v = real_position;
	v.x = v.x + 0.1;
	v.y = heightfunction(v.x,v.z);
	v=normalize(v-real_position);
	vec3 calcNormal = cross(u,v);
	worldNormal = normalize(normalMatrix1 * calcNormal);

	
	
	worldPosition = vec3(mdlMatrix * vec4(real_position,1.0));
	texCoord = inTexCoord;
	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(real_position, 1.0);
}

