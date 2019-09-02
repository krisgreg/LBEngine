#pragma once
#include "SceneObject.h"

namespace lbe
{
	class SceneObjectPlaceableInEditor : public SceneObject
	{
	private:
	protected:

	public:
		SceneObjectPlaceableInEditor(sf::Vector2f Pos);
		virtual ~SceneObjectPlaceableInEditor();

		virtual bool ApplyUpdatePacket(sf::Packet& packet) = 0;
		virtual bool WriteUpdatePacket(sf::Packet& packet) = 0;

		virtual sf::Texture* GetObjectIcon() = 0;
		virtual sf::IntRect GetObjectIconRect() = 0;
		virtual std::string GetSaveString() = 0;
		virtual bool IsPointInHitbox(sf::Vector2f Point) = 0;
	};
}
