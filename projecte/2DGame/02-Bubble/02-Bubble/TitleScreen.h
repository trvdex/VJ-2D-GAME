#ifndef _TITLE_SCREEN_INCLUDE
#define _TITLE_SCREEN_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Sprite.h"

class TitleScreen
{

public:

	TitleScreen();
	~TitleScreen();

	static TitleScreen& instance()
	{
		static TitleScreen TS;
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