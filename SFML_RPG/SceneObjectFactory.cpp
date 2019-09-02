#include "SceneObjectFactory.h"
#include "Log.h"

SceneObjectCreatorMapType* lbe::SceneObjectFactory::SceneObjectCreator;

SceneObjectCreatorMapType * lbe::SceneObjectFactory::GetMap()
{
	if (!SceneObjectCreator)
	{
		lbe::SceneObjectFactory::SceneObjectCreator = new SceneObjectCreatorMapType;
	}
	return SceneObjectCreator;
}

lbe::SceneObjectFactory::SceneObjectFactory()
{
}

lbe::SceneObjectFactory::~SceneObjectFactory()
{
}

lbe::SceneObject* lbe::SceneObjectFactory::CreateInstance(std::string const &name, sf::Vector2f Pos)
{
	SceneObjectCreatorMapType::iterator it = GetMap()->find(name);
	if (it == GetMap()->end())
	{
		return 0;
	}
	return it->second(Pos);
}