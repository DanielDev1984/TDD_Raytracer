#pragma once
#include <iostream>
class TDD_Raytracer
{
public:
	void calculateAndDrawProjectilePathway();
	void calculateAndDrawClock();
	void drawSphereWithBasicShading(); // the closer the sphere is to the rays origin, the brighter the color
	//todo: change the function signature -> currently too much information needs to be passed
	void drawSphereWithPhongShading(const float lightPos_x, const float lightPos_y, const float lightPos_z, std::string fileName, std::array<unsigned int, 3> sphereColor);
private:
	void updateProgressBar(int progressPercentage);
	void initProgressBar() { std::cout << "Progress [%] : "; m_progress = 0; };
	// helper value for drawing the progressbar
	//todo: maybe outsource these to a "utility" class...
	int m_progress; 
	bool m_progressBarInitialized{ false };
	static constexpr int s_progressBarStepSize{ 5 };
};