#include "Tutorial.h"
#include "Game.h"

Tutorial::Tutorial()
{

}

Tutorial::~Tutorial()
{
	if (sprite != NULL)
		delete sprite;
}

void Tutorial::init()
{
	// Initialize the shader program
	initShaders();

	// Set the projection matrix
	projection = glm::ortho(0.f, float(1280), float(960), 0.f);

	// Load the texture
	spritesheet.loadFromFile("images/tutorial.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	// Create the sprite
	sprite = Sprite::createSprite(glm::ivec2(1280, 960), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);
}

void Tutorial::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Tutorial::render()
{
	glm::mat4 modelview;
	//Elementos del juego
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	sprite->setPosition(glm::vec2(0, 0));
	sprite->render();
}

void Tutorial::update(int deltaTime)
{
	sprite->update(deltaTime);
}