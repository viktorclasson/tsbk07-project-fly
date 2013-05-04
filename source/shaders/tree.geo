#version 130

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;



in gl_PerVertex
{
  vec4 gl_Position;
} gl_in[];

  //in vec3 worldPosition[1];
  //in vec3 worldNormal[1];


out gl_PerVertex
{
  vec4 gl_Position;
};

void main()
{
  vec4 temp = gl_Position;
  gl_Position=temp;
  EmitVertex();
  temp.x=temp.x+0.01;
  gl_Position=temp;
  EmitVertex();
  temp.x=temp.z+0.01;
  gl_Position=temp;
  EmitVertex();
  EndPrimitive();
}
