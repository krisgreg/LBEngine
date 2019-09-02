#include "SceneObject.h"

#include "NetworkManager.h";

sf::Uint32 lbe::SceneObject::UniqueIDCounter = 0;

lbe::SceneObject::SceneObject(sf::Vector2f Pos)
{
	Position = Pos;
	if (NetworkManager::GetActiveNetworkState() == NetworkState::Host)
	{
		UniqueID = GetNewUniqueID();
	}
}

lbe::SceneObject::~SceneObject()
{
}

void lbe::SceneObject::MarkForDeletion()
{
	bMarkedForDeletion = true;
}

bool lbe::SceneObject::bIsMarkedForDeletion()
{
	return bMarkedForDeletion;
}

void lbe::SceneObject::SetPosition(sf::Vector2f NewPosition)
{
	Position = NewPosition;
}

sf::Vector2f lbe::SceneObject::GetPosition()
{
	return Position;
}

bool lbe::SceneObject::IsPlaceableInEditor()
{
	return bIsPlaceableInEditor;
}

sf::Uint32 lbe::SceneObject::GetNewUniqueID()
{
	UniqueIDCounter++;
	return UniqueIDCounter;
}

void lbe::SceneObject::SetUniqueID(sf::Uint32 NewUniqueID)
{
	UniqueID = NewUniqueID;
}

std::string lbe::SceneObject::GetName()
{
	return Name;
}

sf::Uint32 lbe::SceneObject::GetUniqueID()
{
	return UniqueID;
}
