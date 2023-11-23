#include "interfaces/MtlReader.h"

#include <fstream>
#include <iostream>
#include <sstream>

void MtlReader::read(string filename)
{
    ifstream inputFile(filename);
    Material material;
    material.initialize();

    if (inputFile.is_open())
    {
        char line[100];
        string sline;
        
        while (!inputFile.eof())
        {
            inputFile.getline(line, 100);
            sline = line;

            stringstream ssline(line);

            string word;
            ssline >> word;
            
            if (word == "map_Kd")
            {
                string tex;
                ssline >> tex;
                material.textures.push_back(tex);
            }
            else if (word == "Ka")
            {
                float x, y, z;
                ssline >> x >> y >> z;
                material.ka = glm::vec3(x, y,z);
            }
            else if (word == "Kd")
            {
                float x, y, z;
                ssline >> x >> y >> z;
                material.kd = glm::vec3(x, y,z);
            }
            else if (word == "Ks")
            {
                float x, y, z;
                ssline >> x >> y >> z;
                material.ks = glm::vec3(x, y,z);
            }
            else if (word == "q")
            {
                float x;
                ssline >> x;
                material.q = x;
            }
        }

        this->material = material;
    }
    else
    {
        cout << "Problema ao encontrar o arquivo " << filename << endl;
    }
    inputFile.close();
}
