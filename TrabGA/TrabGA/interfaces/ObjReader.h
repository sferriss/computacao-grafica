#pragma once
#include <string>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// GLAD
#include <glad/glad.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class obj_reader
{
public:
    obj_reader();

    ~obj_reader()
    = default;

    static int load_simple_obj(const std::string& file_path, int& n_vertices, glm::vec3 color = glm::vec3(1.0,0.0,0.0));
};
