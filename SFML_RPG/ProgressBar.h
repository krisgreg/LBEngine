#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums.h"

namespace lbe
{
	class ProgressBar
	{
	private:
		sf::RectangleShape BackgroundBar;
		sf::RectangleShape ForegroundBar;
		double FillPercentage = 0.5f;
		lbe::Orientation Style = lbe::Orientation::Horizontal;

	public:
		ProgressBar();
		~ProgressBar();

		void SetColors(sf::Color NewBackgroundColor, sf::Color NewForegroundColor);
		void SetFillPercentage(double NewFillPercentage);
		void SetOrientation(lbe::Orientation NewOrientation);
		void Draw(sf::RenderWindow &window);
	};
}