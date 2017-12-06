#include "MenuScreen.h"
#include "ScreenIndices.h"
#include "Game.h"
#include <iostream>
#include <iostream>
#include "ResourceManager.h"
#include <random>
#include <ctime>

MenuScreen::MenuScreen(Window* window) :
	_window(window), btnGameClicked(false)
{
	_screenIndex = SCREEN_INDEX_MENU;
}


void MenuScreen::initSystem() {
	_program.compileShaders("Shaders/colorShaderVert.txt",
		"Shaders/colorShaderFrag.txt");
	_program.addAtribute("vertexPosition");
	_program.addAtribute("vertexColor");
	_program.addAtribute("vertexUV");
	_program.linkShader();
}

MenuScreen::~MenuScreen()
{
}

void  MenuScreen::drawHUD() {
	GLuint pLocation =
		_program.getUniformLocation("P");

	glm::mat4 cameraMatrix = _hudCamera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	char buffer[256];

	_hudBatch.begin();

	sprintf_s(buffer, "JUGAR");
	_spriteFont->draw(_hudBatch, buffer, glm::vec2(435, 305),
		glm::vec2(0.5), 0.0f, ColorRGBA(255, 255, 255, 255));

	sprintf_s(buffer, "Busca el tesoro haciendo click en los circulos.");
	_spriteFont->draw(_hudBatch, buffer, glm::vec2(0, 200),
		glm::vec2(0.35), 0.0f, ColorRGBA(255, 255, 255, 255));

	sprintf_s(buffer, "Ten cuidado, hay minas en el camino.");
	_spriteFont->draw(_hudBatch, buffer, glm::vec2(0, 180),
		glm::vec2(0.35), 0.0f, ColorRGBA(255, 255, 255, 255));

	sprintf_s(buffer, "TRABAJO FINAL");
	_spriteFont->draw(_hudBatch, buffer, glm::vec2(0, 50),
		glm::vec2(0.5), 0.0f, ColorRGBA(255, 255, 255, 255));

	sprintf_s(buffer, "LUIS MORON");
	_spriteFont->draw(_hudBatch, buffer, glm::vec2(0, 0),
		glm::vec2(0.5), 0.0f, ColorRGBA(255, 255, 255, 255));

	_hudBatch.end();
	_hudBatch.renderBatch();

}

void MenuScreen::build() 
{
}

void MenuScreen::destroy() {
	delete _background;
	delete _button;
}

void MenuScreen::onExit() {

}

void MenuScreen::onEntry() {
	initSystem();
	_camera2D.init(_window->getScreenWidth(), _window->getScreenHeight());
	_camera2D.setPosition(glm::vec2(_window->getScreenWidth() / 2.0f, _window->getScreenHeight() / 2.0f));
	_spriteBacth.init();
	_hudBatch.init();
	initGUI();
	_background = new Background("Textures/naves/menu.png");
	_button = new Button("Textures/naves/menu_button.png");

	_hudCamera.init(_window->getScreenWidth(),
		_window->getScreenHeight());
	_hudCamera.setPosition(
		glm::vec2(_window->getScreenWidth() / 2.0f,
			_window->getScreenHeight() / 2.0f));

	_spriteFont = new SpriteFont("Fonts/arial.ttf", 64);
}

void MenuScreen::update() {
	_camera2D.update();
	_hudCamera.update();
	if (_game->_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		if (_button->clicked(_game->_inputManager.getMouseCoords()) && !btnGameClicked) {
			btnGameClicked = true;
			std::cout << "Button clicked" << std::endl;
			_currentState = ScreenState::CHANGE_NEXT;

		}
	}
	checkInput();
}


void MenuScreen::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_program.use();
	glActiveTexture(GL_TEXTURE0);

	GLuint pLocation =
		_program.getUniformLocation("P");

	glm::mat4 cameraMatrix = _camera2D.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	GLuint imageLocation = _program.getUniformLocation("myImage");
	glUniform1i(imageLocation, 0);

	_spriteBacth.begin();
	_background->draw(_spriteBacth);
	_button->draw(_spriteBacth);

	_spriteBacth.end();
	_spriteBacth.renderBatch();

	drawHUD();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	_program.unuse();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int MenuScreen::getNextScreen()const {
	return SCREEN_INDEX_GAMEPLAY;
}

int MenuScreen::getPreviousScreen() const{
	return SCREEN_INDEX_NO_SCREEN;
}

void MenuScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		_game->onSDLEvent(evnt);
	}
}


