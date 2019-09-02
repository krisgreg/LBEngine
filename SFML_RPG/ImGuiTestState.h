#pragma once
#include "State.h"
#include "Camera.h"

namespace lbe
{
	class StateMachine;

	class ImGuiTestState : public State
	{
	private:
		sf::Clock Clock;
		sf::Time dt;
		sf::View DefaultView;
		lbe::Camera Camera;
		StateMachine& Machine;
		sf::RenderWindow& Window;
	public:
		ImGuiTestState(StateMachine & machine, sf::RenderWindow & window, bool replace);
		~ImGuiTestState();

		void Pause();
		void Resume();

		void Update();
		void Draw();
	};
}