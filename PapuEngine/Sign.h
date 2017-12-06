#pragma once
#include "Agent.h"
#include "InputManager.h"

class Sign :
	public Agent
{
private:
	InputManager* _inputManager;

public:
	Sign(float agent_width,
		float agent_height,
		glm::vec2 position,
		std::string texture);
	Sign(float agent_width,
		float agent_height,
		glm::vec2 position,
		std::string texture,
		InputManager* inputManager);
	void update(float deltaTime);
	~Sign();
};

