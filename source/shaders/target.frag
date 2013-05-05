#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 worldNormal;
in vec3 worldPosition;
uniform sampler2D tex;
uniform vec3 camera_position;

vec3 light_pos = vec3(0.58, 0.58, 0.58);


void main(void)
{
  outColor = vec4(0.0,0.0,0.0,0.0);
  
  // Diffuse light
  float diffuselight;
  diffuselight = max(dot(worldNormal,light_pos),0);
  outColor = outColor + vec4(diffuselight*vec3(1.0, 1.0, 1.0),1.0);

  // Specular light
  float speclight;
  float specExp = 50;
  vec3 r = 2.0*worldNormal*(dot(light_pos,worldNormal)) - light_pos;
  r = normalize(r);
  vec3 v = normalize(worldPosition- camera_position);
  speclight = max(dot(r,v),0);
  speclight = pow(speclight,specExp);
  outColor = outColor + vec4(speclight*vec3(1.0, 1.0, 1.0),1.0);
  
  // Total output
  outColor = outColor*texture(tex, texCoord) + vec4(0.2, 0.2, 0.2, 0);
}

