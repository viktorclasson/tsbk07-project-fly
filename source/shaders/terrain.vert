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
    y=y*pow(y-integer+1,0.4*sin(x/1000)*sin(z/5000));
  if (y<1)
    y=0.99999;

  return y;
}

void main(void)
{
	// Generate terrain
	vec3 real_position = inPosition;
	// Scale the resolution of the terrain with the altitude of the plane
	float planeheightscale;
	planeheightscale=currentPosition.y/1000.0 +1;
	real_position.x=inPosition.x*planeheightscale + currentPosition.x;
	real_position.z=inPosition.z*planeheightscale + currentPosition.z;
	
	real_position.y=heightfunction(real_position.x,real_position.z);
	
	// Calculate normals, one way of doing it, thera are probably better!
	mat3 normalMatrix1 = mat3(mdlMatrix);
	vec3 u = real_position;
	u.z = u.z + 0.1;
	u.y = heightfunction(u.x,u.z);
	u=normalize(u-real_position);
	vec3 v = real_position;
	v.x = v.x + 0.1;
	v.y = heightfunction(v.x,v.z);
	v=normalize(v-real_position);
	vec3 calcNormal = cross(u,v);
	worldNormal = normalize(normalMatrix1 * calcNormal);
	if(real_position.y < 1)
	{
	  // Do some bumping on the water
	  worldNormal.x = worldNormal.x*0.995+0.005*sin(real_position.x/5.0);
	  worldNormal.z = worldNormal.z*0.995+0.005*sin(real_position.z/5.0);
	  worldNormal=normalize(worldNormal);
	}
	worldPosition = vec3(mdlMatrix * vec4(real_position,1.0));
	
	// Calculate the texture coordinates
	vec2 calctex;
	calctex.x=real_position.x/60.0-floor(real_position.x/60.0);
	calctex.y=real_position.z/60.0-floor(real_position.z/60.0);
	
	texCoord = calctex;
	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(real_position, 1.0);
}

