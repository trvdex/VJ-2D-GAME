#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


enum GameScreen
{
	SCREEN_TITLE,
	SCREEN_TUTORIAL,
	SCREEN_GAME,
	SCREEN_CREDITS
};

void Game::init()
{
	bPlay = true;
	state = SCREEN_TITLE;
	glClearColor(0.0f, 0.6f, 0.8f, 1.0f);

	// Inicializa todas las escenas
	scene.init();
	titleScreen.init();
	tutorial.init();
	credits.init();


	AudioEngine::getInstance().stopAll();
	AudioEngine::getInstance().play2D("sounds/title_music.mp3", false, 0.4f);

	previousState = state;
}

bool Game::update(int deltaTime)
{
    // Si el estado ha cambiado, actualiza la música
    if (previousState != state) {
        // No cambiar cuando vamos de TITLE a TUTORIAL
        if ((previousState == SCREEN_TITLE && state == SCREEN_TUTORIAL)) {
        }
        else {
            // Para otros cambios de estado, detener música actual
            AudioEngine::getInstance().stopAll();

            // Reproducir música según el nuevo estado
            switch (state) {
            case SCREEN_TITLE:
            case SCREEN_TUTORIAL: // Mismo audio para título y tutorial
                AudioEngine::getInstance().play2D("sounds/title_music.mp3", false, 0.4f);
                break;
            case SCREEN_GAME:
                AudioEngine::getInstance().play2D("sounds/game_music.mp3", true, 0.5f);
                break;
            case SCREEN_CREDITS:
                AudioEngine::getInstance().play2D("sounds/credits_music.mp3", true, 0.5f);
                break;
            }
        }

        // Actualizar estado anterior
        previousState = state;
    }
    if (state == SCREEN_GAME && (scene.getBossDefeatedTimer() > 10000 || scene.isPlayerDead())) {
        state = SCREEN_CREDITS;
    }

    switch (state)
    {
    case SCREEN_TITLE:
        titleScreen.update(deltaTime);
        break;
    case SCREEN_TUTORIAL:
        tutorial.update(deltaTime);
        break;
    case SCREEN_GAME:
        scene.update(deltaTime);
        break;
    case SCREEN_CREDITS:
        credits.update(deltaTime);
        break;
    }

    return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (state)
	{
	case SCREEN_TITLE:
		titleScreen.render();
		break;
	case SCREEN_TUTORIAL:
		tutorial.render();
		break;
	case SCREEN_GAME:
		scene.render();
		break;
	case SCREEN_CREDITS:
		credits.render();
		break;
	}
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	else if (key == GLFW_KEY_SPACE && state == SCREEN_TITLE) 
		state = SCREEN_TUTORIAL;
	else if (key == GLFW_KEY_SPACE && state == SCREEN_TUTORIAL) 
		state = SCREEN_GAME;
	keys[key] = true;

}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}



