#include "Background.h"
#include "ResourceManager.h"
#include <glm\glm.hpp>


Background::Background(std::string texture): _texture(texture)
{
}

void Background::draw(SpriteBacth& spritebatch) {
	static int textureID = ResourceManager::getTexture("Images/naves/menu.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Color  color;
	color.set(255, 255, 0, 255);
	glm::vec4 destRect(0, 0, 800, 600);
	spritebatch.draw(destRect, uvRect, textureID, 0.0f, color);
}


Background::~Background()
{
}
