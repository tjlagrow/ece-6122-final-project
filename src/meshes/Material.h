#ifndef ECE_6122_FINAL_PROJECT_MATERIAL_H
#define ECE_6122_FINAL_PROJECT_MATERIAL_H


class Material
{
public:
	Material() { };
	Material(
		const char *name,
		glm::vec3 Ka,
		glm::vec3 Kd,
		glm::vec3 Ks,
		glm::vec3 Ke,
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
		m_Ni = Ni;
		m_Ns = Ns;
		m_d = d;
		m_illum = illum;
	}

	void setName(const char *name) { m_name = name; }
	void setAmbient(glm::vec3 Ka) { m_Ka = Ka; }
	void setDiffuse(glm::vec3 Kd) { m_Kd = Kd; }
	void setSpecular(glm::vec3 Ks) { m_Ks = Ks; }
	void setEmission(glm::vec3 Ke) { m_Ke = Ke; }
	void setRefraction(float Ni) { m_Ni = Ni; }
	void setSpecularExponent(float Ns) { m_Ns = Ns; }
	void setDissolveFactor(float d) { m_d = d; }
	void setIllumination(unsigned int illum) { m_illum = illum; }

private:
	const char *m_name;
	glm::vec3 m_Ka; // Ambient
	glm::vec3 m_Kd; // Diffuse
	glm::vec3 m_Ks; // Specular
	glm::vec3 m_Ke; // Emission
	float m_Ni; // Optical density; index of refraction
	float m_Ns; // Specular exponent; high exponent is tight highlight
	float m_d; // Factor; amount of dissolve applied to material; transparency
	unsigned int m_illum; // Illumination model

};

#endif //ECE_6122_FINAL_PROJECT_MATERIAL_H
