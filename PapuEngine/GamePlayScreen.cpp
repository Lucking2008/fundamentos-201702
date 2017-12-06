#include "GamePlayScreen.h"
#include "Game.h"
#include "ScreenIndices.h"
#include <iostream>
#include "ResourceManager.h"
#include <random>
#include <ctime>


bool GamePlayScreen::onExitClicked()
{
	_currentState = ScreenState::EXIT_APPLICATION;
	return true;
}

GamePlayScreen::GamePlayScreen(Window* window):
	_window(window),_bullet(0),_score(0)
{
	_screenIndex = SCREEN_INDEX_GAMEPLAY;
}

GamePlayScreen::~GamePlayScreen()
{
}

void GamePlayScreen::build() {

}

void GamePlayScreen::destroy() {

}

void GamePlayScreen::onExit() {
}

void GamePlayScreen::onEntry() {
	initWorld();
	initSystem();
	_spriteBatch.init();
	_hudBatch.init();
	initGUI();
	_camera2d.init(_window->getScreenWidth(),
		_window->getScreenHeight());
	_camera2d.setPosition(
		glm::vec2(_window->getScreenWidth() / 2.0f,
			_window->getScreenHeight() / 2.0f));
	
	_background = new Background("Textures/naves/game.png");

	_hudCamera.init(_window->getScreenWidth(),
		_window->getScreenHeight());
	_hudCamera.setPosition(
		glm::vec2(_window->getScreenWidth() / 2.0f,
				_window->getScreenHeight() / 2.0f));

	_spriteFont = new SpriteFont("Fonts/arial.ttf",64);

	initWorld();

}

void GamePlayScreen::initWorld() {
	if (_signs.size() > 0)
	{
		for (size_t i = 0; i < _signs.size(); i++)
		{
			for (size_t j = 0; j < _signs[i].size(); j++)
			{
				if (_signs[i][j] != nullptr)
				{
					delete _signs[i][j];
					_signs[i][j] = nullptr;
				}

				if (_mines.size() > 0)
				{
					if (_mines[i][j] != nullptr)
					{
						delete _mines[i][j];
						_mines[i][j] = nullptr;
					}
				}
			}
		}
		_signs.clear();
		_mines.clear();

		if (_treasure != nullptr)
		{
			delete _treasure;
			_treasure = nullptr;
		}
	}


	_flagClick = false;
	_boardX = 150;
	_boardY = 15;
	_mineSize = 50;
	_mineSpace = 10;
	_boardSize = 8;
	_revealed = vector<vector<bool>>(_boardSize, vector<bool>(_boardSize, false));
	_signs = vector<vector<Sign*>>(_boardSize, vector<Sign*>(_boardSize));
	_state = 0;

	int posX = _boardX;
	int posY = _boardY;

	for (size_t i = 0; i < _boardSize; i++)
	{
		for (size_t j = 0; j < _boardSize; j++)
		{
			_signs[i][j] = new Sign(_mineSize, _mineSize, glm::vec2(posX, posY), "Assets/sign.png");
			posX += _mineSize + _mineSpace;
		}

		posX = _boardX;
		posY += _mineSize + _mineSpace;
	}
}

void GamePlayScreen::initGUI()
{

}

void GamePlayScreen::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_program.use();
	GLuint pLocation =
		_program.getUniformLocation("P");

	glm::mat4 cameraMatrix = _camera2d.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	GLuint imageLocation = _program.getUniformLocation("myImage");
	glUniform1i(imageLocation, 0);
	_spriteBatch.begin();

	_background->draw(_spriteBatch);

	for (size_t i = 0; i < _signs.size(); i++)
		for(size_t j = 0; j < _signs.size(); j++)
			if(!_revealed[i][j])
				_signs[i][j]->draw(_spriteBatch);

	for (size_t i = 0; i < _mines.size(); i++)
		for (size_t j = 0; j < _mines.size(); j++)
			if (_mines[i][j] != nullptr && _revealed[i][j])
				_mines[i][j]->draw(_spriteBatch);

	if (_treasure != nullptr && _revealed[_treasureI][_treasureJ])
		_treasure->draw(_spriteBatch);

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	drawHUD();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	_program.unuse();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void GamePlayScreen::update() {
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / _game->getFps();
	float previuosTime = SDL_GetTicks();
	float newTime = SDL_GetTicks();
	float frameTime = newTime - previuosTime;
	previuosTime = newTime;
	float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

	_camera2d.update();
	_hudCamera.update();
	checkInput();
	
	if (_state != 0)
	{
		_elapsed += 0.1f;
	}

	if (_state != 0 && _elapsed >= 10.0f) {
		initWorld();
		_elapsed = 0.0f;
	}

	if (_game->_inputManager.isKeyDown(SDLK_f)) {
		
	}

	if (_state == 0)
	{
		if (_game->_inputManager.isKeyDown(SDL_BUTTON_LEFT))
		{
			glm::vec2 mouseCoords = _game->_inputManager.getMouseCoords();

			for (size_t i = 0; i < _signs.size(); i++)
			{
				for (size_t j = 0; j < _signs[i].size(); j++)
				{
					if (_signs[i][j] != nullptr)
					{
						glm::vec2 signCoords = _signs[i][j]->getPosition();
						if (mouseCoords.x >= signCoords.x && mouseCoords.x <= signCoords.x + _mineSize &&
							_window->getScreenHeight() - mouseCoords.y >= signCoords.y && _window->getScreenHeight() - mouseCoords.y <= signCoords.y + _mineSize)	//Si esta haciendo click
						{
							if (!_revealed[i][j])
							{
								if (!_flagClick)
								{
									_flagClick = true;
									createBoard(i, j);
								}

								_revealed[i][j] = true;
								if (_mines[i][j] != nullptr) //Perdiste
								{
									_state = -1;
								}

								if (i == _treasureI && j == _treasureJ) //Ganaste
								{
									_state = 1;
								}

							}
						}

					}
				}
			}

		}
	}

}

