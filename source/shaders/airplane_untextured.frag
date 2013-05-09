#version 130

out vec4 out_Color;

in vec4 fragPosition;
in vec3 fragNormal;

uniform vec3 ambientLightLevel;
uniform vec3 sourceLightLevel;
uniform vec3 sourceDirection;
uniform vec3 camPosition;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;
uniform float Tr;

void main(void)
{
vec3 ambLight, diffLight, specLight;

// Ambient light
ambLight = vec3(clamp(Ka.r * ambientLightLevel.r, 0, 1), clamp(Ka.g * ambientLightLevel.g, 0, 1), clamp(Ka.b * ambientLightLevel.b, 0, 1));

// Diffuse reflections
float diffShade = clamp(dot(normalize(fragNormal), sourceDirection), 0, 1);

diffLight = vec3(clamp(Kd.r * sourceLightLevel.r * diffShade, 0, 1), clamp(Kd.g * sourceLightLevel.g * diffShade, 0, 1), clamp(Kd.b * sourceLightLevel.b * diffShade, 0, 1));

// Specular reflections
vec3 r = (2.0 * fragNormal * dot(sourceDirection, fragNormal)) - sourceDirection;
r = normalize(r);
vec3 v = normalize(vec3(fragPosition) - camPosition);

float specShade = max(dot(r,v),0);
specShade = pow(specShade, Ns);

specLight = vec3(clamp(Ks.r * sourceLightLevel.r * specShade, 0, 1), clamp(Ks.g * sourceLightLevel.g * specShade, 0, 1), clamp(Ks.b * sourceLightLevel.b * specShade, 0, 1));

// Output sum of he three components
out_Color = vec4(clamp(ambLight.r + diffLight.r + specLight.r, 0, 1), clamp(ambLight.g + diffLight.g + specLight.g, 0, 1), clamp(ambLight.b + diffLight.b + specLight.b, 0, 1), 1 - Tr);

/*
const vec3 light = vec3(0.58, 0.58, 0.58);
float shade = clamp(clamp(dot(normalize(fragNormal), light),0,1) + 0.5, 0, 1);
vec2 invTexCoord = vec2(fragTexCoord.s, 1 - fragTexCoord.t);
vec4 texColor = texture(texUnit, invTexCoord);
out_Color = texColor*shade;
//out_Color = vec4(0.7, 0.1, 0.3, 1.0)*shade;
*/
}
