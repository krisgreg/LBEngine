#pragma once
#include <SFML/Graphics.hpp>

namespace lbe
{
	class Camera
	{
	private:
		sf::View CameraView;
		sf::Vector2f Pos;
		int FollowSpeed = 5;

	public:
		Camera();
		~Camera();

		void Move(sf::Vector2f MovementInput, sf::Time dt);
		void Follow(sf::Vector2f TargetPos, sf::Time dt);
		void SetPlayerView(sf::RenderWindow &window);
		sf::Vector2f GetPos();
		void SetPos(sf::Vector2f NewPos);
		void SetPos(sf::Vector2i NewPos);
		void UpdateView(sf::RenderWindow& Window);
		void ZoomView(float DistanceScrolled);
		sf::Vector2f GetViewSize();

		void DebugCameraViewSize();
	};
}