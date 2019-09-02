#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

namespace lbe
{
	class SceneObject
	{
	private:
		static sf::Uint32 UniqueIDCounter;
	protected:
		std::string Name;
		sf::Vector2f Position;
		bool bMarkedForDeletion = false;
		bool bIsPlaceableInEditor = false;
		sf::Uint32 UniqueID;

	public:
		SceneObject(sf::Vector2f Pos);
		virtual ~SceneObject();

		virtual bool ApplyUpdatePacket(sf::Packet& packet) = 0;
		virtual bool WriteUpdatePacket(sf::Packet& packet) = 0;

		virtual void Draw(sf::RenderWindow &window) = 0;
		virtual void Update(sf::Time dt) = 0;
		void MarkForDeletion();
		bool bIsMarkedForDeletion();
		void SetPosition(sf::Vector2f NewPosition);
		sf::Vector2f GetPosition();
		bool IsPlaceableInEditor();
		static sf::Uint32 GetNewUniqueID();
		void SetUniqueID(sf::Uint32 NewUniqueID);
		std::string GetName();
		sf::Uint32 GetUniqueID();
	};
}
