#pragma once
#include <vector>
#include <SFML\Graphics.hpp>

namespace lbe
{
	class PointInterpolator
	{
	private:
		std::vector<sf::Vector2f> Points;
		double AnimationState = 0;

	public:
		PointInterpolator();
		~PointInterpolator();

		void AddPoint(sf::Vector2f NewPoint);
		sf::Vector2f GetInterpolatedPoint();
		bool UpdateAnimationState(sf::Time dt);
		double GetAnimationState();
		void ResetAnimationState();

		void ClearPoints();
	};
}

