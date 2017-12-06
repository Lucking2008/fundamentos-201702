#pragma once
#include "Agent.h"
#include "InputManager.h"

class Mine :
	public Agent
{
private:
	InputManager* _inputManager;

public:
	Mine(float agent_width,
		float agent_height,
		glm::vec2 position,
		std::string texture);
	Mine(float agent_width,
		float agent_height,
		glm::vec2 position,
		std::string texture,
		InputManager* inputManager);
	void update(float deltaTime);
	~Mine();
};

