#include "ShaderManager.h"

GLuint ShaderManager::getProgram() const
{
	return this->program;
}

ShaderManager::ShaderManager()
{
	this->program = 0;
}

ShaderManager::~ShaderManager()
{

}


GLuint ShaderManager::loadShader(const char * file, GLenum shaderType)
{
	std::ifstream inShader;
	std::string input;

	inShader.open(file, std::ios::binary);

	if (inShader.is_open())
	{
		//Read in the file
		inShader.seekg(0, std::ios::end);
		int length = (int)(inShader.tellg());
		inShader.seekg(0, std::ios::beg);

		char* contents = new char[length + 1];
		inShader.read(contents, length);
		contents[length] = 0;

		inShader.close();



		GLuint shaderIndex;
		shaderIndex = glCreateShader(shaderType);

		glShaderSource(shaderIndex, 1, &contents, 0);
		glCompileShader(shaderIndex);

		GLint results;
		glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &results);

		if (results != 0) //successful
			return shaderIndex;
		else //failure
		{
			GLint logLength;
			glGetShaderiv(shaderIndex, GL_INFO_LOG_LENGTH, &logLength);
			GLchar* logArr = new GLchar[logLength];
			glGetShaderInfoLog(shaderIndex, logLength, 0, logArr);

			std::cout << logArr << std::endl;

			glDeleteShader(shaderIndex);
			delete[] logArr;
		}


		delete[] contents;
	}
}

bool ShaderManager::loadShaders(const char * vertexFile, const char * fragmentFile)
{
	GLuint vertexLoadResults;
	vertexLoadResults = loadShader(vertexFile, GL_VERTEX_SHADER);
	if (vertexLoadResults == 0) return 0;

	GLuint fragmentLoadResults;
	fragmentLoadResults = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (fragmentLoadResults == 0) return 0;

	this->program = glCreateProgram();
	glAttachShader(this->program, vertexLoadResults);
	glAttachShader(this->program, fragmentLoadResults);

	glLinkProgram(this->program);

	GLint results;
	glGetProgramiv(this->program, GL_LINK_STATUS, &results);

	if (results != 0) //successful
		return true;
	else //failure
	{
		GLint logLength;
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* logArr = new GLchar[logLength];
		glGetProgramInfoLog(this->program, logLength, 0, logArr);

		std::cout << logArr << std::endl;

		//TODO: delete program here
		delete[] logArr;
	}
}
