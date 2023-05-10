#pragma once

class TDD_Raytracer
{
public:
	void calculateAndDrawProjectilePathway();
	void calculateAndDrawClock();
	void drawSphereWithBasicShading(); // the closer the sphere is to the rays origin, the brighter the color
	void drawSphereWithPhongShading();
private:
	void updateProgressBar(int progressPercentage);
	void initProgressBar() { std::cout << "Progress [%] : "; m_progress = 0; };
	// helper value for drawing the progressbar
	//todo: maybe outsource these to a "utility" class...
	int m_progress; 
	bool m_progressBarInitialized{ false };
	static constexpr int s_progressBarStepSize{ 5 };
};