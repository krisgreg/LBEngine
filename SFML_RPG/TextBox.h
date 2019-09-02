#pragma once
#include <SFML/Graphics.hpp>

namespace lbe
{
	class TextBox
	{
	private:
		sf::RectangleShape Background;
		sf::Text Text;
		sf::Font Font;
	public:
		TextBox();
		TextBox(sf::Font &TextFont);
		~TextBox();

		void Draw(sf::RenderWindow &window);
		bool bIsPointInBounds(sf::Vector2f Point);
		void SetPosition(sf::Vector2f NewPos);
		void SetSize(sf::Vector2f NewSize);
		void EnterText(sf::Uint32 NewChar);
		void SetText(std::string NewString);
		std::string GetString();
		sf::Vector2f GetSize();
		sf::Vector2f GetPosition();

	private:
		void UpdateTextPosition();
	};
}
