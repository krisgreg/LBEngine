#pragma once
#include <SFML/Graphics.hpp>
#include "Consts.h"

namespace lbe
{
	struct HelperFunctions
	{
		static double GetDistance(sf::Vector2f Pos1, sf::Vector2f Pos2)
		{
			return sqrt(pow(abs(Pos1.x - Pos2.x), 2) + pow(abs(Pos1.y - Pos2.y), 2));
		}
		static sf::Vector2f RotateVector(sf::Vector2f InputVector, double Degrees, bool RotateClockwise = true)
		{
			sf::Vector2f OutputVector;

			double Theta = Degrees * lbe::HalfC;

			double CosT = cos(Theta);
			double SinT = sin(Theta);

			OutputVector.x = InputVector.x * CosT - InputVector.y * SinT;
			OutputVector.y = InputVector.x * SinT + InputVector.y * CosT;

			return OutputVector;
		}
		static double GetRotation(sf::Vector2f Vector)
		{
			sf::Vector2f ZeroDegreeVector(0, -1);
			double Dot = Vector.x * ZeroDegreeVector.x + Vector.y * ZeroDegreeVector.y;
			double Determinant = ZeroDegreeVector.x * Vector.y - ZeroDegreeVector.y * Vector.x;
			double Angle = atan2(Determinant, Dot);
			Angle = (Angle / lbe::PI) * 360 / 2;
			//std::cout << "Angle=" << Angle << std::endl;
			return Angle;
		}
	};
}