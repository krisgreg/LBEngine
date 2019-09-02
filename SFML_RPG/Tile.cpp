#include "Tile.h"
#include "Consts.h"

lbe::Tile::Tile(sf::Vector2f WorldPos, sf::Texture *Texture, sf::Vector2f TexturePos, int TextureId) : TextureId(TextureId)
{
	BackgroundSprite.setSize(sf::Vector2f(lbe::TILE_SIZE, lbe::TILE_SIZE));
	BackgroundSprite.setTexture(Texture);
	BackgroundSprite.setTextureRect(sf::IntRect(TexturePos.x, TexturePos.y, lbe::TILE_TEXTURE_SIZE, lbe::TILE_TEXTURE_SIZE));
	BackgroundSprite.setPosition(WorldPos);
}

lbe::Tile::~Tile()
{
}

void lbe::Tile::Draw(sf::RenderWindow & window)
{
	window.draw(BackgroundSprite);
}

sf::Vector2f lbe::Tile::GetWorldPos()
{
	return BackgroundSprite.getPosition();
}

int lbe::Tile::GetTextureId()
{
	return TextureId;
}

bool lbe::Tile::bContainsPoint(sf::Vector2f Point)
{
	return BackgroundSprite.getGlobalBounds().contains(Point);
}
