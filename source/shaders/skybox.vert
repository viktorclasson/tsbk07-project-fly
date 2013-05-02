#version 150

// In
in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 inTexCoord;
uniform mat4 projMatrix;
uniform mat4 camMatrix;
uniform mat4 mdlMatrix;

// To the fragment shader
out vec2 texcoord;

// Transform matrix
uniform mat4 myMatrix;

void main(void)
{  
  texcoord = inTexCoord;
  gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
