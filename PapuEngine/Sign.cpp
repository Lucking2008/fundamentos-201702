#include "Sign.h"



Sign::Sign(float agent_width,
	float agent_height,
	glm::vec2 position,
	std::string texture) : Agent(agent_width, agent_height, position, texture)
{

}

Sign::Sign(float agent_width,
	float agent_height,
	glm::vec2 position,
	std::string texture,
	InputManager* inputManager) : Agent(agent_width, agent_height, position, texture)
{
	_inputManager = inputManager;
}

Sign::~Sign()
{

}

void Sign::update(float deltaTime)
{

}
