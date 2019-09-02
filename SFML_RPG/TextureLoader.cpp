#include "TextureLoader.h"
#include "Consts.h"
#include <iostream>

sf::Texture lbe::TextureLoader::TileTextureMap;
sf::Texture lbe::TextureLoader::SwordTexture;
sf::Texture lbe::TextureLoader::EnemySlimeTexture;
sf::Texture lbe::TextureLoader::ButtonTextures;
sf::Texture lbe::TextureLoader::PlayerBody;
sf::Texture lbe::TextureLoader::PlayerHand;

lbe::TextureLoader::TextureLoader()
{
}


lbe::TextureLoader::~TextureLoader()
{
}

void lbe::TextureLoader::InitTextures()
{
	if (!TileTextureMap.loadFromFile(lbe::PATH_TEXTURE_MAP))
	{
		LBE_WARN("Could not load texture file from {0}", lbe::PATH_TEXTURE_MAP);
	}

	if (!SwordTexture.loadFromFile(lbe::PATH_PLAYER_WEAPON))
	{
		LBE_WARN("Could not load texture file from {0}", lbe::PATH_TEXTURE_MAP);
	}

	if (!EnemySlimeTexture.loadFromFile(lbe::PATH_TEST_ENEMY_SPRITE))
	{
		LBE_WARN("Could not load texture file from {0}", lbe::PATH_TEST_ENEMY_SPRITE);
	}

	if (!ButtonTextures.loadFromFile(lbe::PATH_BUTTON_SPRITES))
	{
		LBE_WARN("Could not load texture file from {0}", lbe::PATH_BUTTON_SPRITES);
	}

	if (!PlayerBody.loadFromFile(lbe::PATH_PLAYER_BODY_SPRITE))
	{
		LBE_WARN("Could not load texture file from {0}", lbe::PATH_PLAYER_BODY_SPRITE);
	}

	if (!PlayerHand.loadFromFile(lbe::PATH_PLAYER_HAND_SPRITE))
	{
		LBE_WARN("Could not load texture file from {0}", lbe::PATH_PLAYER_HAND_SPRITE);
	}
}
