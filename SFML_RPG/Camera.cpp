#include "Camera.h"
#include "Consts.h"
#include <iostream>

lbe::Camera::Camera()
{
	CameraView.setCenter(Pos);
	CameraView.setSize(SCREEN_SIZE);
}


lbe::Camera::~Camera()
{
}

void lbe::Camera::Move(sf::Vector2f MovementInput, sf::Time dt)
{
	Pos += (MovementInput * dt.asSeconds());
	CameraView.setCenter(Pos);
	//std::cout << "CameraMove()\n";
}

void lbe::Camera::Follow(sf::Vector2f TargetPos, sf::Time dt)
{
	//std::cout << "Pos.x=" << Pos.x << " Target.x=" << TargetPos.x << " NewPos.x=" << Pos.x * .5f + TargetPos.x * .5f << std::endl;
	Pos = Pos * (1.f - dt.asSeconds() * FollowSpeed) + TargetPos * (dt.asSeconds() * FollowSpeed);
	//Pos = Pos * .5f + TargetPos * .5f;
	//std::cout << "Pos.x=" << Pos.x << " Target.x=" << TargetPos.x << std::endl;
	CameraView.setCenter(Pos);
}

void lbe::Camera::SetPlayerView(sf::RenderWindow & window)
{
	window.setView(CameraView);
}

sf::Vector2f lbe::Camera::GetPos()
{
	return Pos;
}

void lbe::Camera::SetPos(sf::Vector2f NewPos)
{
	Pos = NewPos;
	CameraView.setCenter(Pos);
	//std::cout << "CameraSetPos()\n";
}

void lbe::Camera::SetPos(sf::Vector2i NewPos)
{
	Pos = sf::Vector2f(NewPos.x, NewPos.y);
	CameraView.setCenter(Pos);
	//std::cout << "CameraSetPos()\n";
}

void lbe::Camera::UpdateView(sf::RenderWindow & Window)
{
	CameraView.setSize(sf::Vector2f(Window.getSize().x, Window.getSize().y));
}

void lbe::Camera::ZoomView(float DistanceScrolled)
{
	CameraView.zoom(1.f + (-DistanceScrolled * 0.1f));
}

sf::Vector2f lbe::Camera::GetViewSize()
{
	return CameraView.getSize();
}

void lbe::Camera::DebugCameraViewSize()
{
	LBE_TRACE("CameraView: x={0:02.3f} y={1:02.3f}", CameraView.getSize().x, CameraView.getSize().y);
}
