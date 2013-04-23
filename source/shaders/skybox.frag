#version 150

// From the vertex shader
in vec2 texcoord;
uniform sampler2D texUnit;

// Out
out vec4 out_Color;

uniform mat4 texMatrix;

void main(void)
{
  out_Color = texture(texUnit, texcoord);
  
}
