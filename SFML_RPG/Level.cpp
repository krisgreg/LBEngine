#include "Level.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "HelperFunctions.h"
#include "Consts.h"
#include "Enums.h"
#include "LevelEditorSidePanel.h"
#include "TestEnemy.h"
#include "SceneObjectFactory.h"
#include "LevelManager.h"
#include "NetworkManager.h"

lbe::Level::Level(std::string LevelName, sf::Texture *TextureMap) : TextureMap(TextureMap), LevelName(LevelName)
{
	LBE_INFO("Loading Level: {0}..", LevelName);

	std::stringstream FilePath;
	FilePath << "./assets/levels/" << LevelName << ".txt";

	std::ifstream LevelFile;
	LevelFile.open(FilePath.str(), std::ios::in);

	if (LevelFile.is_open())
	{
		std::string Line;
		int RowNumber = 0;
		int CollisionRowNumber = 0;
		while (std::getline(LevelFile, Line))
		{
			std::string Identifier;
			std::istringstream LineStream(Line);

			LineStream >> Identifier;
			if (Identifier == "a")
			{
				LineStream >> AnchorPoint.x >> AnchorPoint.y;
				AnchorPoint *= lbe::TILE_SIZE;
			}
			else if (Identifier == "s")
			{
				LineStream >> Size.x >> Size.y;
				CollisionMask.resize(Size.x);
				for (auto &i : CollisionMask)
				{
					i.resize(Size.y);
				}
			}
			else if (Identifier == "r")
			{
				for (int i = 0; i < Size.x; i++)
				{
					int TileTextureId;
					LineStream >> TileTextureId;
					if (TileTextureId != 0)
					{
						TileTextureId--;
						sf::Vector2f NewTileAnchorPoint(AnchorPoint.x + i * lbe::TILE_SIZE, AnchorPoint.y + RowNumber * lbe::TILE_SIZE);
						sf::Vector2f NewTileTexturePos((lbe::TILE_TEXTURE_SIZE * TileTextureId) % lbe::TEXTURE_MAP_SIZE.x, floor((lbe::TILE_TEXTURE_SIZE * TileTextureId) / lbe::TEXTURE_MAP_SIZE.x) * lbe::TILE_TEXTURE_SIZE);
						//std::cout << "Anchor.x=" << NewTileAnchorPoint.x << " Anchor.y=" << NewTileAnchorPoint.y << std::endl;
						//std::cout << "TexturePos.x=" << NewTileTexturePos.x << " TexturePos.y=" << NewTileTexturePos.y << std::endl;
						Tiles.push_back(lbe::Tile(NewTileAnchorPoint, TextureMap, NewTileTexturePos, ++TileTextureId));
					}
				}
				RowNumber++;
			}
			else if (Identifier == "c")
			{
				for (int i = 0; i < Size.x; i++)
				{
					int TileCollisionMask;
					LineStream >> TileCollisionMask;
					CollisionMask[i][CollisionRowNumber] = TileCollisionMask;

					if (TileCollisionMask == 1)
					{
						sf::Vector2f NewTileAnchorPoint(AnchorPoint.x + i * lbe::TILE_SIZE, AnchorPoint.y + CollisionRowNumber * lbe::TILE_SIZE);
						sf::RectangleShape NewCollisionTile;
						NewCollisionTile.setSize(sf::Vector2f(lbe::TILE_SIZE, lbe::TILE_SIZE));
						NewCollisionTile.setPosition(NewTileAnchorPoint);
						NewCollisionTile.setFillColor(lbe::COLLISION_TILE_COLOR);
						CollisionTiles.push_back(NewCollisionTile);
					}
				}
				CollisionRowNumber++;
			}
			else if (Identifier == "n")
			{
				std::string NewNeighbour;
				LineStream >> NewNeighbour;
				Neighbours.push_back(NewNeighbour);
			}
			else if (Identifier == "o")
			{
				if (NetworkManager::GetActiveNetworkState() == lbe::NetworkState::Host)
				{
					std::string NewSceneObjectName;
					sf::Vector2f NewSceneObjectPos;
					LineStream >> NewSceneObjectName >> NewSceneObjectPos.x >> NewSceneObjectPos.y;
					SceneObjects.emplace_back(lbe::SceneObjectFactory::CreateInstance(NewSceneObjectName, NewSceneObjectPos));
				}
			}
		}
		AnchorTile.setSize(sf::Vector2f(lbe::TILE_SIZE, lbe::TILE_SIZE));
		AnchorTile.setPosition(sf::Vector2f(AnchorPoint.x, AnchorPoint.y));
		AnchorTile.setFillColor(lbe::ANCHOR_TILE_COLOR);

		SizeTile.setSize(sf::Vector2f(lbe::TILE_SIZE, lbe::TILE_SIZE));
		SizeTile.setPosition(sf::Vector2f(AnchorPoint.x + (Size.x - 1) * lbe::TILE_SIZE, AnchorPoint.y + (Size.y - 1) * lbe::TILE_SIZE));
		SizeTile.setFillColor(lbe::SIZE_TILE_COLOR);

		UpdateBorderTile();
		BorderTile.setFillColor(sf::Color::Transparent);
		BorderTile.setOutlineColor(sf::Color::Red);
		BorderTile.setOutlineThickness(lbe::TILE_SIZE / 10.f);
	}
	else
	{
		LBE_WARN("Level: {0} could not be loaded, file could not be opened.", LevelName);
	}

	if (DEBUG_LEVEL_LOADER)
	{
		LBE_TRACE("Anchor.x={0:d} Anchor.y={1:d}", AnchorPoint.x, AnchorPoint.y);
		LBE_TRACE("Size.x={0:d} Size.y={1:d}", Size.x, Size.y);
	}
	LevelFile.close();

	LBE_INFO("Loaded Level: {0}!", LevelName);
}


