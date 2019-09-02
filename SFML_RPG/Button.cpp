#include "Button.h"

#include "Consts.h"
#include "TextureLoader.h"

lbe::Button::Button()
{
	LoadFont();

	Body.setPosition(0, 0);
	Body.setSize(sf::Vector2f(200, 50));
	Body.setTexture(&lbe::TextureLoader::ButtonTextures);
	Body.setTextureRect(sf::IntRect(0, 0, 140, 40));
}


lbe::Button::Button(sf::Text NewButtonText, std::function<bool()> onClickFunction) : onClick(onClickFunction), State(lbe::ButtonState::Deselected), ButtonText(NewButtonText)
{
	Body.setPosition(0, 0);
	Body.setSize(sf::Vector2f(200, 50));
	Body.setTexture(&lbe::TextureLoader::ButtonTextures);
	Body.setTextureRect(sf::IntRect(0, 0, 140, 40));

	UpdateTextPosition();
}

lbe::Button::Button(std::string NewButtonString, sf::Font &NewButtonFont, std::function<bool()> onClickFunction) : onClick(onClickFunction), State(lbe::ButtonState::Deselected)
{
	ButtonText.setFont(NewButtonFont);
	ButtonText.setCharacterSize(20);
	ButtonText.setFillColor(sf::Color::White);
	ButtonText.setString(NewButtonString.c_str());

	Body.setPosition(0, 0);
	Body.setSize(sf::Vector2f(200, 50));
	Body.setTexture(&lbe::TextureLoader::ButtonTextures);
	Body.setTextureRect(sf::IntRect(0, 0, 140, 40));

	UpdateTextPosition();
}

lbe::Button::~Button()
{
	onClick = []() { return true; };
}

void lbe::Button::select()
{
	State = lbe::ButtonState::Selected;
	if (bDisplayOutlineOnClick)
	{
		Body.setOutlineThickness(5);
		Body.setOutlineColor(sf::Color::White);
	}
}

void lbe::Button::deselect()
{
	State = lbe::ButtonState::Deselected;
	if (bDisplayOutlineOnClick)
	{
		//Body.setOutlineThickness(5);
		Body.setOutlineColor(sf::Color::Transparent);
	}
}

bool lbe::Button::click()
{
	State = lbe::ButtonState::Clicked;
	if (bDisplayOutlineOnClick)
	{
		Body.setOutlineThickness(5);
		Body.setOutlineColor(sf::Color::White);
	}
	return onClick();
}

void lbe::Button::setOnClickFunction(std::function<bool()> onClickFunction)
{
	onClick = onClickFunction;
}

void lbe::Button::Draw(sf::RenderWindow &window)
{
	window.draw(Body);
	window.draw(ButtonText);
}

bool lbe::Button::bIsPointInBounds(sf::Vector2f Point)
{
	return Body.getGlobalBounds().contains(Point);
}

void lbe::Button::SetButtonColor(sf::Color NewColor)
{
	Body.setFillColor(NewColor);
}

void lbe::Button::SetTextColor(sf::Color NewColor)
{
	ButtonText.setFillColor(NewColor);
}

void lbe::Button::SetTextObject(sf::Text NewTextObject)
{
	ButtonText = NewTextObject;
	UpdateTextPosition();
}

void lbe::Button::SetButtonText(std::string NewButtonText)
{
	ButtonText.setString(NewButtonText);
	UpdateTextPosition();
}

void lbe::Button::SetPosition(sf::Vector2f NewPos)
{
	Body.setPosition(NewPos);
	UpdateTextPosition();
}

void lbe::Button::SetOnClickFunction(std::function<bool()> NewOnClickFunction)
{
	onClick = NewOnClickFunction;
}

void lbe::Button::SetTexture(sf::Texture *TextureMap, sf::Vector2f TexturePos)
{
	//std::cout << "x=" << TexturePos.x << " y=" << TexturePos.y << std::endl;
	Body.setFillColor(sf::Color::White);
	Body.setTexture(TextureMap);
	Body.setTextureRect(sf::IntRect(TexturePos.x, TexturePos.y, lbe::TILE_TEXTURE_SIZE, lbe::TILE_TEXTURE_SIZE));
}

void lbe::Button::SetTexture(sf::Texture * TextureMap, sf::IntRect TextureRect)
{
	Body.setFillColor(sf::Color::White);
	Body.setTexture(TextureMap);
	Body.setTextureRect(TextureRect);
}

void lbe::Button::SetSize(sf::Vector2f NewSize)
{
	Body.setSize(NewSize);
	UpdateTextPosition();
}

void lbe::Button::SetbDisplayOutlineOnClick(bool NewValue)
{
	bDisplayOutlineOnClick = NewValue;
}

void lbe::Button::SetTextureRect(sf::IntRect NewTextureRect)
{
	Body.setTextureRect(NewTextureRect);
}

void lbe::Button::LoadFont()
{
	if (!ButtonTextFont.loadFromFile("./assets/fonts/Asimov.otf"))
	{
		LBE_WARN("Could not load font file Asimov.otf");
	}
}

void lbe::Button::UpdateTextPosition()
{
	ButtonText.setOrigin(sf::Vector2f((ButtonText.getLocalBounds().width / 2), (ButtonText.getLocalBounds().height / 2)));
	ButtonText.setPosition((Body.getSize().x / 2) + Body.getPosition().x, (Body.getSize().y / 2) + Body.getPosition().y);
}
