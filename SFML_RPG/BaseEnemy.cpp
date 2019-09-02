#include "BaseEnemy.h"

#include <iostream>

#include "HelperFunctions.h"
#include "Consts.h"
#include "NetworkManager.h"
#include "LevelManager.h"
#include "Player.h"

lbe::RegisterAsSceneObject<lbe::BaseEnemy> lbe::BaseEnemy::RegisterSceneObject("BaseEnemy");

lbe::BaseEnemy::BaseEnemy(sf::Vector2f Pos) : SceneObjectPlaceableInEditor(Pos), SpawnPosition(Pos)
{
	bIsPlaceableInEditor = false;
	Name = "BaseEnemy";

	Body.setFillColor(COLOR_DEEP_RED);
	Body.setOutlineColor(COLOR_BRIGHT_RED);
	Body.setOutlineThickness(lbe::PLAYER_OUTLINE_THICKNESS);
	Body.setRadius(lbe::PLAYER_INNER_RADIUS);
	Body.setOrigin(Body.getRadius(), Body.getRadius());

	Body.setPosition(Pos);
}


lbe::BaseEnemy::~BaseEnemy()
{
}

bool lbe::BaseEnemy::ApplyUpdatePacket(sf::Packet& packet)
{
	bool success = true;
	if (!(packet >> Position.x))
	{
		success = false;
		LBE_WARN("UpdatePacket failed to apply to BaseEnemy!");
	}
	if (!(packet >> Position.y))
	{
		LBE_WARN("UpdatePacket failed to apply to BaseEnemy!");
		success = false;
	}
	//LBE_TRACE("NewBaseEnemyPosition: x={0}, y={1}", Position.x, Position.y);
	return success;
}

bool lbe::BaseEnemy::WriteUpdatePacket(sf::Packet & packet)
{
	packet << Position.x << Position.y;

	return true;
}

void lbe::BaseEnemy::Draw(sf::RenderWindow &window)
{
	if (bHasSprite)
	{
		//window.draw(Body);
		window.draw(Sprite);
	}
	else
	{
		window.draw(Body);
	}
}

void lbe::BaseEnemy::Move(sf::Vector2f MovementInput, sf::Time dt)
{
	Body.move(MovementInput * dt.asSeconds());
	if (bHasSprite)
	{
		Sprite.setPosition(GetPos());
	}
	Position = Body.getPosition();
}

sf::Vector2f lbe::BaseEnemy::GetPos()
{
	return Body.getPosition();
}

void lbe::BaseEnemy::SetPos(sf::Vector2f NewPos)
{
	Position = NewPos;
	Body.setPosition(NewPos);
	if (bHasSprite)
	{
		Sprite.setPosition(Body.getPosition());
	}
}

void lbe::BaseEnemy::UpdateRotation(sf::Vector2f Point)
{
	if (!(GetPos() == Point))
	{
		//LBE_TRACE("Point used for enemy UpdateRotation:x={0:03.2f} y={1:03.2f}", Point.x, Point.y);
		ForwardVector = (Point - GetPos()) / (sqrt(pow(abs(Point.x - GetPos().x), 2) + pow(abs(Point.y - GetPos().y), 2)));
		//LBE_TRACE("New enemy ForwardVector:x={0:03.2f} y={1:03.2f}", ForwardVector.x, ForwardVector.y);
	}
}

double lbe::BaseEnemy::GetRotation()
{
	sf::Vector2f ZeroDegreeVector(0, -1);
	double Dot = ForwardVector.x * ZeroDegreeVector.x + ForwardVector.y * ZeroDegreeVector.y;
	double Determinant = ZeroDegreeVector.x * ForwardVector.y - ZeroDegreeVector.y * ForwardVector.x;
	double Angle = atan2(Determinant, Dot);
	Angle = (Angle / lbe::PI) * 360 / 2;
	//std::cout << "Angle=" << Angle << std::endl;
	return Angle;
}

double lbe::BaseEnemy::GetRadius()
{
	return Body.getRadius();
}

void lbe::BaseEnemy::SetColors(sf::Color FillColor, sf::Color OutlineColor)
{
	Body.setFillColor(FillColor);
	Body.setOutlineColor(OutlineColor);
}

void lbe::BaseEnemy::Update(sf::Time dt)
{
	if (NetworkManager::GetActiveNetworkState() == NetworkState::Host)
	{
		//LBE_TRACE("Enemy update starting! loc:x={0:03.2f} y={1:03.2f}", GetPos().x, GetPos().y);
		UpdateRotation(Destination);
		if (lbe::HelperFunctions::GetDistance(Destination, Position) > DestinationAcceptanceDistance)
		{
			Move(ForwardVector * (float)MovementSpeed, dt);
		}
		//LBE_TRACE("Enemy updated! loc:x={0:03.2f} y={1:03.2f}", GetPos().x, GetPos().y);
	}
	else
	{
		SetPos(Position);
	}
}

void lbe::BaseEnemy::SetSprite(sf::Texture * Texture, sf::Vector2f TextureSize, sf::Vector2f TexturePos)
{
	Sprite.setSize(sf::Vector2f(lbe::PLAYER_INNER_RADIUS * 2, lbe::PLAYER_INNER_RADIUS * 2));
	Sprite.setTexture(Texture);
	Sprite.setTextureRect(sf::IntRect(TexturePos.x, TexturePos.y, TextureSize.x, TextureSize.y));
	Sprite.setOrigin(lbe::PLAYER_INNER_RADIUS, lbe::PLAYER_INNER_RADIUS);
	Sprite.setPosition(Body.getPosition());
	bHasSprite = true;
}

void lbe::BaseEnemy::ReceiveDamage(double DamageToTake)
{
	HitPoints -= DamageToTake;
	if (HitPoints <= 0)
	{
		bIsDead = true;
		MarkForDeletion();
	}
}

bool lbe::BaseEnemy::IsDead()
{
	return bIsDead;
}

void lbe::BaseEnemy::SetDestination(sf::Vector2f NewDestination)
{
	Destination = NewDestination;
}

void lbe::BaseEnemy::UpdateAI()
{
	std::vector<SceneObject*> SceneObjects = LevelManager::GetSceneObjectsFromLevel(Position);
	std::vector<lbe::Player*> Players;
	for (auto &Object : SceneObjects)
	{
		if (lbe::Player* player = dynamic_cast<lbe::Player*>(Object))
		{
			Players.push_back(player);
		}
	}

	bool bTargetFound = false;
	double CurrentClosest = AggroRange;
	for (auto &player : Players)
	{
		if (lbe::HelperFunctions::GetDistance(Position, player->GetPosition()) < CurrentClosest)
		{
			Destination = player->GetPosition();
			CurrentClosest = lbe::HelperFunctions::GetDistance(Position, player->GetPosition());
			bTargetFound = true;
		}
	}
	
	if (!bTargetFound)
	{
		Destination = SpawnPosition;
	}
}

std::string lbe::BaseEnemy::GetSaveString()
{
	return std::string();
}

sf::Texture * lbe::BaseEnemy::GetObjectIcon()
{
	return nullptr;
}

sf::IntRect lbe::BaseEnemy::GetObjectIconRect()
{
	return sf::IntRect();
}

bool lbe::BaseEnemy::IsPointInHitbox(sf::Vector2f Point)
{
	return lbe::HelperFunctions::GetDistance(Position, Point) < Body.getRadius();
}