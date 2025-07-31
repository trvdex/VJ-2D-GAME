#ifndef _TUTORIAL_INCLUDE
#define _TUTORIAL_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Sprite.h"
#include <irrKlang.h>

class Tutorial
{

public:

	Tutorial();
	~Tutorial();

	static Tutorial& instance()
	{
		static Tutorial TS;
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