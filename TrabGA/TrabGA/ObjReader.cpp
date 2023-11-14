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
	
	ifstream inputFile;
	inputFile.open(file_path.c_str());

	if (inputFile.is_open())
	{
		char line[100];
		string sline;
		while (!inputFile.eof())
		{
			inputFile.getline(line, 100);
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
			if (word == "vn")
			{
				glm::vec3 vn;
				ssline >> vn.x >> vn.y >> vn.z;
				normals.push_back(vn);

			}
			if (word == "vt")
			{
				glm::vec2 vt;
				ssline >> vt.s >> vt.t;
				texcoords.push_back(vt);

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

					vbuffer.push_back(vertices[index].x);
					vbuffer.push_back(vertices[index].y);
					vbuffer.push_back(vertices[index].z);

					vbuffer.push_back(1.0);
					vbuffer.push_back(0.0);
					vbuffer.push_back(0.0);
					
					tokens[i] = tokens[i].substr(pos+1, tokens[i].size());
					pos = tokens[i].find("/");
					token = tokens[i].substr(0, pos);
					int indext = atoi(token.c_str()) - 1;

					vbuffer.push_back(texcoords[indext].s);
					vbuffer.push_back(texcoords[indext].t);
					
					token = tokens[i].substr(pos + 1, tokens[i].size());
					int indexn = atoi(token.c_str()) - 1;

					vbuffer.push_back(normals[indexn].x);
					vbuffer.push_back(normals[indexn].y);
					vbuffer.push_back(normals[indexn].z);

				}
			}
		}
	}
	else
	{
		cout << "Problema ao encontrar o arquivo " << file_path << endl;
	}
	inputFile.close();
	GLuint VBO, VAO;

	n_vertices = vbuffer.size() / 11;   
	

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
