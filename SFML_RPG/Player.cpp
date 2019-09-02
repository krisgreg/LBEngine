#include "Player.h"

#include <iostream>

#include "Consts.h"
#include "HelperFunctions.h"
#include "TextureLoader.h"
#include "DefaultSwordSlashHitbox.h"
#include "LevelManager.h"
#include "GameState.h"
#include "NetworkManager.h"

lbe::RegisterAsSceneObject<lbe::Player> lbe::Player::RegisterSceneObject("Player");

lbe::Player::Player(sf::Vector2f Pos) : SceneObject(Pos)
{
	Name = "Player";

	Body.setFillColor(SKIN_TONE_TAN);
	Body.setOutlineColor(SKIN_TONE_YELLOW);
	Body.setOutlineThickness(lbe::PLAYER_OUTLINE_THICKNESS);
	Body.setRadius(lbe::PLAYER_INNER_RADIUS);
	Body.setOrigin(Body.getRadius(), Body.getRadius());
	Body.setPosition(Pos);

	PlayerView.setCenter(sf::Vector2f(Body.getPosition()));
	PlayerView.setSize(SCREEN_SIZE);

	LeftHand.setFillColor(SKIN_TONE_TAN);
	LeftHand.setOutlineColor(SKIN_TONE_YELLOW);
	LeftHand.setOutlineThickness(lbe::PLAYER_OUTLINE_HAND_THICKNESS);
	LeftHand.setRadius(lbe::PLAYER_INNER_HAND_RADIUS);
	LeftHand.setOrigin(LeftHand.getRadius(), LeftHand.getRadius());
	LeftHand.setPosition(Pos.x + lbe::PLAYER_ARM_DISTANCE, Pos.y);

	RightHand.setFillColor(SKIN_TONE_TAN);
	RightHand.setOutlineColor(SKIN_TONE_YELLOW);
	RightHand.setOutlineThickness(lbe::PLAYER_OUTLINE_HAND_THICKNESS);
	RightHand.setRadius(lbe::PLAYER_INNER_HAND_RADIUS);
	RightHand.setOrigin(RightHand.getRadius(), RightHand.getRadius());
	RightHand.setPosition(Pos.x - lbe::PLAYER_ARM_DISTANCE, Pos.y);

	Weapon.setSize(sf::Vector2f(60, 150));
	Weapon.setTexture(&lbe::TextureLoader::SwordTexture);
	Weapon.setTextureRect(sf::IntRect(0, 0, 400, 1000));
	UpdateWeapon();

	SlashAnimation.AddPoint(sf::Vector2f(0, 0));
	SlashAnimation.AddPoint(sf::Vector2f(50, 50));
	SlashAnimation.AddPoint(sf::Vector2f(100, 0));

	if (USING_PLAYER_SPRITE)
	{
		BodySprite.setSize(sf::Vector2f(lbe::PLAYER_RADIUS * 1.3f, lbe::PLAYER_RADIUS * 2.6f));
		BodySprite.setTexture(&lbe::TextureLoader::PlayerBody);
		BodySprite.setOrigin(BodySprite.getLocalBounds().width / 2.f, BodySprite.getLocalBounds().height / 2.f);
		BodySprite.setPosition(Position);

		LeftHandSprite.setSize(sf::Vector2f(lbe::PLAYER_HAND_RADIUS * 2.f, lbe::PLAYER_HAND_RADIUS * 2.f));
		LeftHandSprite.setTexture(&lbe::TextureLoader::PlayerHand);
		LeftHandSprite.setOrigin(LeftHandSprite.getLocalBounds().width / 2.f, LeftHandSprite.getLocalBounds().height / 2.f);
		LeftHandSprite.setPosition(GetLeftHandPos());

		RightHandSprite.setSize(sf::Vector2f(lbe::PLAYER_HAND_RADIUS * 2.f, lbe::PLAYER_HAND_RADIUS * 2.f));
		RightHandSprite.setTexture(&lbe::TextureLoader::PlayerHand);
		RightHandSprite.setOrigin(RightHandSprite.getLocalBounds().width / 2.f, RightHandSprite.getLocalBounds().height / 2.f);
		RightHandSprite.setPosition(GetRightHandPos());
	}
}


lbe::Player::~Player()
{
}

bool lbe::Player::ApplyUpdatePacket(sf::Packet& packet)
{
	sf::Vector2f PlayerPos;
	sf::Vector2f LeftHandPos;
	sf::Vector2f RightHandPos;
	sf::Vector2f NewForwardVector;
	bool success = packet >> PlayerPos.x >> PlayerPos.y >> LeftHandPos.x >> LeftHandPos.y >> RightHandPos.x >> RightHandPos.y >> NewForwardVector.x >> NewForwardVector.y;
	if (GameState::LocalPlayer->GetUniqueID() == GetUniqueID())
	{
		return false;
	}
	SetPos(PlayerPos);
	SetForwardVector(NewForwardVector);
	SetHandPos(LeftHandPos, RightHandPos);
	return success;
}

