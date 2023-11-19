#include "interfaces/Object.h"

void Object::initialize(string filePath, Shader* shader, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis)
{
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->axis = axis;
	this->shader = shader;

	load_obj(filePath);
}

void Object::draw()
{
	for (auto& mesh : groups)
	{
		mesh.update(position, scale, angle, axis);
		mesh.draw();
	}
}

void Object::rotate(float deltaAngle, const glm::vec3& rotationAxis) {
	this->angle += deltaAngle;
	this->axis = rotationAxis;
	for (auto& mesh : groups) {
		mesh.update(position, scale, angle, axis);
	}
}

void Object::translate(const glm::vec3& translation) {
	this->position = translation;
	for (auto& mesh : groups) {
		mesh.update(position, scale, angle, axis);
	}
}

void Object::scale_object(const glm::vec3& scale_factor) {
	this->scale = scale_factor;
	for (auto& mesh : groups) {
		mesh.update(position, scale, angle, axis);
	}
}

glm::vec3 Object::get_position() const
{
	return this->position;
}

void Object::load_obj(const string& filePath)
{
	string texNomes[] = {"../../3D_models/Pokemon/textures/PikachuMouthDh.png",
                         "../../3D_models/Pokemon/textures/PikachuDh.png",
						 "../../3D_models/Pokemon/textures/PikachuHohoDh.png",
						 "../../3D_models/Pokemon/textures/PikachuEyeDh.png",
						 "../../3D_models/Pokemon/textures/PikachuDh.png" };
	int i = 0;

	glm::vec3 color;
	color.r = 1.0;  color.g = 0.0;  color.b = 0.0;


	vector <glm::vec3> vertices;
	vector <glm::vec3> colors;
	vector <GLuint> indices;
	vector <glm::vec2> texCoords;
	vector <glm::vec3> normals;
	vector <GLfloat> vbuffer;

	ifstream inputFile;
	inputFile.open(filePath.c_str());
	if (inputFile.is_open())
	{
		char line[100];
		string sline;

		bool inicio_grupo = true;

		while (!inputFile.eof())
		{
			inputFile.getline(line, 100);
			sline = line;

			string word;

			istringstream ssline(line);
			ssline >> word;
			
			if (word == "v" || inputFile.eof())
			{
				if (inicio_grupo)
				{
					if (vbuffer.size())
					{
						inicio_grupo = false;

						mesh grupo;
						GLuint texID = load_texture(texNomes[i]);
						i++;
						int nVerts;
						GLuint VAO = generate_vao(vbuffer, nVerts);
						grupo.initialize(VAO, nVerts, shader, texID);

						groups.push_back(grupo);
						
						vbuffer.clear();
					}
				}

				glm::vec3 v;				
				ssline >> v.x >> v.y >> v.z;
				vertices.push_back(v);
				colors.push_back(color);
			}
			if (word == "vt")
			{
				glm::vec2 vt;

				ssline >> vt.s >> vt.t;

				texCoords.push_back(vt);
			}
			if (word == "vn")
			{
				glm::vec3 vn;

				ssline >> vn.x >> vn.y >> vn.z;

				normals.push_back(vn);
			}

			if (word == "g")
			{
				inicio_grupo = true;
			}

			if (word == "f")
			{
				string tokens[3];

				ssline >> tokens[0] >> tokens[1] >> tokens[2];

				for (int i = 0; i < 3; i++)
				{
					int pos = tokens[i].find("/");
					string token = tokens[i].substr(0, pos);
					int index = atoi(token.c_str()) - 1;
					indices.push_back(index);

					vbuffer.push_back(vertices[index].x);
					vbuffer.push_back(vertices[index].y);
					vbuffer.push_back(vertices[index].z);
					vbuffer.push_back(colors[index].r);
					vbuffer.push_back(colors[index].g);
					vbuffer.push_back(colors[index].b);
					
					tokens[i] = tokens[i].substr(pos + 1);
					pos = tokens[i].find("/");
					token = tokens[i].substr(0, pos);
					index = atoi(token.c_str()) - 1;

					vbuffer.push_back(texCoords[index].s);
					vbuffer.push_back(texCoords[index].t);
					
					tokens[i] = tokens[i].substr(pos + 1);
					index = atoi(tokens[i].c_str()) - 1;

					vbuffer.push_back(normals[index].x);
					vbuffer.push_back(normals[index].y);
					vbuffer.push_back(normals[index].z);
				}
			}

		}

	}
	else
	{
		cout << "Problema ao encontrar o arquivo " << filePath << endl;
	}
	inputFile.close();

}

GLuint Object::generate_vao(vector<GLfloat> vbuffer, int& nVerts)
{
	GLuint VBO, VAO;

	nVerts = vbuffer.size() / 11;
	
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, vbuffer.size() * sizeof(GLfloat), vbuffer.data(), GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	return VAO;
}

GLuint Object::load_texture(string file_path)
{
	GLuint tex_id;
	
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
				data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex_id;
}