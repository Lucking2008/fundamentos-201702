#include "Treasure.h"



Treasure::Treasure(float agent_width,
	float agent_height,
	glm::vec2 position,
	std::string texture) : Agent(agent_width, agent_height, position, texture)
{

}

Treasure::Treasure(float agent_width,
	float agent_height,
	glm::vec2 position,
	std::string texture,
	InputManager* inputManager) : Agent(agent_width, agent_height, position, texture)
{
	_inputManager = inputManager;
}

Treasure::~Treasure()
{

}

void Treasure::update(float deltaTime)
{

}