bool lbe::Player::WriteUpdatePacket(sf::Packet& packet)
{
	return packet << Position.x << Position.y << LeftHand.getPosition().x << LeftHand.getPosition().y << RightHand.getPosition().x << RightHand.getPosition().y << ForwardVector.x << ForwardVector.y;
}

void lbe::Player::Draw(sf::RenderWindow &window)
{
	window.draw(Weapon);
	if (USING_PLAYER_SPRITE)
	{
		window.draw(LeftHandSprite);
		window.draw(RightHandSprite);
		window.draw(BodySprite);
	}
	else
	{
		window.draw(LeftHand);
		window.draw(RightHand);
		window.draw(Body);
	}

	if (DEBUG_PLAYER_HANDS)
	{
		window.draw(ForwardVectorLine, 2, sf::Lines);
		window.draw(ArmPositionLine, 4, sf::Lines);
	}
}

void lbe::Player::Move(sf::Vector2f MovementInput, sf::Time dt)
{
	Body.move(MovementInput * dt.asSeconds());
	LeftHand.move(MovementInput * dt.asSeconds());
	RightHand.move(MovementInput * dt.asSeconds());
	Position = Body.getPosition();
	//PlayerView.setCenter(sf::Vector2f(Body.getPosition().x, Body.getPosition().y));
	DesiredLeftHandPos = Body.getPosition() + lbe::HelperFunctions::RotateVector(ForwardVector, 270.f) * lbe::PLAYER_ARM_DISTANCE;
	DesiredRightHandPos = Body.getPosition() + lbe::HelperFunctions::RotateVector(ForwardVector, 90.f) * lbe::PLAYER_ARM_DISTANCE;

	LeftHand.setPosition(LeftHand.getPosition() * (1.f - dt.asSeconds() * HandFollowSpeed) + DesiredLeftHandPos * (dt.asSeconds() * HandFollowSpeed));
	RightHand.setPosition(RightHand.getPosition() * (1.f - dt.asSeconds() * HandFollowSpeed) + DesiredRightHandPos * (dt.asSeconds() * HandFollowSpeed));
	LeftHandSprite.setPosition(GetLeftHandPos());
	RightHandSprite.setPosition(GetRightHandPos());

	if (DEBUG_PLAYER_HANDS)
	{
		ForwardVectorLine[0] = sf::Vertex(Body.getPosition(), sf::Color::Red);
		ForwardVectorLine[1] = sf::Vertex(Body.getPosition() + ForwardVector * 100.f, sf::Color::Red);
		ArmPositionLine[0] = sf::Vertex(Body.getPosition(), sf::Color::Blue);
		ArmPositionLine[1] = sf::Vertex(Body.getPosition() + lbe::HelperFunctions::RotateVector(ForwardVector, 270.f) * lbe::PLAYER_ARM_DISTANCE, sf::Color::Blue);
		ArmPositionLine[2] = sf::Vertex(Body.getPosition(), sf::Color::Blue);
		ArmPositionLine[3] = sf::Vertex(Body.getPosition() + lbe::HelperFunctions::RotateVector(ForwardVector, 90.f) * lbe::PLAYER_ARM_DISTANCE, sf::Color::Blue);
	}

	if (bIsPerformingAttack)
	{
		SlashAnimation.ClearPoints();
		if (SlashAnimation.UpdateAnimationState(dt * 0.75f))
		{
			if (!bIsPerformingReverseAttack)
			{
				SlashAnimation.AddPoint(Body.getPosition() + lbe::HelperFunctions::RotateVector(ForwardVector, 60.f) * lbe::PLAYER_ARM_DISTANCE);
				SlashAnimation.AddPoint(Body.getPosition() + ForwardVector * lbe::PLAYER_ARM_DISTANCE);
				SlashAnimation.AddPoint(Body.getPosition() + lbe::HelperFunctions::RotateVector(ForwardVector, 300.f) * lbe::PLAYER_ARM_DISTANCE);
			}
			else
			{
				SlashAnimation.AddPoint(Body.getPosition() + lbe::HelperFunctions::RotateVector(ForwardVector, 300.f) * lbe::PLAYER_ARM_DISTANCE);
				SlashAnimation.AddPoint(Body.getPosition() + ForwardVector * lbe::PLAYER_ARM_DISTANCE);
				SlashAnimation.AddPoint(Body.getPosition() + lbe::HelperFunctions::RotateVector(ForwardVector, 60.f) * lbe::PLAYER_ARM_DISTANCE);
			}
			RightHand.setPosition(SlashAnimation.GetInterpolatedPoint());
			RightHandSprite.setPosition(GetRightHandPos());
			//std::cout << "Anim; x=" << SlashAnimation.GetInterpolatedPoint().x << " y=" << SlashAnimation.GetInterpolatedPoint().y << std::endl;

			if (!hasHitWithCurrentAttack && SlashAnimation.GetAnimationState() >= 0.5f)
			{
				DefaultSwordSlashHitbox* SwordSlashHitbox = dynamic_cast<DefaultSwordSlashHitbox*>(lbe::SceneObjectFactory::CreateInstance("DefaultSwordSlashHitbox", GetPos()));
				SwordSlashHitbox->SetRotation(GetPlayerRotation());
				if (NetworkManager::GetActiveNetworkState() == NetworkState::Host)
				{
					LevelManager::AddSceneObject(SwordSlashHitbox, GetPos());
				}
				else if (NetworkManager::GetActiveNetworkState() == NetworkState::Client)
				{
					NetworkManager::SpawnObject(SwordSlashHitbox);
				}
				//LBE_TRACE("SwordSlashHitbox Spawned");

				hasHitWithCurrentAttack = true;
			}
		}
		else
		{
			hasHitWithCurrentAttack = false;
			bIsPerformingAttack = false;
		}
	}
}

