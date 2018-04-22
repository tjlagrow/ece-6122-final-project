#ifndef ECE_6122_FINAL_PROJECT_MATERIAL_H
#define ECE_6122_FINAL_PROJECT_MATERIAL_H


#include <string>

/**
 * What do each of the material properties mean?
 * Ka: Ambient TODO briefly explain
 * Kd: Diffuse TODO briefly explain
 * Ks: Specular TODO briefly explain
 * Ke: TODO briefly explain
 * shininess: TODO briefly explain
 * Ni: TODO briefly explain
 * Ns: TODO briefly explain
 * d: TODO briefly explain
 * illum:
 * 0. Color on and Ambient off
 * 1. Color on and Ambient on
 * 2. Highlight on
 * 3. Reflection on and Ray trace on
 * 4. Transparency: Glass on, Reflection: Ray trace on
 * 5. Reflection: Fresnel on and Ray trace on
 * 6. Transparency: Refraction on, Reflection: Fresnel off and Ray trace on
 * 7. Transparency: Refraction on, Reflection: Fresnel on and Ray trace on
 * 8. Reflection on and Ray trace off
 * 9. Transparency: Glass on, Reflection: Ray trace off
 * 10. Casts shadows onto invisible surfaces
 *
 * The color includes an ambient constant term, and a diffuse and specular
 * shading term for each light source. The formula is:
 * color = KaIa + Kd { SUM j=1..ls, (N*Lj)Ij } + Ks { SUM j=1..ls, ((H*Hj)^Ns)Ij }
 *
 * Term definitions are:
 * Ia ambient light,
 * Ij light j's intensity,
 * Ka ambient reflectance,
 * Kd diffuse reflectance,
 * Ks specular reflectance,
 * H unit vector bisector between L and V,
 * L unit light vector,
 * N unit surface normal,
 * V unit view vector
 */
class Material
{
public:
	Material() { }

	void setName(std::string name) { m_name = name; }
	void setAmbient(glm::vec3 Ka) { m_Ka = Ka; }
	void setDiffuse(glm::vec3 Kd) { m_Kd = Kd; }
	void setSpecular(glm::vec3 Ks) { m_Ks = Ks; }
	void setEmission(glm::vec3 Ke) { m_Ke = Ke; }
	void setShininess(float shininess) { m_shininess = shininess; }
	void setRefraction(float Ni) { m_Ni = Ni; }
	void setSpecularExponent(float Ns) { m_Ns = Ns; }
	void setDissolveFactor(float d) { m_d = d; }
	void setIllumination(unsigned int illum) { m_illum = illum; }
	void setShadingModel(int model) { m_shadingModel = model; }
	void setBlendFunc(int blendFunc) { m_blendFunc = blendFunc; }
	void setWireframe(int enable) { m_enableWireframe = enable; }

	const std::string &getName() const { return m_name; }
	const glm::vec3 &getAmbient() { return m_Ka; }
	const glm::vec3 &getDiffuse() { return m_Kd; }
	const glm::vec3 &getSpecular() { return m_Ks; }
	const glm::vec3 &getEmission() { return m_Ke; }
	const float &getShininess() { return m_shininess; }
	const float &getRefraction() { return m_Ni; }
	const float &getSpecularExponent() { return m_Ns; }
	const float &getDissolveFactor() { return m_d; }
	const unsigned int &getIllumination() { return m_illum; }
	const int &getShadingModel() { return m_shadingModel; }
	const int &getBlendFunc() { return m_blendFunc; }
	const int &getWireframe() { return m_enableWireframe; }

private:
	std::string m_name;
	glm::vec3 m_Ka; // Ambient
	glm::vec3 m_Kd; // Diffuse
	glm::vec3 m_Ks; // Specular
	glm::vec3 m_Ke; // Emission
	float m_shininess; // Shininess
	float m_Ni; // Optical density; index of refraction
	float m_Ns; // Specular exponent; high exponent is tight highlight
	float m_d; // Factor; amount of dissolve applied to material; transparency
	unsigned int m_illum; // Illumination model
	int m_shadingModel;
	int m_blendFunc;
	int m_enableWireframe;
};

#endif //ECE_6122_FINAL_PROJECT_MATERIAL_H
