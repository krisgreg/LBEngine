#pragma once
#include <SFML/Graphics.hpp>
#include "PointInterpolator.h"
#include "SceneObject.h"
#include "SceneObjectFactory.h"

namespace lbe
{
	class Player : public SceneObject
	{
	private:
		static RegisterAsSceneObject<Player> RegisterSceneObject;
		sf::CircleShape Body;
		sf::RectangleShape BodySprite;
		sf::View PlayerView;
		sf::Vector2f ForwardVector;
		sf::CircleShape LeftHand;
		sf::CircleShape RightHand;
		sf::RectangleShape LeftHandSprite;
		sf::RectangleShape RightHandSprite;
		sf::Vector2f DesiredLeftHandPos;
		sf::Vector2f DesiredRightHandPos;
		// used 25 pre hand changes
		int HandFollowSpeed = 15;
		sf::RectangleShape Weapon;
		lbe::PointInterpolator SlashAnimation;
		bool bIsPerformingAttack = false;
		bool bIsPerformingReverseAttack = false;
		bool hasHitWithCurrentAttack = false;
		double HitPoints = 100.f;
		bool bIsDead = false;

		sf::Vertex ForwardVectorLine[2];
		sf::Vertex ArmPositionLine[4];
	public:
		Player(sf::Vector2f Pos);
		~Player();

		bool ApplyUpdatePacket(sf::Packet& packet);
		bool WriteUpdatePacket(sf::Packet& packet);

		void Draw(sf::RenderWindow &window);
		void Move(sf::Vector2f MovementInput, sf::Time dt);
		void SetPlayerView(sf::RenderWindow &window);
		sf::Vector2f GetPos();
		sf::Vector2f GetLeftHandPos();
		sf::Vector2f GetRightHandPos();
		void SetPos(sf::Vector2f NewPos);
		void SetHandPos(sf::Vector2f LeftHandPos, sf::Vector2f RightHandPos);
		void SetForwardVector(sf::Vector2f NewForwardVector);
		void UpdateView(sf::RenderWindow& Window);
		void UpdatePlayerRotation(sf::Vector2f MousePos, sf::Time dt);
		void UpdateWeapon();
		double GetPlayerRotation();
		void StartPerformingAttack();
		void ReceiveDamage(double DamageToTake);
		bool IsDead();
		void Update(sf::Time dt);
	};
}

