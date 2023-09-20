#pragma once

#include "Shader.h"

class sprite
{
public:
    sprite();

    ~sprite()
    = default;

    void setup();
    
protected:
    GLuint vao_{};
    GLuint vbo_{};
};
