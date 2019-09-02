#pragma once
#include "State.h"

#include <SFML/Graphics.hpp>

#include "Button.h"
#include "TextBox.h"
#include "LevelManager.h"
#include "Camera.h"
#include "LevelEditorSidePanel.h"
#include "SaveLevelPanel.h"

namespace lbe
{
	class StateMachine;

	class LevelEditorState : public State
	{
	private:
		sf::Font AsimovFont;
		sf::Text FpsText;
		int FrameCounter = 0;
		sf::Clock Clock;
		sf::Clock Ticker;
		sf::Time dt;
		sf::Vector2f MovementInput = sf::Vector2f(0, 0);
		double MovementSpeed = 800.f;
		bool bKeyPressedW = false;
		bool bKeyPressedA = false;
		bool bKeyPressedS = false;
		bool bKeyPressedD = false;
		bool bLMBPressed = false;
		bool bRMBPressed = false;
		bool bMMBPressed = false;
		sf::View DefaultView;
		lbe::Button OpenLevelButton;
		std::string CurrentOpenLevel;
		lbe::Camera Camera;
		lbe::LevelEditorSidePanel SidePanel;
		lbe::TextBox LevelInput;
		lbe::TextBox *CurrentlySelectedTextBox = nullptr;
		lbe::SaveLevelPanel SaveLevelPanel;
		StateMachine& Machine;
		sf::RenderWindow& Window;
		lbe::Button SaveLevelButton;
		sf::Vector2f MousePos;
		sf::Vector2f PreviousMousePos;

	public:
		LevelEditorState(StateMachine & machine, sf::RenderWindow & window, bool replace);
		~LevelEditorState();

		void Pause();
		void Resume();

		void Update();
		void ParseInput();
		void Draw();

		void UpdateWidgets();

		void OpenLevel(std::string LevelName);
		void SetSaveLevelPanelbWindowIsActive(bool NewValue);
	};
}
