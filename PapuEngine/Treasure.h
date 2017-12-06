#pragma once
#include "Agent.h"
#include "InputManager.h"

class Treasure :
	public Agent
{
private:
	InputManager* _inputManager;

public:
	Treasure(float agent_width,
		float agent_height,
		glm::vec2 position,
		std::string texture);
	Treasure(float agent_width,
		float agent_height,
		glm::vec2 position,
		std::string texture,
		InputManager* inputManager);
	void update(float deltaTime);
	~Treasure();
};

