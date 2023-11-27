#include "./interfaces/SceneReader.h"

void scene_reader::initialize(string path, Shader* shader)
{
    this->shader = shader;

    load_scene(path);
}

void scene_reader::load_scene(const string& file_path)
{
    ifstream inputFile;
    inputFile.open(file_path.c_str());
    
    if (inputFile.is_open())
    {
        int objCount = 0;
        bool firstObj = true;

        char line[100];
        string sline;
        glm::vec3 translation;
        glm::vec3 scale;
        
        while (!inputFile.eof())
        {
            inputFile.getline(line, 100);
            sline = line;

            string word;

            istringstream ssline(line);
            ssline >> word;
            
            if (word == "newobj")
            {
                objCount++;
            }
            else if (word == "translation")
            {
                float x, y, z;
                ssline >> x >> y >> z;
                translation = glm::vec3(x, y,z);
            }
            else if (word == "scale")
            {
                float x, y, z;
                ssline >> x >> y >> z;
                scale = glm::vec3(x, y,z);
            }
            else if (word == "objpath")
            {
                string objPath;
                ssline >> objPath;
                Object obj;
                obj.initialize(objPath, shader, objCount, translation, scale);
                objects.push_back(obj);
            }
        }
    }
    else
    {
        cout << "Problema ao encontrar o arquivo " << file_path << endl;
    }
    
    inputFile.close();
}
