#version 410

layout (location = 0) out vec4 finalColor;

in DATA
{
    vec4 pos;
    vec2 uv;
    vec4 col;
    vec3 norm;
} fin;

uniform vec4 ambientLightIntensity;
uniform vec4 diffuseLightIntensity;
uniform vec4 specularLightIntensity;

uniform vec4 Ka;
uniform vec4 Kd;
uniform vec4 Ks;

uniform float shininess;
uniform vec4 emission;

uniform float constantAtten;
uniform float linearAtten;
uniform float quadraticAtten;

uniform int lightType;
uniform vec4 lightSourcePosition;
uniform vec4 lightDirection;
uniform float spotlightOuterCone;
uniform float spotlightInnerCone;

uniform vec3 eyePosition;

void main()
{
	// Now calculate the parameters for the lighting equation:
	// color =
	//    (Ka * Lag) +
	//    (Ka * La) +
	//    (attenuation * ((Kd * (N dot L) * Ld)) +
	//    (Ks * ((N dot HV) ^ shininess) * Ls))
	// Ka, Kd, Ks: surface material properties
	// Lag: global ambient light (placeholder for later, not used here)
	// La, Ld, Ls: ambient, diffuse, and specular components of the light source
	// N: normal
	// L: light vector
	// HV: half vector
	// shininess
	// attenuation: light intensity attenuation over distance and spotlight angle

	finalColor = vec4(0.0, 0.0, 0.0, 1.0);

	vec3 lightVector;
	float attenuation = 1.0;

	if (lightType == 1) // Point light source
	{
		lightVector = normalize(lightSourcePosition.xyz - fin.pos.xyz);

		// calculate light attenuation
		float distance = distance(lightSourcePosition.xyz, fin.pos.xyz);

		attenuation = 1.0 / (constantAtten + (linearAtten * distance)
			+ (quadraticAtten * distance * distance));

	}
	else if (lightType == 2) // Directional light source
	{
		// The light position is actually the light vector
		lightVector = lightSourcePosition.xyz;

		// For directional lights, there is no light attenuation
		attenuation = 1.0;
	}
	else if (lightType == 3) // Spotlight light source
	{
		lightVector = normalize(lightSourcePosition.xyz - fin.pos.xyz);

		float distance = distance(lightSourcePosition.xyz, fin.pos.xyz);

		float spotEffect = dot(normalize(lightDirection.xyz), normalize(lightVector));

		// spotlightInnerCone is in radians, not degrees.
		if (spotEffect > cos(spotlightInnerCone))
		{
			// If the vertex is in the spotlight cone
			attenuation = spotEffect /
			    (constantAtten + linearAtten * distance +
				    quadraticAtten * distance * distance);
		}
		else if (spotEffect > cos(spotlightOuterCone))
		{
			// Between inner and outer spotlight cone, make the light attenuate sharply.
			attenuation = (pow(spotEffect, 12)) /
			    (constantAtten + linearAtten * distance +
				    quadraticAtten * distance * distance);
		}
		else
		{
			// If the fragment is outside of the spotlight cone,
			// then there is no light.
			attenuation = 0.0;
		}
	}
	else
	{
		attenuation = 0.0;
	}

	// Calculate diffuse Color
	float NdotL = max(dot(fin.norm, lightVector), 0.0);

	vec4 diffuseColor = Kd * diffuseLightIntensity * NdotL;

	// Calculate Specular color
	// Here we use the original Phong illumination model.
	vec3 E = normalize(eyePosition - fin.pos.xyz);

	vec3 R = normalize(-reflect(lightVector, fin.norm)); // Light reflection vector

	float RdotE = max(dot(R, E), 0.0);

	vec4 specularColor = Ks * specularLightIntensity * pow(RdotE, shininess);

	vec4 ambientColor = Ka * ambientLightIntensity;

	finalColor +=
	    ambientColor + emission + attenuation * (diffuseColor + specularColor);

    finalColor = fin.col;
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
