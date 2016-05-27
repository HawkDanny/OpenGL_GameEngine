#include "Model.h"



Model::Model()
{
	vertArr = 0;
	vertCount = 0;
	tag = "";
}


Model::~Model()
{
}

//change the parameter to an object
bool Model::buffer(std::string objFile)
{
	std::vector<glm::vec3> locations;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<VertInd> indices;

	std::ifstream inFile;
	inFile.open(objFile);

	std::string input; //the string used with stringstream to parse the file
	if (inFile.is_open())
	{
		while (std::getline(inFile, input))
		{
			std::istringstream ss(input);
			std::string label;

			ss >> label;

			if (label == "v") //vertex locations
			{
				float a, b, c; //floats used for reading in data
				ss >> a >> b >> c;
				locations.push_back(glm::vec3(a, b, c));
			}
			else if (label == "vt") //uvs
			{
				float a, b; //floats used for reading in data
				ss >> a >> b;
				uvs.push_back(glm::vec2(a, b));
			}
			else if (label == "vn") //normals
			{
				float a, b, c; //floats used for reading in data
				ss >> a >> b >> c;
				normals.push_back(glm::vec3(a, b, c));
			}
			else if (label == "f") //indices
			{
				unsigned int a, b, c;
				char slash1, slash2;

				for (int i = 0; i < 4; i++)
				{
					ss >> a >> slash1 >> b >> slash2 >> c;
					indices.push_back(VertInd{ --a, --b, --c });
				}
			}
		}
		inFile.close();
	}

	vertCount = indices.size();

	//Duplicate vertices into a single buffer
	std::vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
		vertBufData[i] = Vertex(locations[indices[i].locInd],
			uvs[indices[i].uvInd],
			normals[indices[i].normInd]);

	GLuint vertBuf;

	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertCount, &(vertBufData[0]), GL_STATIC_DRAW); //Upload data

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3)); //possibly should be (void*)sizeof(VertInd)
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindVertexArray(0);

	return true;
}

void Model::render()
{
	glBindVertexArray(vertArr);
	glDrawArrays(GL_QUADS, 0, this->vertCount);
}
