#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace lbe
{
	class StateMachine;

	class State
	{
	protected:
		StateMachine& Machine;
		sf::RenderWindow& Window;

		bool bIsReplacing;

		std::unique_ptr<State> NextState;
	public:
		State(StateMachine &Machine, sf::RenderWindow &Window, bool Replace = true);
		virtual ~State();

		State(const State&) = delete;
		State& operator= (const State&) = delete;

		virtual void Pause() = 0;
		virtual void Resume() = 0;

		virtual void Update() = 0;
		virtual void Draw() = 0;

		std::unique_ptr<State> Next();

		bool GetbIsReplacing();
	};
}