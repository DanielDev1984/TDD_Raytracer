#include "Material.h"

Material::Material(float ambientFactor, float diffuseFactor, float specularFactor, float shininessFactor):
	m_ambientReflectionFactor{ ambientFactor }, 
	m_diffuseReflectionFactor{ diffuseFactor }, 
	m_specularReflectionFactor{ specularFactor }, 
	m_shininessFactor{ shininessFactor } {};
