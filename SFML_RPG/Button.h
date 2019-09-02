#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

#include "Enums.h"

namespace lbe
{
	class Button
	{
	private:
		sf::RectangleShape Body;
		sf::Text ButtonText;
		sf::Font ButtonTextFont;
		lbe::ButtonState State;
		std::function<bool()> onClick;
		bool bDisplayOutlineOnClick = false;

	public:
		Button();
		Button(sf::Text NewButtonText, std::function<bool()> onClickFunction);
		Button(std::string NewButtonString, sf::Font &NewButtonFont, std::function<bool()> onClickFunction);
		~Button();

		void select();
		void deselect();
		bool click();
		void setOnClickFunction(std::function<bool()> onClickFunction);

		void Draw(sf::RenderWindow &window);
		bool bIsPointInBounds(sf::Vector2f Point);
		void SetButtonColor(sf::Color NewColor);
		void SetTextColor(sf::Color NewColor);
		void SetTextObject(sf::Text NewTextObject);
		void SetButtonText(std::string);
		void SetPosition(sf::Vector2f NewPos);
		void SetOnClickFunction(std::function<bool()> NewOnClickFunction);
		void SetTexture(sf::Texture *TextureMap, sf::Vector2f TexturePos);
		void SetTexture(sf::Texture *TextureMap, sf::IntRect TextureRect);
		void SetSize(sf::Vector2f NewSize);
		void SetbDisplayOutlineOnClick(bool NewValue);
		void SetTextureRect(sf::IntRect NewTextureRect);

	private:
		void LoadFont();
		void UpdateTextPosition();
	};
}
