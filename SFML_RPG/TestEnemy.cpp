#include "TestEnemy.h"

#include <iostream>
#include <sstream>

#include "TextureLoader.h"
#include "Consts.h"
#include "HelperFunctions.h"

lbe::RegisterAsSceneObject<lbe::TestEnemy> lbe::TestEnemy::RegisterSceneObject("TestEnemy");

lbe::TestEnemy::TestEnemy(sf::Vector2f Pos) : BaseEnemy(Pos)
{
	bIsPlaceableInEditor = true;
	Name = "TestEnemy";

	Body.setFillColor(COLOR_DEEP_RED);
	Body.setOutlineColor(COLOR_BRIGHT_RED);

	SetSprite(&lbe::TextureLoader::EnemySlimeTexture, sf::Vector2f(32, 32), sf::Vector2f(0, 0));

	MovementSpeed = 40.f;
}


lbe::TestEnemy::~TestEnemy()
{
}

sf::Texture* lbe::TestEnemy::GetObjectIcon()
{
	return &lbe::TextureLoader::EnemySlimeTexture;
}

sf::IntRect lbe::TestEnemy::GetObjectIconRect()
{
	return sf::IntRect(0,0,32,32);
}

std::string lbe::TestEnemy::GetSaveString()
{
	std::stringstream OutputStream;
	OutputStream << "TestEnemy " << Position.x << " " << Position.y;
	return OutputStream.str();
}

bool lbe::TestEnemy::IsPointInHitbox(sf::Vector2f Point)
{
	return lbe::HelperFunctions::GetDistance(Position, Point) < Body.getRadius();
}
