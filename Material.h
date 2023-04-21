#pragma once
class Material
{
public:
	Material(float ambientFactor, float diffuseFactor, float specularFactor, float shininessFactor);
	float getAmbientReflectionFactor() { return m_ambientReflectionFactor; };
	float getDiffuseReflectionFactor() { return m_diffuseReflectionFactor; };
	float getSpecularReflectionFactor() { return m_specularReflectionFactor; };
	float getShininessFactor() { return m_shininessFactor; };
private:
	float m_ambientReflectionFactor{};
	float m_diffuseReflectionFactor{};
	float m_specularReflectionFactor{};
	float m_shininessFactor{};
};

