#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Consts.h"

namespace lbe
{
	class LevelManager
	{
	private:
		static std::vector<lbe::Level> Levels;
	public:
		LevelManager();
		~LevelManager();

		static void LoadLevels();
		static sf::Vector2f HandleCollision(sf::Vector2f PlayerPos, double Radius = lbe::PLAYER_RADIUS);
		static sf::Vector2i GetLevelAnchorPointByName(std::string LevelName);
		static sf::Vector2i GetLevelSizeByName(std::string LevelName);

		static void DrawAllLevels(sf::RenderWindow &window);
		static void DrawCurrentLevel(sf::RenderWindow &window, sf::Vector2f PlayerPos);
		static void DrawLevel(sf::RenderWindow &window, sf::String LevelName);
		static void PaintTile(std::string LevelName, sf::Vector2f WorldPos, int NewTextureId, lbe::Layer Layer);
		static void SaveLevel(std::string LevelName, sf::Vector2i AnchorPoint, sf::Vector2i Size);
		static void SaveLevel(std::string LevelName);

		static void UpdateSceneObjects(sf::Time dt);
		static void UpdateAI();
		static std::vector<SceneObject*>& GetSceneObjectsFromLevel(sf::Vector2f Position);
		static void AddSceneObject(SceneObject* NewSceneObject, sf::Vector2f Position);
		static void DeleteMarkedSceneObjects();
		static void ApplyUpdatePacketToLevel(sf::Packet &packet, sf::Vector2f Position);
		static SceneObject* GetSceneObjectByID(sf::Uint32 ID);
		static bool SceneObjectWithIDExist(sf::Uint32 ID);
	};
}
