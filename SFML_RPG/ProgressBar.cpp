#include "ProgressBar.h"


lbe::ProgressBar::ProgressBar()
{
	BackgroundBar.setFillColor(sf::Color::White);
	BackgroundBar.setSize(sf::Vector2f(200, 40));
	BackgroundBar.setPosition(0, 100);
	ForegroundBar.setFillColor(sf::Color::Red);
	ForegroundBar.setSize(sf::Vector2f(BackgroundBar.getSize().x * FillPercentage, 40));
	ForegroundBar.setPosition(0, 100);
}


lbe::ProgressBar::~ProgressBar()
{
}

void lbe::ProgressBar::SetColors(sf::Color NewBackgroundColor, sf::Color NewForegroundColor)
{
	BackgroundBar.setFillColor(NewBackgroundColor);
	ForegroundBar.setFillColor(NewForegroundColor);
}

void lbe::ProgressBar::SetFillPercentage(double NewFillPercentage)
{
	FillPercentage = NewFillPercentage;
	ForegroundBar.setSize(sf::Vector2f(BackgroundBar.getSize().x * FillPercentage, 40));
}

void lbe::ProgressBar::SetOrientation(lbe::Orientation NewOrientation)
{
	sf::Vector2f OldBackgroundSize = BackgroundBar.getSize();
	//sf::Vector2f OldForegroundSize = ForegroundBar.getSize();
	BackgroundBar.setSize(sf::Vector2f(OldBackgroundSize.x, OldBackgroundSize.y));
	ForegroundBar.setSize(sf::Vector2f(OldBackgroundSize.x, OldBackgroundSize.y * FillPercentage));
}

void lbe::ProgressBar::Draw(sf::RenderWindow &window)
{
	window.draw(BackgroundBar);
	window.draw(ForegroundBar);
}