void  GamePlayScreen::drawHUD() {
	GLuint pLocation =
		_program.getUniformLocation("P");

	glm::mat4 cameraMatrix = _hudCamera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	char buffer[256];

	_hudBatch.begin();

	for (size_t i = 0; i < _signs.size(); i++)
	{
		for (size_t j = 0; j < _signs[i].size(); j++)
		{
			if (_signs[i][j] != nullptr)
			{
				if (_flagClick && _revealed[i][j])
				{
					int num = getMinesAround(i, j);
					sprintf_s(buffer, "%d", num);
					_spriteFont->draw(_hudBatch, buffer, _signs[i][j]->getPosition() + glm::vec2(_mineSize / 3, 0.0f),
						glm::vec2(0.5), 0.0f, ColorRGBA(255, 255, 255, 255));
				}

			}
		}
	}

	if (_state == -1)
	{
		sprintf_s(buffer, "PERDISTE");
		_spriteFont->draw(_hudBatch, buffer, glm::vec2(0, 0),
			glm::vec2(0.5), 0.0f, ColorRGBA(255, 255, 255, 255));
	}
	if (_state == 1)
	{
		sprintf_s(buffer, "GANASTE");
		_spriteFont->draw(_hudBatch, buffer, glm::vec2(0, 0),
			glm::vec2(0.5), 0.0f, ColorRGBA(255, 255, 255, 255));
	}

	_hudBatch.end();
	_hudBatch.renderBatch();

}

void GamePlayScreen::initSystem()
{
	_program.compileShaders("Shaders/colorShaderVert.txt",
		"Shaders/colorShaderFrag.txt");
	_program.addAtribute("vertexPosition");
	_program.addAtribute("vertexColor");
	_program.addAtribute("vertexUV");
	_program.linkShader();
}


void GamePlayScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		_game->onSDLEvent(evnt);
	}
}

void GamePlayScreen::createBoard(int clickPosI, int clickPosJ)
{
	for (size_t i = 0; i < _mines.size(); i++)
	{
		for (size_t j = 0; j < _mines[i].size(); j++)
		{
			if (_mines[i][j] != nullptr)
			{
				delete _mines[i][j];
				_mines[i][j] = nullptr;
			}
		}
	}
	_mines.clear();

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> rand(0, _boardSize - 1);

	_mines = vector<vector<Mine*>>(_boardSize, vector<Mine*>(_boardSize));
	for (size_t i = 0; i < 15; i++)
	{
		do
		{
			int randI = rand(randomEngine);
			int randJ = rand(randomEngine);

			//Si no hay mina en esa posicion del tablero y el primer click no fue hecho ahi, crear la mina
			if (_mines[randI][randJ] == nullptr && randI != clickPosI && randJ != clickPosJ)
			{
				_mines[randI][randJ] = new Mine(_mineSize, _mineSize, glm::vec2(_boardX + randJ * (_mineSize + _mineSpace), _boardY + randI * (_mineSize + _mineSpace)), "Assets/mine.png");
				break; //salir del do-while
			}

		} while (true);
	}

	//Tesoro
	do
	{
		int randI = rand(randomEngine);
		int randJ = rand(randomEngine);

		//Si no hay mina en esa posicion del tablero y el primer click no fue hecho ahi, crear el tesoro
		if (_mines[randI][randJ] == nullptr && randI != clickPosI && randJ != clickPosJ)
		{
			_treasure = new Treasure(_mineSize - 10, _mineSize - 10, glm::vec2(_boardX + randJ * (_mineSize + _mineSpace), _boardY + randI * (_mineSize + _mineSpace)), "Assets/treasure.png");
			_treasureI = randI;
			_treasureJ = randJ;
			break; //salir del do-while
		}

	} while (true);


}

int GamePlayScreen::getMinesAround(int i, int j)
{
	int counter = 0;
	int di[9] = { 0, 1, 1, 1, 0, -1, -1, -1, 0 };
	int dj[9] = { 1, 1, 0, -1, -1, -1, 0, 1, 0 };

	for (size_t k = 0; k < 9; k++)
	{
		int posI = i + di[k];
		int posJ = j + dj[k];

		if (posI >= 0 && posI < _boardSize && posJ >= 0 && posJ < _boardSize)
		{
			if (_mines[posI][posJ] != nullptr)
				counter++;
		}
	}

	return counter;
}

int GamePlayScreen::getNextScreen() const {
	return SCREEN_INDEX_NO_SCREEN;
};

int GamePlayScreen::getPreviousScreen() const {
	return SCREEN_INDEX_MENU;
}
