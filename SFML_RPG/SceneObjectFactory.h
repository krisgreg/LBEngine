#pragma once
#include <map>
#include <functional>

#include "SceneObject.h"
#include "Log.h"

template<typename T> lbe::SceneObject* CreateT(sf::Vector2f Pos) { return new T(Pos); }

typedef std::map<std::string, std::function<lbe::SceneObject*(sf::Vector2f)>> SceneObjectCreatorMapType;

namespace lbe
{
	class SceneObjectFactory
	{
	private:
		static SceneObjectCreatorMapType* SceneObjectCreator;

	public:
		SceneObjectFactory();
		~SceneObjectFactory();

		static SceneObjectCreatorMapType* GetMap();
		static lbe::SceneObject* CreateInstance(std::string const& name, sf::Vector2f Pos);
	};

	template<typename T>
	struct RegisterAsSceneObject : SceneObjectFactory
	{
		RegisterAsSceneObject(std::string const& name)
		{
			SceneObjectFactory::GetMap()->insert(std::make_pair(name, &CreateT<T>));
		}
	};
}
