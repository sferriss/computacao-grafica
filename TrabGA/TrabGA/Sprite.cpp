#include "./interfaces/Sprite.h"

sprite::sprite()
{
    setup();
}

void sprite::setup()
{
	constexpr GLfloat vertices[] = {
    	
		//x    y    z    r    g    b
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0, // v0
		-0.5, -0.5,  0.5, 0.0, 1.0, 1.0, // v1
		 0.5, -0.5, -0.5, 1.0, 0.0, 1.0, // v2

		 -0.5, -0.5, 0.5, 1.0, 1.0, 0.0, //v3
		  0.5, -0.5, 0.5, 0.0, 1.0, 1.0, //v4 
		  0.5, -0.5,-0.5, 1.0, 0.0, 1.0, //v5
    	
		 -0.5, -0.5, -0.5, 1.0, 1.0, 0.0, //v6
		  0.0,  0.5,  0.0, 1.0, 1.0, 0.0, //v7
		  0.5, -0.5, -0.5, 1.0, 1.0, 0.0, //v8
    	
		  -0.5, -0.5, -0.5, 1.0, 0.0, 1.0, //v9
		   0.0,  0.5,  0.0, 1.0, 0.0, 1.0, //v10
		  -0.5, -0.5,  0.5, 1.0, 0.0, 1.0, //v11
    	
		  -0.5, -0.5, 0.5, 1.0, 1.0, 0.0, //v12
		   0.0,  0.5, 0.0, 1.0, 1.0, 0.0, //v13
		   0.5, -0.5, 0.5, 1.0, 1.0, 0.0, //v14
    	
		   0.5, -0.5,  0.5, 0.0, 1.0, 1.0, //v15
		   0.0,  0.5,  0.0, 0.0, 1.0, 1.0, //v16
		   0.5, -0.5, -0.5, 0.0, 1.0, 1.0, //v17
	};
	
	glGenBuffers(1, &vbo_);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vao_);
	
	glBindVertexArray(vao_);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
}
