#version 130

out vec4 outColor;
in vec2 texCoord;
in vec3 worldNormal;
in vec3 worldPosition;
uniform sampler2D tex;
uniform sampler2D skybox_tex;
uniform sampler2D texMountain;

uniform vec3 currentPosition;

vec3 light_pos = vec3(0.58, 0.58, 0.58);


void main(void)
{
  outColor = vec4(0.0,0.0,0.0,0.0);
  float diffuselight;

  diffuselight = max(dot(worldNormal,light_pos),0);
  outColor = outColor + vec4(diffuselight*vec3(1.0, 1.0, 1.0),1.0);
  if (worldPosition.y > 1)
  outColor = outColor*texture(tex, texCoord) + vec4(0.2, 0.2, 0.2, 1.0);
  if (worldPosition.y > 50)
  {
    outColor = (0.5*outColor)*(4.0*texture(texMountain, texCoord)) + vec4(0.2, 0.2, 0.2, 1.0);
  }
  if (worldPosition.y < 1)
  {
    // Reflection of the skybox in the water :D
    vec3 reflectionDir;
    reflectionDir = reflect(worldPosition-currentPosition , worldNormal);
    reflectionDir=normalize(reflectionDir);
    vec2 reflectionTex;
    reflectionTex.x=(reflectionDir.x+1)/2.0;
    reflectionTex.y=(reflectionDir.z+1)/2.0;
    // Recalculate for the white areas in the skybox texture
    //    Bottom right
    if(reflectionTex.x > 0.75 && reflectionTex.y > 0.75)
    {
      if(reflectionTex.x > reflectionTex.y)
	reflectionTex.x = 0.75;
      else
	reflectionTex.y = 0.75;
    }
    //    Some other triangle
    if(reflectionTex.x > 0.75 && reflectionTex.y < 0.25)
    {
      if(reflectionTex.x > 1-reflectionTex.y)
	reflectionTex.y = 0.25;
      else
	reflectionTex.x = 0.75; 
    }
    //    Some other triangle
    if(reflectionTex.x < 0.25 && reflectionTex.y < 0.25)
    {
      if(reflectionTex.x > reflectionTex.y)
	reflectionTex.x = 0.25; 
      else
	reflectionTex.y = 0.25;
    }
    //    Some other triangle
    if(reflectionTex.x < 0.25 && reflectionTex.y > 0.75)
    {
      if(1-reflectionTex.x > reflectionTex.y)
	reflectionTex.y = 0.75;
      else
	reflectionTex.x = 0.25; 
    }
    outColor = 0.2*outColor+0.8*texture(skybox_tex, reflectionTex);
  }
  
}