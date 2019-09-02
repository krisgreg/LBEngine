#pragma once
#include "State.h"

#include <SFML/Graphics.hpp>

#include "Button.h"
#include "Player.h"
#include "LevelManager.h"
#include "Camera.h"
#include "BaseEnemy.h"
#include "TestEnemy.h"

namespace lbe
{
	class StateMachine;

	class GameState : public State
	{
	private:
		const int TILEMAP_SIZE_X = 5;
		const int TILEMAP_SIZE_Y = 5;
		sf::Font AsimovFont;
		sf::Text FpsText;
		int FrameCounter = 0;
		sf::Clock Clock;
		sf::Clock Ticker;
		sf::Time dt;
		sf::Vector2f MovementInput = sf::Vector2f(0, 0);
		double MovementSpeed = 400.f;
		bool bKeyPressedW = false;
		bool bKeyPressedA = false;
		bool bKeyPressedS = false;
		bool bKeyPressedD = false;
		sf::View DefaultView;
		lbe::Camera FollowCamera;
		sf::Vector2f CurrentMousePos = sf::Vector2f(0, 0);
	public:
		static lbe::Player* LocalPlayer;

	public:
		GameState(StateMachine& machine, sf::RenderWindow& window, bool replace = true);
		~GameState();

		void Pause();
		void Resume();

		void Update();
		void ParseInput();
		void Draw();

		static void CreateLocalPlayer(sf::Vector2f Pos, sf::Uint32 UniqueID);
	};
}
