#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 worldNormal;
in vec3 worldPosition;
uniform sampler2D tex;

vec3 light_pos = vec3(0.58, 0.58, 0.58);


void main(void)
{
  outColor = vec4(0.0,0.0,0.0,0.0);
  float diffuselight;

  diffuselight = max(dot(worldNormal,light_pos),0);
  outColor = outColor + vec4(diffuselight*vec3(1.0, 1.0, 1.0),1.0);
  if (worldPosition.y > 1)
  outColor = outColor*texture(tex, texCoord) + vec4(0.2, 0.2, 0.2, 1.0);
  else
  outColor = outColor*texture(tex, texCoord)*vec4(0.5,0.5,1.5,1.0) + vec4(0.2, 0.2, 0.2, 1.0);
}
