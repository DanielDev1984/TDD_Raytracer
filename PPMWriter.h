#pragma once

#include <iostream>
#include <fstream>
#include "Canvas.h"
class PPMWriter
{
public:
    PPMWriter(int dimX, int dimY, std::string filename) : m_dimX{ dimX }, m_dimY{ dimY }, m_filename{ filename } {};
    void createPPM( Canvas& c) {
        {
            std::ofstream ostrm(m_filename, std::ios::binary);
            ostrm << "P3" << "\n" << m_dimX << ' ' << m_dimY << "\n" << 255 << "\n";
            for (auto y = 0; y < c.getDimY(); y++)
            {
                for (auto x = 0; x < c.getDimX(); x++)
                {
                    auto [r, g, b,a] = c.getImageData(x, y);
                    r = (int)r;
                    g = (int)g;
                    b = (int)b;
                    ostrm << r << ' ' << g << ' ' << b << ' ';
                }
                //ostrm << "\n";
            }
           
        }
    }
private:
	int m_dimX{ 0 }, m_dimY{ 0 };
    const std::string m_filename{ "" };
};

