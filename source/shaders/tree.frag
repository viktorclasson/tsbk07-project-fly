#version 130

out vec4 outColor;
in vec2 texCoord;
in vec3 worldNormal;
in vec3 worldPosition;
uniform sampler2D texbark;
uniform sampler2D texleaf;

vec3 light_pos = vec3(0.58, 0.58, 0.58);


void main(void)
{
  outColor = vec4(0.0,0.0,0.0,0.0);
  float diffuselight;

  diffuselight = max(dot(worldNormal,light_pos),0);
  outColor = outColor + vec4(diffuselight*vec3(1.0, 1.0, 1.0),1.0);
  if (worldPosition.y > 1)
  outColor = outColor*texture(texbark, texCoord) + vec4(0.2, 0.2, 0.2, 1.0);
  else
  outColor = outColor*texture(texbark, texCoord)*vec4(0.3,0.3,1.5,1.0) + vec4(0.2, 0.2, 0.2, 1.0);
}
