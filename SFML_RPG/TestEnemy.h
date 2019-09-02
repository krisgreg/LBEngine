#pragma once
#include "BaseEnemy.h"
#include "SceneObjectFactory.h"

namespace lbe
{
	class TestEnemy : public BaseEnemy
	{
	private:
		static RegisterAsSceneObject<TestEnemy> RegisterSceneObject;
	public:
		TestEnemy(sf::Vector2f Pos);
		~TestEnemy();

		sf::Texture* GetObjectIcon();
		sf::IntRect GetObjectIconRect();
		std::string GetSaveString();
		bool IsPointInHitbox(sf::Vector2f Point);
	};
}
