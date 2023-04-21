#pragma once
class Material
{
public:
	Material() {};
	Material(float ambientFactor, float diffuseFactor, float specularFactor, float shininessFactor);

	const Material& operator=(Material obj) {//todo: make argument const&
		setAmbientReflectionFactor(obj.getAmbientReflectionFactor());
		setDiffuseReflectionFactor(obj.getDiffuseReflectionFactor());
		setSpecularReflectionFactor(obj.getSpecularReflectionFactor());
		setShininessFactor(obj.getShininessFactor());
		return *this;
	};

	float getAmbientReflectionFactor() const { return m_ambientReflectionFactor; };
	void setAmbientReflectionFactor(float ambientFactor) { m_ambientReflectionFactor = ambientFactor; };
	float getDiffuseReflectionFactor() const { return m_diffuseReflectionFactor; };
	void setDiffuseReflectionFactor(float diffuseFactor) { m_diffuseReflectionFactor = diffuseFactor; };
	float getSpecularReflectionFactor() const { return m_specularReflectionFactor; };
	void setSpecularReflectionFactor(float specularFactor) { m_specularReflectionFactor = specularFactor; };
	float getShininessFactor() const { return m_shininessFactor; };
	void setShininessFactor(float shininessFactor) { m_shininessFactor = shininessFactor; };
private:
	float m_ambientReflectionFactor{0.0};
	float m_diffuseReflectionFactor{0.0};
	float m_specularReflectionFactor{0.0};
	float m_shininessFactor{0.0};
};

