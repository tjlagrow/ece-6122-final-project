#ifndef ECE_6122_FINAL_PROJECT_MATERIAL_H
#define ECE_6122_FINAL_PROJECT_MATERIAL_H


#include <string>

class Material
{
public:
	Material() { };
	Material(
		std::string name,
		glm::vec3 Ka,
		glm::vec3 Kd,
		glm::vec3 Ks,
		glm::vec3 Ke,
		float shininess,
		float Ni,
		float Ns,
		float d,
		unsigned int illum)
	{
		m_name = name;
		m_Ka = Ka;
		m_Kd = Kd;
		m_Ks = Ks;
		m_Ke = Ke;
		m_shininess = shininess;
		m_Ni = Ni;
		m_Ns = Ns;
		m_d = d;
		m_illum = illum;
	}

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

	std::string getName() { return m_name; }
	glm::vec3 getAmbient() { return m_Ka; }
	glm::vec3 getDiffuse() { return m_Kd; }
	glm::vec3 getSpecular() { return m_Ks; }
	glm::vec3 getEmission() { return m_Ke; }
	float getShininess() { return m_shininess; }
	float getRefraction() { return m_Ni; }
	float getSpecularExponent() { return m_Ns; }
	float getDissolveFactor() { return m_d; }
	unsigned int getIllumination() { return m_illum; }

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

};

#endif //ECE_6122_FINAL_PROJECT_MATERIAL_H
