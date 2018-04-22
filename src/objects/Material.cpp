#include "Material.h"

/**
 * Constructor
 */
Material::Material()
{
	m_name = "__default__";
}

/**
 * TODO Document
 * @param lhs TODO Document
 * @param rhs TODO Document
 * @return TODO Document
 */
bool operator==(const Material &lhs, const Material &rhs)
{
	return (
		lhs.getAmbient() == rhs.getAmbient() &&
		lhs.getDiffuse() == rhs.getDiffuse() &&
		lhs.getSpecular() == rhs.getSpecular() &&
		lhs.getEmission() == rhs.getEmission() &&
		lhs.getSpecularExponent() == rhs.getSpecularExponent() &&
		lhs.getDissolveFactor() == rhs.getDissolveFactor() &&
		lhs.getShininess() == rhs.getShininess() &&
		lhs.getRefraction() == rhs.getRefraction() &&
		lhs.getIllumination() == rhs.getIllumination()
	);
}
