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
    //outColor = outColor*texture(tex, texCoord)*vec4(0.8,0.6,1.4,1.0) + vec4(0.2, 0.2, 0.2, 1.0);
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
    // Recalculate for the white triangles, can maybe be improved by another if in the if's to check which triangle you're in.
    //    Bottom right
    if(reflectionTex.x > 0.75 && reflectionTex.y > 0.75)
    {
      reflectionTex.x = 0.75; //reflectionTex.y = 0.75;
    }
    //    Some other triangle
    if(reflectionTex.x > 0.75 && reflectionTex.y < 0.25)
    {
      reflectionTex.y = 0.25; //reflectionTex.y = 0.25;
    }
    //    Some other triangle
    if(reflectionTex.x < 0.25 && reflectionTex.y < 0.25)
    {
      reflectionTex.x = 0.25; //reflectionTex.y = 0.25;
    }
    //    Some other triangle
    if(reflectionTex.x < 0.25 && reflectionTex.y > 0.75)
    {
      reflectionTex.y = 0.75; //reflectionTex.y = 0.75;
    }
    outColor = texture(skybox_tex, reflectionTex);
    //outColor = outColor*texture(tex, texCoord)*vec4(0.5,0.5,0.5,1.0)+ vec4(0.15,0.15,0.8,1.0) + vec4(0.2, 0.2, 0.2, 1.0);
  }
  
}