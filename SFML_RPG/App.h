#pragma once
#include <SFML/Graphics.hpp>

#include "StateMachine.h"

namespace lbe
{
	class App
	{
	private:
		sf::RenderWindow Window;
		StateMachine Machine;
	public:
		App();
		~App();

		void Run();
	};
}

