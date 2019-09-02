#pragma once
#include <SFML/Graphics.hpp>

#include "Tile.h"
#include "Consts.h"
#include "Enums.h"
#include "SceneObject.h"

namespace lbe
{
	class Level
	{
	private: 
		sf::Vector2i AnchorPoint;
		sf::Vector2i Size;
		sf::Texture *TextureMap;
		std::vector<Tile> Tiles;
		std::string LevelName;
		std::vector<std::vector<int>> CollisionMask;
		std::vector<std::string> Neighbours;
		std::vector<sf::RectangleShape> CollisionTiles;
		std::vector<SceneObject*> SceneObjects;
		sf::RectangleShape AnchorTile;
		sf::RectangleShape SizeTile;
		sf::RectangleShape BorderTile;
	public:
		Level(std::string LevelName, sf::Texture *TextureMap);
		~Level();

		sf::Vector2f HandleCollision(sf::Vector2f PlayerPos, double Radius = lbe::PLAYER_RADIUS);
		void Draw(sf::RenderWindow &window);
		void DrawLevelByName(sf::RenderWindow &window, std::string LevelToDraw);
		bool bPlayerIsInLevel(sf::Vector2f PlayerPos);
		std::vector<std::string> GetNeighbours();
		std::string GetLevelName();
		sf::Vector2i GetAnchorPoint();
		sf::Vector2i GetSize();
		void PaintTile(sf::Vector2f WorldPos, int NewTextureId, lbe::Layer Layer);
		void SaveLevel(sf::Vector2i AnchorPoint, sf::Vector2i Size);
		void SaveLevel();
		void UpdateSceneObjects(sf::Time dt);
		void UpdateAI();
		std::vector<SceneObject*>& GetSceneObjects();
		void AddSceneObject(SceneObject* NewSceneObject);
		void DeleteMarkedSceneObjects();
		void ApplyUpdatePacket(sf::Packet &packet);
		SceneObject* GetSceneObjectByID(sf::Uint32 ID);
		bool SceneObjectWithIDExist(sf::Uint32 ID);
		void UpdateBorderTile();
	};
}
