#pragma once
#include "State.h"

#include <SFML/Graphics.hpp>

#include "Button.h"

namespace lbe
{
	class StateMachine;

	class MenuState : public lbe::State
	{
	private:
		sf::Font AsimovFont;
		std::vector<lbe::Button> Buttons;
		sf::Text MainMenuHeadline;
		StateMachine& Machine;
		sf::RenderWindow& Window;
		sf::View DefaultView;

	public:
		MenuState(StateMachine& machine, sf::RenderWindow& window, bool replace = true);
		~MenuState();

		void Pause();
		void Resume();

		void Update();
		void Draw();

		void CreateWidgets(StateMachine& machine, sf::RenderWindow& window);
	};
}
