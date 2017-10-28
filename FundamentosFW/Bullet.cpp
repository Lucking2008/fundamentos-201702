#include "Bullet.h"
#include "ResourceManager.h"
#include "GLTexture.h"

#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"


void Bullet::draw(SpriteBacth& spriteBatch) {
	glm::vec4 destRect(
		_position.x, _position.y,
		BULLET_RADIUS * 2, BULLET_RADIUS * 2);

	const glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Color color;
	color.r = 75;
	color.g = 75;
	color.b = 75;
	color.a = 255;
	static GLTexture texture = ResourceManager::getTexture("Textures/Paper_Mario_.png");
	spriteBatch.draw(destRect, uv, texture.id, 0.0f, color);
}

bool Bullet::collideWithWorld(
	const std::vector<std::string>& levelData) {
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
		return true;
	}
	return (levelData[gridPosition.y][gridPosition.x] != '.');
}

bool Bullet::update(const std::vector<std::string>& levelData,
	float deltaTime) {
	_position += _direction * _speed * deltaTime;
	return false;
}

bool Bullet::collideWithAgent(Agent* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;
	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPosA - centerPosB;
	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0) {
		return true;
	}
	return false;
}

Bullet::Bullet(glm::vec2 position, glm::vec2 direction,
	float damage, float speed) : 
	_position(position),
	_direction(direction),
	_damage(damage),
	_speed(speed)
{
}


Bullet::~Bullet()
{
}
