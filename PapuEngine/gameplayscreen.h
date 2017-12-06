#pragma once

#include <SDL\SDL.h>
#include "IGameScreen.h"
#include "Camera2D.h"
#include "GLS_Program.h"
#include "Window.h"
#include "GLTexture.h"
#include "SpriteBacth.h"
#include <vector>
#include "SpriteFont.h"
#include "Background.h"
#include "Ship.h"
#include "EnemyShip.h"
#include "Vullet.h"

#include "Sign.h"
#include "Mine.h"
#include "Treasure.h"

class GamePlayScreen : public IGameScreen
{
private:
	GLS_Program _program;
	Camera2D _camera2d;
	Background* _background;
	Window* _window = nullptr;
	SpriteBacth _spriteBatch;
	GLTexture _texture;
	SpriteFont* _spriteFont;
	Camera2D _hudCamera;
	SpriteBacth _hudBatch;
	float _elapsed;
	int _score;
	int _bullet;
	void drawHUD();

	bool _flagClick;
	int _boardX;
	int _boardY;
	int _mineSize;
	int _mineSpace;
	int _boardSize;
	int _state;
	vector<vector<Sign*>> _signs;
	vector<vector<Mine*>> _mines;
	vector<vector<int>> _table;
	vector<vector<bool>> _revealed;
	int _treasureI;
	int _treasureJ;
	Treasure* _treasure;

public:
	GamePlayScreen(Window* window);
	~GamePlayScreen();
	bool onExitClicked();
	void initWorld();
	virtual void build() override;
	virtual void destroy() override;
	virtual void onExit() override;
	virtual void onEntry() override;
	
	virtual void initGUI()override;
	virtual void draw()override;
	virtual void update()override;
	virtual void initSystem() override;
	virtual int getNextScreen() const override;
	virtual int getPreviousScreen() const override;
	virtual void checkInput() override;

private:
	void createBoard(int clickPosI, int clickPosJ);
	int getMinesAround(int i, int j);
};

