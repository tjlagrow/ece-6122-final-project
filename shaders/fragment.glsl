#version 410

layout (location = 0) out vec4 finalColor;

in DATA
{
    vec4 pos;
    vec2 uv;
    vec4 col;
    vec3 norm;
} fin;

uniform vec4 Ka;
uniform vec4 Kd;
uniform vec4 Ks;

uniform vec3 eyePosition;

void main()
{
	finalColor = Kd;

//	if (hasTexture == 1)
//	{
//		// Perform the texture mapping. Retrieve the texture color
//		// from the texture image using the texture coordinates
//		vec4 textureColor = texture(texUnit, fin.uv);
//
//		// Combine the lighting color with the texture color
//		finalColor = mix(finalColor, textureColor, 0.5f);
//	}
}
