#pragma once

#include <iostream>
#include <fstream>
#include "Canvas.h"
class PPMWriter
{
public:
    PPMWriter(int dimX, int dimY, std::string filename);
    void createPPM(Canvas& c);
private:
	int m_dimX{ 0 }, m_dimY{ 0 };
    const std::string m_filename{ "" };
};