lbe::Level::~Level()
{
}

sf::Vector2f lbe::Level::HandleCollision(sf::Vector2f PlayerPos, double Radius)
{
	sf::FloatRect LevelRect(AnchorPoint.x, AnchorPoint.y, Size.x * lbe::TILE_SIZE, Size.y * lbe::TILE_SIZE);
	if (LevelRect.contains(PlayerPos))
	{
		//std::cout << LevelName << " Intersect!" << std::endl;
		int XPos = floor((PlayerPos.x - AnchorPoint.x) / lbe::TILE_SIZE);
		int YPos = floor((PlayerPos.y - AnchorPoint.y) / lbe::TILE_SIZE);

		//std::cout << "XPos=" << XPos << " YPos=" << YPos << " Collision=" << CollisionMask[XPos][YPos] << std::endl;

		sf::Vector2f NewPlayerPos(PlayerPos);

		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				if ((XPos + x) >= 0 && (XPos + x) <= Size.x - 1 && (YPos + y) >= 0 && (YPos + y) <= Size.y - 1)
				{
					if (CollisionMask[XPos + x][YPos + y] == 1)
					{
						sf::FloatRect XRect(AnchorPoint.x + (lbe::TILE_SIZE * (XPos + x)) - Radius, AnchorPoint.y + (lbe::TILE_SIZE * (YPos + y)), lbe::TILE_SIZE + Radius * 2, lbe::TILE_SIZE);
						if (XRect.contains(NewPlayerPos))
						{
							if (NewPlayerPos.x < XRect.left + XRect.width / 2)
							{
								if ((XPos - 1) >= 0 && (XPos - 1) <= Size.x - 1 && (YPos) >= 0 && (YPos) <= Size.y - 1)
								{ 
									if (CollisionMask[XPos - 1][YPos] == 0)
									{
										NewPlayerPos.x = XRect.left;
									}
								}
							}
							else
							{
								if ((XPos + 1) >= 0 && (XPos + 1) <= Size.x - 1 && (YPos) >= 0 && (YPos) <= Size.y - 1)
								{
									if (CollisionMask[XPos + 1][YPos] == 0)
									{
										NewPlayerPos.x = XRect.left + XRect.width;
									}
								}
							}
						}
						sf::FloatRect YRect(AnchorPoint.x + (lbe::TILE_SIZE * (XPos + x)), AnchorPoint.y + (lbe::TILE_SIZE * (YPos + y)) - Radius, lbe::TILE_SIZE, lbe::TILE_SIZE + Radius * 2);
						if (YRect.contains(NewPlayerPos))
						{
							if (NewPlayerPos.y < XRect.top + YRect.height / 2)
							{
								if ((XPos) >= 0 && (XPos) <= Size.x - 1 && (YPos - 1) >= 0 && (YPos - 1) <= Size.y - 1)
								{
									if (CollisionMask[XPos][YPos - 1] == 0)
									{
										NewPlayerPos.y = YRect.top;
									}
								}
							}
							else
							{
								if ((XPos) >= 0 && (XPos) <= Size.x - 1 && (YPos + 1) >= 0 && (YPos + 1) <= Size.y - 1)
								{
									if (CollisionMask[XPos][YPos + 1] == 0)
									{
										NewPlayerPos.y = YRect.top + YRect.height;
									}
								}
							}
						}

						std::vector<sf::Vector2f> Corners;
						Corners.push_back(sf::Vector2f(AnchorPoint.x + (lbe::TILE_SIZE * (XPos + x + 0)), AnchorPoint.y + (lbe::TILE_SIZE * (YPos + y + 0))));
						Corners.push_back(sf::Vector2f(AnchorPoint.x + (lbe::TILE_SIZE * (XPos + x + 1)), AnchorPoint.y + (lbe::TILE_SIZE * (YPos + y + 0))));
						Corners.push_back(sf::Vector2f(AnchorPoint.x + (lbe::TILE_SIZE * (XPos + x + 0)), AnchorPoint.y + (lbe::TILE_SIZE * (YPos + y + 1))));
						Corners.push_back(sf::Vector2f(AnchorPoint.x + (lbe::TILE_SIZE * (XPos + x + 1)), AnchorPoint.y + (lbe::TILE_SIZE * (YPos + y + 1))));

						for (auto &Corner : Corners)
						{
							double DistanceToCorner = lbe::HelperFunctions::GetDistance(NewPlayerPos, Corner);
							if (DistanceToCorner < Radius)
							{
								double Overlap = 1.f * (DistanceToCorner - Radius);
								NewPlayerPos.x -= Overlap * (NewPlayerPos.x - Corner.x) / DistanceToCorner;
								NewPlayerPos.y -= Overlap * (NewPlayerPos.y - Corner.y) / DistanceToCorner;
							}
						}
					}
				}
			}
		}
		
		return NewPlayerPos;
	}

	return PlayerPos;
}

