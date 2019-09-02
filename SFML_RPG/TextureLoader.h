#pragma once
#include <SFML/Graphics.hpp>

namespace lbe
{
	class TextureLoader
	{
	public:
		static sf::Texture TileTextureMap;
		static sf::Texture SwordTexture;
		static sf::Texture EnemySlimeTexture;
		static sf::Texture ButtonTextures;
		static sf::Texture PlayerBody;
		static sf::Texture PlayerHand;

		TextureLoader();
		~TextureLoader();

		static void InitTextures();
	};
}
