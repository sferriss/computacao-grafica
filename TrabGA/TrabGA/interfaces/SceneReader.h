#pragma once
#include "Object.h"

class scene_reader
{
public:
    scene_reader() = default;
    void initialize(string path, Shader* shader);
    vector <Object> objects;

protected:
    void load_scene(const string& file_path);
    Shader* shader;
};
