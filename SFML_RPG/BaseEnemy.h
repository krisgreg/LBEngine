#pragma once
#include <SFML/Graphics.hpp>
#include "SceneObjectPlaceableInEditor.h"
#include "SceneObjectFactory.h"

namespace lbe
{
	class BaseEnemy : public SceneObjectPlaceableInEditor
	{
	private:
		static RegisterAsSceneObject<BaseEnemy> RegisterSceneObject;
	protected:
		sf::CircleShape Body;
		sf::Vector2f ForwardVector = sf::Vector2f(0, -1);
		sf::Vector2f Destination = sf::Vector2f(0.f, 0.f);
		sf::Vector2f SpawnPosition;
		double MovementSpeed = 200.f;
		double AggroRange = 200.f;
		double DestinationAcceptanceDistance = 5.f;
		bool bHasSprite = false;
		sf::RectangleShape Sprite;
		double HitPoints = 100.f;
		bool bIsDead = false;

		sf::Vertex ForwardVectorLine[2];
	public:
		BaseEnemy(sf::Vector2f Pos);
		virtual ~BaseEnemy();

		bool ApplyUpdatePacket(sf::Packet& packet);
		bool WriteUpdatePacket(sf::Packet& packet);

		void Draw(sf::RenderWindow &window);
		void Move(sf::Vector2f MovementInput, sf::Time dt);
		sf::Vector2f GetPos();
		void SetPos(sf::Vector2f NewPos);
		void UpdateRotation(sf::Vector2f Point);
		double GetRotation();
		double GetRadius();
		void SetColors(sf::Color FillColor, sf::Color OutlineColor);
		void Update(sf::Time dt);
		void SetSprite(sf::Texture *Texture, sf::Vector2f TextureSize ,sf::Vector2f TexturePos);
		void ReceiveDamage(double DamageToTake);
		bool IsDead();
		void SetDestination(sf::Vector2f NewDestination);
		void UpdateAI();
		std::string GetSaveString();

		sf::Texture* GetObjectIcon();
		sf::IntRect GetObjectIconRect();
		bool IsPointInHitbox(sf::Vector2f Point);
	};
}