#include "TextBox.h"
#include <iostream>
#include <sstream>

lbe::TextBox::TextBox()
{
	Background.setFillColor(sf::Color::White);
	Background.setPosition(0, 0);
	Background.setSize(sf::Vector2f(200, 50));

	UpdateTextPosition();
}

lbe::TextBox::TextBox(sf::Font & TextFont)
{
	Background.setFillColor(sf::Color::White);
	Background.setPosition(0, 0);
	Background.setSize(sf::Vector2f(200, 50));

	Text.setFont(TextFont);
	Text.setCharacterSize(20);
	Text.setFillColor(sf::Color::Black);

	UpdateTextPosition();
}


lbe::TextBox::~TextBox()
{
}

void lbe::TextBox::Draw(sf::RenderWindow & window)
{
	window.draw(Background);
	window.draw(Text);
}

bool lbe::TextBox::bIsPointInBounds(sf::Vector2f Point)
{
	return Background.getGlobalBounds().contains(Point);
}

void lbe::TextBox::SetPosition(sf::Vector2f NewPos)
{
	Background.setPosition(NewPos);
	UpdateTextPosition();
}

void lbe::TextBox::SetSize(sf::Vector2f NewSize)
{
	Background.setSize(NewSize);
}

void lbe::TextBox::EnterText(sf::Uint32 NewChar)
{
	if (NewChar >= 128 || NewChar == 27 || NewChar == 13)
	{
		return;
	}
	else if (NewChar == 8)
	{
		if (Text.getString() != "")
		{
			Text.setString(Text.getString().substring(0, Text.getString().getSize() - 1));
		}
		return;
	}
	else
	{
		//std::cout << "Textstring updated with: " << static_cast<char>(NewChar) << std::endl;
		std::string CurrentText(Text.getString());
		CurrentText += static_cast<char>(NewChar);
		//std::cout << "New Text: " << CurrentText <<std::endl;
		Text.setString(CurrentText);
		UpdateTextPosition();
		return;
	}
}

void lbe::TextBox::SetText(std::string NewString)
{
	Text.setString(NewString);
	UpdateTextPosition();
}

std::string lbe::TextBox::GetString()
{
	return Text.getString();
}

sf::Vector2f lbe::TextBox::GetSize()
{
	return Background.getSize();
}

sf::Vector2f lbe::TextBox::GetPosition()
{
	return Background.getPosition();
}

void lbe::TextBox::UpdateTextPosition()
{
	//Text.setOrigin(sf::Vector2f(0, Text.getLocalBounds().height / 2));
	Text.setPosition(Background.getPosition().x, (Background.getSize().y / 2) + Background.getPosition().y - 10);
}
