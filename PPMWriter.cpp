#include "PPMWriter.h"

PPMWriter::PPMWriter(int dimX, int dimY, std::string filename) : 
	m_dimX{ dimX }, 
	m_dimY{ dimY }, 
	m_filename{ filename } {};

// create a binary (i.e. "P6") ppm
//todo: adapt tests when needed!
void PPMWriter::createPPM(Canvas& c) {
    {

        /* Open PPM File */
        FILE* file;
        errno_t err = fopen_s(&file, "renderedImage.ppm", "wb");
        if (err==0)
        {
            std::cout << "fopen succeeded!" << "\n";

            /* Write PPM Header */
            fprintf(file, "P6 %d %d %d\n", m_dimX, m_dimY, 255); /* width, height, maxval */

            /* Write Image Data */
            for (auto y = 0; y < c.getDimY(); y++)
            {
                for (auto x = 0; x < c.getDimX(); x++)
                {
                    //todo: make this more efficient
                    auto [r, g, b, a] = c.getImageData(x, y);
                    unsigned char colorValue[3];
                    colorValue[0] = r;
                    colorValue[1] = g;
                    colorValue[2] = b;
                    fwrite(colorValue, 1, 3, file);
                }
            }
            /* Close PPM File */
            fclose(file);
        }
        else
        {
            std::cout << "fopen failed!" << "\n";
        }
    }
}