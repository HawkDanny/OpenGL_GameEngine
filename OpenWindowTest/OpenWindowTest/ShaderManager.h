#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm\include\glm\gtx\euler_angles.hpp"
#include "glm\include\glm\gtx\transform.hpp"
#include <FreeImage.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

class ShaderManager
{
private:
	GLuint program; //index where the loaded shaders are stored on the graphics card

	GLuint loadShader(const char* file, GLenum shaderType);

public:
	GLuint getProgram() const;

	ShaderManager();
	~ShaderManager();

	bool loadShaders(const char* vertexFile, const char* fragmentFile);
};

