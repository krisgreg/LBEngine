#pragma once
#include <SFML/Graphics.hpp>

namespace lbe
{
	class Tile
	{
	private:
		sf::RectangleShape BackgroundSprite;
		int TextureId;
	public:
		Tile(sf::Vector2f WorldPos, sf::Texture *Texture, sf::Vector2f TexturePos, int TextureId);
		~Tile();

		void Draw(sf::RenderWindow &window);
		sf::Vector2f GetWorldPos();
		int GetTextureId();
		bool bContainsPoint(sf::Vector2f Point);
	};
}
