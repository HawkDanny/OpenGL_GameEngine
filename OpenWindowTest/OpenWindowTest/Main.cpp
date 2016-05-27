#include "Engine.h"


int main()
{
	Engine gameEngine = Engine();
	if (gameEngine.init() == false)
		return -1;

	if (gameEngine.loadTextures("textures/redTexture1024.png", gameEngine.textureIDs[0]) == false) //enemy1
		return -1;
	if (gameEngine.loadTextures("textures/blueTexture1024.png", gameEngine.textureIDs[1]) == false) //enemy2
		return -1;
	if (gameEngine.loadTextures("textures/purpleTexture1024.png", gameEngine.textureIDs[2]) == false) //player
		return -1;
	if (gameEngine.loadTextures("textures/seafoamTexture1024.png", gameEngine.textureIDs[3]) == false) //bullet
		return -1;
	

	//if (gameEngine.loadTextures("textures/TestTexture.png", gameEngine.textureIDs[2]) == false)
		//return -1;

	gameEngine.hardCodeObjectValues();

	if (gameEngine.bufferModels() == false)
		return -1;

	if (gameEngine.useShaders())
		gameEngine.gameLoop();
	return 0;
}