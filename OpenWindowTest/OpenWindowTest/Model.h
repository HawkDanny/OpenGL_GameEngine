#include "PhysicsManager.h"

struct VertInd
{
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;
};

class Model
{
private:
	GLuint vertArr;
	unsigned int vertCount;

public:
	std::string tag;

	Model();
	~Model();

	bool buffer(std::string objFile);
	void render();
};

