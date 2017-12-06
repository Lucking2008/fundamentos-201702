#include "Mine.h"



Mine::Mine(float agent_width,
	float agent_height,
	glm::vec2 position,
	std::string texture) : Agent(agent_width, agent_height, position, texture)
{

}

Mine::Mine(float agent_width,
	float agent_height,
	glm::vec2 position,
	std::string texture,
	InputManager* inputManager) : Agent(agent_width, agent_height, position, texture)
{
	_inputManager = inputManager;
}

Mine::~Mine()
{

}

void Mine::update(float deltaTime)
{

}
