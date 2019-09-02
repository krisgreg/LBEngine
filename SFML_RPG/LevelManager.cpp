#include "LevelManager.h"

#include <iostream>
#include <filesystem>
#include <sstream>

#include "Consts.h"
#include "TextureLoader.h"

std::vector<lbe::Level> lbe::LevelManager::Levels;

lbe::LevelManager::LevelManager()
{
}


lbe::LevelManager::~LevelManager()
{
}

void lbe::LevelManager::LoadLevels()
{
	Levels.clear();

	std::string path = ".\\assets\\levels\\";
	for (auto & p : std::experimental::filesystem::directory_iterator(path))
	{
		std::stringstream LevelStream;
		LevelStream << p;
		std::string LevelName(LevelStream.str());
		LevelName.replace(LevelName.find(path), path.length(), "");
		LevelName.replace(LevelName.find(".txt"), 4, "");
		//std::cout << "AutoPathExplorer: " << LevelName << std::endl;
		Levels.push_back(lbe::Level(LevelName, &lbe::TextureLoader::TileTextureMap));
	}
}

sf::Vector2f lbe::LevelManager::HandleCollision(sf::Vector2f PlayerPos, double Radius)
{
	sf::Vector2f CurrentPos(PlayerPos);
	for (auto &i : Levels)
	{
		CurrentPos = i.HandleCollision(PlayerPos, Radius);
		if (CurrentPos != PlayerPos)
		{
			return CurrentPos;
		}
	}
	return PlayerPos;
}

sf::Vector2i lbe::LevelManager::GetLevelAnchorPointByName(std::string LevelName)
{
	for (auto &i : Levels)
	{
		if (i.GetLevelName() == LevelName)
		{
			return i.GetAnchorPoint();
		}
	}

	return sf::Vector2i();
}

sf::Vector2i lbe::LevelManager::GetLevelSizeByName(std::string LevelName)
{
	for (auto &i : Levels)
	{
		if (i.GetLevelName() == LevelName)
		{
			return i.GetSize();
		}
	}

	return sf::Vector2i();
}

void lbe::LevelManager::DrawAllLevels(sf::RenderWindow & window)
{
	for (auto &i : Levels)
	{
		i.Draw(window);
	}
}

void lbe::LevelManager::DrawCurrentLevel(sf::RenderWindow &window, sf::Vector2f PlayerPos)
{
	for (auto &i : Levels)
	{
		if (i.bPlayerIsInLevel(PlayerPos))
		{
			i.Draw(window);
			for (auto &Neighbour : i.GetNeighbours())
			{
				for (auto &j : Levels)
				{
					if (j.GetLevelName() == Neighbour)
					{
						j.Draw(window);
					}
				}
			}
		}
	}
}

void lbe::LevelManager::DrawLevel(sf::RenderWindow & window, sf::String LevelName)
{
	for (auto &i : Levels)
	{
		if (i.GetLevelName() == LevelName)
		{
			i.Draw(window);
		}
	}
}

void lbe::LevelManager::PaintTile(std::string LevelName, sf::Vector2f WorldPos, int NewTextureId, lbe::Layer Layer)
{
	for (auto &i : Levels)
	{
		if (i.GetLevelName() == LevelName)
		{
			i.PaintTile(WorldPos, NewTextureId, Layer);
		}
	}
}

void lbe::LevelManager::SaveLevel(std::string LevelName, sf::Vector2i AnchorPoint, sf::Vector2i Size)
{
	for (auto &i : Levels)
	{
		if (i.GetLevelName() == LevelName)
		{
			i.SaveLevel(AnchorPoint, Size);
		}
	}
	LBE_INFO("Level Saved");
}

void lbe::LevelManager::SaveLevel(std::string LevelName)
{
	for (auto &i : Levels)
	{
		if (i.GetLevelName() == LevelName)
		{
			i.SaveLevel();
		}
	}
	LBE_INFO("Level Saved");
}

void lbe::LevelManager::UpdateSceneObjects(sf::Time dt)
{
	for (auto &i : Levels)
	{
		i.UpdateSceneObjects(dt);
	}
}

void lbe::LevelManager::UpdateAI()
{
	for (auto &i : Levels)
	{
		i.UpdateAI();
	}
}

std::vector<lbe::SceneObject*>& lbe::LevelManager::GetSceneObjectsFromLevel(sf::Vector2f Position)
{
	for (auto &i : Levels)
	{
		if (i.bPlayerIsInLevel(Position))
		{
			return i.GetSceneObjects();
		}
	}
	std::vector<lbe::SceneObject*> EmptyVector;
	return EmptyVector;
}

void lbe::LevelManager::AddSceneObject(SceneObject* NewSceneObject, sf::Vector2f Position)
{
	for (auto &i : Levels)
	{
		if (i.bPlayerIsInLevel(Position))
		{
			i.AddSceneObject(NewSceneObject);
		}
	}
}

void lbe::LevelManager::DeleteMarkedSceneObjects()
{
	for (auto &i : Levels)
	{
		i.DeleteMarkedSceneObjects();
	}
}

void lbe::LevelManager::ApplyUpdatePacketToLevel(sf::Packet &packet, sf::Vector2f Position)
{
	for (auto &i : Levels)
	{
		if (i.bPlayerIsInLevel(Position))
		{
			i.ApplyUpdatePacket(packet);
		}
	}
}

lbe::SceneObject* lbe::LevelManager::GetSceneObjectByID(sf::Uint32 ID)
{
	for (auto &i : Levels)
	{
		if (i.SceneObjectWithIDExist(ID))
		{
			return i.GetSceneObjectByID(ID);
		}
	}
}

bool lbe::LevelManager::SceneObjectWithIDExist(sf::Uint32 ID)
{
	for (auto &i : Levels)
	{
		if (i.SceneObjectWithIDExist(ID))
		{
			return true;
		}
	}
	return false;
}