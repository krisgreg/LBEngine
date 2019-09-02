#pragma once
#include <memory>
#include <stack>
#include <SFML/Graphics.hpp>

namespace lbe
{
	class State;

	class StateMachine
	{
	private:
		std::stack<std::unique_ptr<State>> States;

		bool bResume = false;
		bool bIsRunning = false;
	public:
		StateMachine();
		~StateMachine();

		void Run(std::unique_ptr<State> state);

		void NextState();
		void LastState();

		void Update();
		void Draw();

		bool Running() { return bIsRunning; }
		void Quit() { bIsRunning = false; }

		template <typename T>
		static std::unique_ptr<T> Build(StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	};

	template <typename T>
	std::unique_ptr<T> lbe::StateMachine::Build(StateMachine& machine, sf::RenderWindow& window, bool replace)
	{
		return std::unique_ptr<T>(new T(machine, window, replace));
	}
}