#pragma once
#include "SceneObject.h"

#include "SceneObjectFactory.h"

namespace lbe
{
	class DefaultSwordSlashHitbox : public SceneObject
	{
	private:
		static RegisterAsSceneObject<DefaultSwordSlashHitbox> RegisterSceneObject;
		double Damage = 200.f;
		double Rotation = 0.f;
		double ArcSize = 60.f;
	public:
		DefaultSwordSlashHitbox(sf::Vector2f Pos);
		virtual ~DefaultSwordSlashHitbox();

		bool ApplyUpdatePacket(sf::Packet& packet);
		bool WriteUpdatePacket(sf::Packet& packet);

		void Draw(sf::RenderWindow &window);
		void Update(sf::Time dt);
		void SetRotation(double NewRotation);
	};
}