void lbe::Level::Draw(sf::RenderWindow & window)
{
	for (auto &i : Tiles)
	{
		i.Draw(window);
	}

	for (auto &i : SceneObjects)
	{
		i->Draw(window);
	}

	if (lbe::LevelEditorSidePanel::GetSelectedLayer() == lbe::Layer::Collision)
	{
		for (auto &i : CollisionTiles)
		{
			window.draw(i);
		}
	}
	else if (lbe::LevelEditorSidePanel::GetSelectedLayer() == lbe::Layer::Meta)
	{
		window.draw(AnchorTile);
		window.draw(SizeTile);
		window.draw(BorderTile);
	}
}

void lbe::Level::DrawLevelByName(sf::RenderWindow & window, std::string LevelToDraw)
{
	if (LevelToDraw == LevelName)
	{
		for (auto &i : Tiles)
		{
			i.Draw(window);
		}
	}
}

bool lbe::Level::bPlayerIsInLevel(sf::Vector2f PlayerPos)
{
	sf::FloatRect LevelRect(AnchorPoint.x, AnchorPoint.y, Size.x * lbe::TILE_SIZE, Size.y * lbe::TILE_SIZE);
	if (LevelRect.contains(PlayerPos))
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<std::string> lbe::Level::GetNeighbours()
{
	return Neighbours;
}

std::string lbe::Level::GetLevelName()
{
	return LevelName;
}

sf::Vector2i lbe::Level::GetAnchorPoint()
{
	return AnchorPoint;
}

sf::Vector2i lbe::Level::GetSize()
{
	return Size;
}

void lbe::Level::PaintTile(sf::Vector2f WorldPos, int NewTextureId, lbe::Layer Layer)
{
	if (Layer == lbe::Layer::Background)
	{
		auto i = Tiles.begin();
		while (i != Tiles.end())
		{
			if ((i)->bContainsPoint(WorldPos))
			{
				i = Tiles.erase(i);
			}
			else
			{
				++i;
			}
		}

		if (NewTextureId != 0)
		{
			NewTextureId--;
			sf::Vector2f NewTileTexturePos((lbe::TILE_TEXTURE_SIZE * NewTextureId) % lbe::TEXTURE_MAP_SIZE.x, floor((lbe::TILE_TEXTURE_SIZE * NewTextureId) / lbe::TEXTURE_MAP_SIZE.x) * lbe::TILE_TEXTURE_SIZE);
			Tiles.push_back(lbe::Tile(sf::Vector2f(floor(WorldPos.x / lbe::TILE_SIZE) * lbe::TILE_SIZE, floor(WorldPos.y / lbe::TILE_SIZE) * lbe::TILE_SIZE), TextureMap, NewTileTexturePos, ++NewTextureId));
		}
	}
	else if (Layer == lbe::Layer::Collision)
	{
		auto i = CollisionTiles.begin();
		while (i != CollisionTiles.end())
		{
			if ((i)->getGlobalBounds().contains(WorldPos))
			{
				i = CollisionTiles.erase(i);
			}
			else
			{
				++i;
			}
		}

		if (NewTextureId != 0)
		{
			sf::RectangleShape NewCollisionTile;
			NewCollisionTile.setSize(sf::Vector2f(lbe::TILE_SIZE, lbe::TILE_SIZE));
			NewCollisionTile.setPosition(sf::Vector2f(floor(WorldPos.x / lbe::TILE_SIZE) * lbe::TILE_SIZE, floor(WorldPos.y / lbe::TILE_SIZE) * lbe::TILE_SIZE));
			NewCollisionTile.setFillColor(lbe::COLLISION_TILE_COLOR);
			CollisionTiles.push_back(NewCollisionTile);
		}
	}
	else if (Layer == lbe::Layer::Meta)
	{
		sf::Vector2f NewTilePos(floor(WorldPos.x / lbe::TILE_SIZE) * lbe::TILE_SIZE, floor(WorldPos.y / lbe::TILE_SIZE) * lbe::TILE_SIZE);
		// 0 = anchor tile, 1 = size tile
		if (NewTextureId == 0)
		{
			AnchorTile.setPosition(NewTilePos);
		}
		else if (NewTextureId == 1)
		{
			SizeTile.setPosition(NewTilePos);
		}
		UpdateBorderTile();
	}
}

void lbe::Level::SaveLevel(sf::Vector2i AnchorPoint, sf::Vector2i NewSize)
{
	std::stringstream NewFileString;
	NewFileString << "# a = anchor point in world coords\n"
		<< "# s = x and y size of map\n"
		<< "# r = a row of tiles\n"
		<< "# c = collision mask; 1 = wall, 0 = air\n"
		<< "# n = neighbour zone\n"
		<< "# o = scene object\n\n"
		<< "a " << AnchorPoint.x << " " << AnchorPoint.y << "\n"
		<< "s " << NewSize.x << " " << NewSize.y << "\n\n";

	// r
	std::vector<std::vector<int>> TileTextureIds;
	TileTextureIds.resize(NewSize.y);
	for (auto &i : TileTextureIds)
	{
		i.resize(NewSize.x);
	}

	for (auto &Tile : Tiles)
	{
		sf::Vector2f WorldPos = Tile.GetWorldPos();
		int TextureId = Tile.GetTextureId();

		int TileXPos = (WorldPos.x / lbe::TILE_SIZE) - AnchorPoint.x;
		int TileYPos = (WorldPos.y / lbe::TILE_SIZE) - AnchorPoint.y;

		TileTextureIds[TileYPos][TileXPos] = TextureId;
	}


	for (auto &i : TileTextureIds)
	{
		NewFileString << "r";
		for (auto &j : i)
		{
			NewFileString << " " << j;
		}
		NewFileString << "\n";
	}
	NewFileString << "\n";

	// c
	CollisionMask.resize(NewSize.y);
	for (auto &i : CollisionMask)
	{
		i.resize(NewSize.x);
	}

	for (auto &i : CollisionMask)
	{
		for (auto &j : i)
		{
			j = 0;
		}
	}

	for (auto &Tile : CollisionTiles)
	{
		sf::Vector2f WorldPos = sf::Vector2f(Tile.getGlobalBounds().left, Tile.getGlobalBounds().top);

		int TileXPos = (WorldPos.x / lbe::TILE_SIZE) - AnchorPoint.x;
		int TileYPos = (WorldPos.y / lbe::TILE_SIZE) - AnchorPoint.y;

		if (!(TileXPos >= NewSize.x || TileXPos < 0 || TileYPos >= NewSize.y || TileYPos < 0))
		{
			CollisionMask[TileYPos][TileXPos] = 1;
		}
	}

	for (auto &i : CollisionMask)
	{
		NewFileString << "c";
		for (auto &j : i)
		{
			NewFileString << " " << j;
		}
		NewFileString << "\n";
	}
	NewFileString << "\n";

	for (auto &i : SceneObjects)
	{
		if (SceneObjectPlaceableInEditor* Object = dynamic_cast<SceneObjectPlaceableInEditor*>(i))
		{
			NewFileString << "o " << Object->GetSaveString() << "\n";
		}
	}
	NewFileString << "\n";

	// n
	std::stringstream FilePath;
	FilePath << "./assets/levels/" << LevelName << ".txt";

	std::ifstream LevelFile;
	LevelFile.open(FilePath.str(), std::ios::in);

	if (LevelFile.is_open())
	{
		std::string Line;
		while (std::getline(LevelFile, Line))
		{
			std::string Identifier;
			std::istringstream LineStream(Line);

			LineStream >> Identifier;
			if (Identifier == "n")
			{
				NewFileString << "n ";
				std::string NewNeighbour;
				LineStream >> NewNeighbour;
				NewFileString << NewNeighbour << "\n";
			}
		}
	}
	else
	{
		LBE_WARN("Level: {0} could not be loaded, file could not be opened.", LevelName);
	}

	LevelFile.close();

	std::remove(FilePath.str().c_str());

	std::ofstream NewLevelFile;
	NewLevelFile.open(FilePath.str());
	if (NewLevelFile.is_open())
	{
		NewLevelFile << NewFileString.str();
	}
	else
	{
		LBE_WARN("Level: {0} could not be loaded, file could not be opened.", LevelName);
	}

	NewLevelFile.close();
}

void lbe::Level::SaveLevel()
{
	sf::Vector2i NewAnchorPoint(AnchorTile.getGlobalBounds().left / lbe::TILE_SIZE, AnchorTile.getGlobalBounds().top / lbe::TILE_SIZE);
	sf::Vector2i NewSize(BorderTile.getGlobalBounds().width / lbe::TILE_SIZE, BorderTile.getGlobalBounds().height / lbe::TILE_SIZE);

	SaveLevel(NewAnchorPoint, NewSize);
}

void lbe::Level::UpdateSceneObjects(sf::Time dt)
{
	for (auto &Object : SceneObjects)
	{
		if (BaseEnemy* Enemy = dynamic_cast<BaseEnemy*>(Object))
		{
			Enemy->Update(dt);
			Enemy->SetPos(LevelManager::HandleCollision(Enemy->GetPos(), Enemy->GetRadius()));
		}
		else
		{
			Object->Update(dt);
		}
	}

	DeleteMarkedSceneObjects();
}

void lbe::Level::UpdateAI()
{
	for (auto &Object : SceneObjects)
	{
		if (BaseEnemy* Enemy = dynamic_cast<BaseEnemy*>(Object))
		{
			Enemy->UpdateAI();
		}
	}
}

std::vector<lbe::SceneObject*>& lbe::Level::GetSceneObjects()
{
	return SceneObjects;
}

void lbe::Level::AddSceneObject(SceneObject* NewSceneObject)
{
	SceneObjects.emplace_back(NewSceneObject);
}

void lbe::Level::DeleteMarkedSceneObjects()
{
	auto i = SceneObjects.begin();
	while (i != SceneObjects.end())
	{
		if ((*i)->bIsMarkedForDeletion())
		{
			if (NetworkManager::GetActiveNetworkState() == NetworkState::Host)
			{
				NetworkManager::RemoveObjectByID((*i)->GetUniqueID());
			}
			i = SceneObjects.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void lbe::Level::ApplyUpdatePacket(sf::Packet &packet)
{
	//LBE_TRACE("update packet recieved for level!");
	//LBE_TRACE("Initial Packet Size = {0}", packet.getDataSize());
	while (!packet.endOfPacket())
	{
		sf::Vector2f Pos;
		sf::Uint32 UniqueID;
		packet >> UniqueID;
		//LBE_TRACE("Update packet content: ID={0}", UniqueID);
		if (SceneObjectWithIDExist(UniqueID))
		{
			GetSceneObjectByID(UniqueID)->ApplyUpdatePacket(packet);
		}
	}
}

// Method is not safe given an ID not in use
// Use SceneObjectWithIDExist(sf::Uint32) to confirm ID is in use before calling this
lbe::SceneObject* lbe::Level::GetSceneObjectByID(sf::Uint32 ID)
{
	for (auto &object : SceneObjects)
	{
		if (object->GetUniqueID() == ID)
		{
			return object;
		}
	}
	LBE_CRITICAL("GetSceneObjectByID(sf::Uint32) could not find an object with the given id");
}

bool lbe::Level::SceneObjectWithIDExist(sf::Uint32 ID)
{
	for (auto &object : SceneObjects)
	{
		if (object->GetUniqueID() == ID)
		{
			return true;
		}
	}
	return false;
}

void lbe::Level::UpdateBorderTile()
{
	BorderTile.setSize(sf::Vector2f(SizeTile.getGlobalBounds().left - AnchorTile.getGlobalBounds().left + lbe::TILE_SIZE, SizeTile.getGlobalBounds().top - AnchorTile.getGlobalBounds().top + lbe::TILE_SIZE));
	BorderTile.setPosition(sf::Vector2f(AnchorTile.getGlobalBounds().left, AnchorTile.getGlobalBounds().top));
}