void lbe::Player::SetPlayerView(sf::RenderWindow & window)
{
	window.setView(PlayerView);
}

sf::Vector2f lbe::Player::GetPos()
{
	return Body.getPosition();
}

sf::Vector2f lbe::Player::GetLeftHandPos()
{
	return LeftHand.getPosition();
}

sf::Vector2f lbe::Player::GetRightHandPos()
{
	return RightHand.getPosition();
}

void lbe::Player::SetPos(sf::Vector2f NewPos)
{
	Body.setPosition(NewPos);
	Position = Body.getPosition();
	BodySprite.setPosition(Position);
	PlayerView.setCenter(sf::Vector2f(Body.getPosition().x, Body.getPosition().y));
}

void lbe::Player::SetHandPos(sf::Vector2f LeftHandPos, sf::Vector2f RightHandPos)
{
	LeftHand.setPosition(LeftHandPos);
	RightHand.setPosition(RightHandPos);
	LeftHandSprite.setPosition(GetLeftHandPos());
	RightHandSprite.setPosition(GetRightHandPos());
	UpdateWeapon();
}

void lbe::Player::SetForwardVector(sf::Vector2f NewForwardVector)
{
	ForwardVector = NewForwardVector;
	BodySprite.setRotation(GetPlayerRotation() - 90.f);
}

void lbe::Player::UpdateView(sf::RenderWindow & Window)
{
	PlayerView.setSize(sf::Vector2f(Window.getSize().x, Window.getSize().y));
}

void lbe::Player::UpdatePlayerRotation(sf::Vector2f MousePos, sf::Time dt)
{
	if (!(Body.getPosition() == MousePos))
	{
		SetForwardVector((MousePos - Body.getPosition()) / (sqrt(pow(abs(MousePos.x - Body.getPosition().x), 2) + pow(abs(MousePos.y - Body.getPosition().y), 2))));
	}
}

void lbe::Player::UpdateWeapon()
{
	Weapon.setOrigin(sf::Vector2f(30, 120));
	Weapon.setPosition(GetRightHandPos());
	if (bIsPerformingAttack)
	{
		if (!bIsPerformingReverseAttack)
		{
			Weapon.setRotation(GetPlayerRotation() - (120.f * SlashAnimation.GetAnimationState() - 60.f));
		}
		else
		{
			Weapon.setRotation(GetPlayerRotation() - (120.f * (1 - SlashAnimation.GetAnimationState()) - 60.f));
		}
	}
	else
	{
		Weapon.setRotation(GetPlayerRotation());
	}
}

double lbe::Player::GetPlayerRotation()
{
	sf::Vector2f ZeroDegreeVector(0, -1);
	double Dot = ForwardVector.x * ZeroDegreeVector.x + ForwardVector.y * ZeroDegreeVector.y;
	double Determinant = ZeroDegreeVector.x * ForwardVector.y - ZeroDegreeVector.y * ForwardVector.x;
	double Angle =  atan2(Determinant, Dot);
	Angle = (Angle / lbe::PI) * 360 / 2;
	//std::cout << "Angle=" << Angle << std::endl;
	return Angle;
}

void lbe::Player::StartPerformingAttack()
{
	if (!bIsPerformingAttack)
	{
		double DistanceRegular = lbe::HelperFunctions::GetDistance(Body.getPosition() + lbe::HelperFunctions::RotateVector(ForwardVector, 60.f) * lbe::PLAYER_ARM_DISTANCE, RightHand.getPosition());
		double DistanceReverse = lbe::HelperFunctions::GetDistance(Body.getPosition() + lbe::HelperFunctions::RotateVector(ForwardVector, 300.f) * lbe::PLAYER_ARM_DISTANCE, RightHand.getPosition());
		if (DistanceRegular <= DistanceReverse)
		{
			bIsPerformingAttack = true;
			bIsPerformingReverseAttack = false;
			SlashAnimation.ResetAnimationState();
		}
		else
		{
			bIsPerformingAttack = true;
			bIsPerformingReverseAttack = true;
			SlashAnimation.ResetAnimationState();
		}
	}
}

void lbe::Player::ReceiveDamage(double DamageToTake)
{
	HitPoints -= DamageToTake;
	if (HitPoints <= 0)
	{
		bIsDead = true;
	}
}

bool lbe::Player::IsDead()
{
	return bIsDead;
}

void lbe::Player::Update(sf::Time dt)
{
}
