#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "TextBox.h"

namespace lbe
{
	class SaveLevelPanel
	{
	private:
		static bool bWindowIsActive;
		lbe::TextBox *CurrentlySelectedTextBox = nullptr;

		sf::RectangleShape Background;
		sf::Font Font;
		std::vector<lbe::Button> Buttons;
		std::vector<sf::Text> DisplayTexts;
		sf::Text SaveText;
		sf::Text AnchorText;
		static lbe::TextBox AnchorPointX;
		static lbe::TextBox AnchorPointY;
		sf::Text SizeText;
		static lbe::TextBox MapSizeX;
		static lbe::TextBox MapSizeY;
		sf::Text LevelNameText;
		static lbe::TextBox LevelName;
		lbe::Button SaveButton;
		lbe::Button ExitButton;

	public:
		SaveLevelPanel();
		SaveLevelPanel(sf::Font &Font);
		~SaveLevelPanel();

		void Draw(sf::RenderWindow &Window);
		void SetPosition(sf::Vector2f NewPos);
		void CreateWidgets();
		static void SetTextBoxValues(sf::Vector2i AnchorPoint, sf::Vector2i Size, std::string LevelName);
		static void SetbWindowIsActive(bool NewValue);
		static bool GetbIsWindowActive();
		bool bIsPointInBounds(sf::Vector2f Point);
		void Click(sf::Vector2f MousePos);
		void EnterText(sf::Uint32 Unicode);
	};
}
