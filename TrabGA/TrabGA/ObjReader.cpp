#include "./interfaces/ObjReader.h"

obj_reader::obj_reader()
{
    int load_simple_obj(std::string file_path, int& n_vertices, glm::vec3 color = glm::vec3(1.0,0.0,0.0));
}

int obj_reader::load_simple_obj(const std::string& file_path, int& n_vertices, glm::vec3 color)
{
	vector <GLfloat> vbuffer;
	vector <glm::vec3> vertices;
	vector <glm::vec3> normals;
	vector <glm::vec2> texcoords;
	
	ifstream input_file;
	input_file.open(file_path.c_str());

	if (input_file.is_open())
	{
		string sline;
		while (!input_file.eof())
		{
			char line[100];
			input_file.getline(line, 100);
			sline = line;
			string word;

			istringstream ssline(line);
			ssline >> word;
			if (word == "v")
			{
				glm::vec3 v;
				ssline >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}
			if (word == "f")
			{
				string tokens[3];
				ssline >> tokens[0] >> tokens[1] >> tokens[2];
				for (const auto& i : tokens)
				{
					int pos = i.find('/');
					string token = i.substr(0, pos);

					int index = atoi(token.c_str()) - 1;


					vbuffer.push_back(vertices[index].x);
					vbuffer.push_back(vertices[index].y);
					vbuffer.push_back(vertices[index].z);

					vbuffer.push_back(rand() % 256 / 255.0);
					vbuffer.push_back(rand() % 256 / 255.0);
					vbuffer.push_back(rand() % 256 / 255.0);
				}
			}
		}
	}
	else
	{
		cout << "Problema ao encontrar o arquivo " << file_path << endl;
	}
	
	input_file.close();
	GLuint vbo;
	GLuint vao;

	n_vertices = vbuffer.size() / 6; 

	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo); 
	glBufferData(GL_ARRAY_BUFFER, vbuffer.size() * sizeof(GLfloat), vbuffer.data(), GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &vao);  

	glBindVertexArray(vao);
	    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vao;
}
