#pragma once
#include "ArithmeticStructures.h"
class Material
{
public:
	Material() {};
	Material(float ambientFactor, float diffuseFactor, float specularFactor, float shininessFactor, ArithmeticStructures::HomogenousCoordinates color);

	const Material& operator=(Material obj) {//todo: make argument const&
		setAmbientReflectionFactor(obj.getAmbientReflectionFactor());
		setDiffuseReflectionFactor(obj.getDiffuseReflectionFactor());
		setSpecularReflectionFactor(obj.getSpecularReflectionFactor());
		setShininessFactor(obj.getShininessFactor());
		setColor(obj.getColor());
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
	ArithmeticStructures::HomogenousCoordinates getColor() const { return m_color; };
	void setColor(ArithmeticStructures::HomogenousCoordinates color) { m_color = color; };
private:
	float m_ambientReflectionFactor{0.0};
	float m_diffuseReflectionFactor{0.0};
	float m_specularReflectionFactor{0.0};
	float m_shininessFactor{0.0};
	ArithmeticStructures::HomogenousCoordinates m_color{ 0.0,0.0,0.0,1.0 };
};

