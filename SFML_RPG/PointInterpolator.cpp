#include "PointInterpolator.h"
#include <iostream>;


lbe::PointInterpolator::PointInterpolator()
{
}


lbe::PointInterpolator::~PointInterpolator()
{
}

void lbe::PointInterpolator::AddPoint(sf::Vector2f NewPoint)
{
	Points.push_back(NewPoint);
}

sf::Vector2f lbe::PointInterpolator::GetInterpolatedPoint()
{
	std::vector<sf::Vector2f> CurrentPoints = Points;
	std::vector<sf::Vector2f> NewPoints;

	while (CurrentPoints.size() > 1)
	{
		//std::cout << "CurrentPoints.size()=" << CurrentPoints.size() << std::endl;
		NewPoints.clear();
		//std::cout << "CurrentPoints.size()=" << CurrentPoints.size() << std::endl;

		using Iter = std::vector<sf::Vector2f>::const_iterator;
		for (Iter it = CurrentPoints.begin(); it != CurrentPoints.end(); it++)
		{
			if ((it+1) != CurrentPoints.end())
			{
				//std::cout << "Loop\n";
				sf::Vector2f NewPoint(it->x * (1 - AnimationState) + (it + 1)->x * AnimationState, it->y * (1 - AnimationState) + (it + 1)->y * AnimationState);
				//std::cout << "it= " << it->x << " it+1=" << (it + 1)->x << std::endl;
				NewPoints.push_back(NewPoint);
			}
		}
		//std::cout << "NewPoints.size()=" << NewPoints.size() << std::endl;
		//std::cout << "CurrentPoints.size()=" << CurrentPoints.size() << std::endl;
		
		CurrentPoints = NewPoints;

		//std::cout << "CurrentPoints.size()=" << CurrentPoints.size() << std::endl;
	}

	return *CurrentPoints.begin();
}

bool lbe::PointInterpolator::UpdateAnimationState(sf::Time dt)
{
	AnimationState += dt.asSeconds() * 4.f;
	if (AnimationState >= 1.f)
	{
		AnimationState = 1.f;
		return false;
	}
	return true;
}

double lbe::PointInterpolator::GetAnimationState()
{
	return AnimationState;
}

void lbe::PointInterpolator::ResetAnimationState()
{
	AnimationState = 0.f;
}

void lbe::PointInterpolator::ClearPoints()
{
	Points.clear();
}
