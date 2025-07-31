#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Sprite.h"

class Credits
{

public:

	Credits();
	~Credits();

	static Credits& instance()
	{
		static Credits TS;
		return TS;
	}

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	Texture spritesheet;
	Sprite* sprite;
	ShaderProgram texProgram;
	glm::mat4 projection;

};

#endif// _TITLE_SCREEN_INCLUDE