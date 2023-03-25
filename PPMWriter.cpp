#include "PPMWriter.h"

PPMWriter::PPMWriter(int dimX, int dimY, std::string filename) : 
	m_dimX{ dimX }, 
	m_dimY{ dimY }, 
	m_filename{ filename } {};

void PPMWriter::createPPM(Canvas& c) {
    {
        std::ofstream ostrm(m_filename, std::ios::binary);
        ostrm << "P3" << "\n" << m_dimX << ' ' << m_dimY << "\n" << 255 << "\n";
        for (auto y = 0; y < c.getDimY(); y++)
        {
            for (auto x = 0; x < c.getDimX(); x++)
            {
                auto [r, g, b, a] = c.getImageData(x, y);
                r = (int)r;
                g = (int)g;
                b = (int)b;
                ostrm << r << ' ' << g << ' ' << b << ' ';
            }
            //ostrm << "\n";
        }

    }
}