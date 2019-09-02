#include "GameState.h"
#include "StateMachine.h"
#include "HelperFunctions.h"
#include <iostream>
#include <sstream>

#include "NetworkManager.h"

lbe::Player* lbe::GameState::LocalPlayer;

lbe::GameState::GameState(StateMachine & machine, sf::RenderWindow & window, bool replace)
	: State{ machine, window, replace }
{
	LBE_INFO("Loading GameState..");

	if (!AsimovFont.loadFromFile("./assets/fonts/Asimov.otf"))
	{
		LBE_WARN("Could not load font file Asimov.otf");
	}

	FpsText.setFont(AsimovFont);
	FpsText.setCharacterSize(20);
	FpsText.setFillColor(sf::Color::Green);
	FpsText.setOutlineThickness(1);
	FpsText.setOutlineColor(sf::Color::Black);

	LevelManager::LoadLevels();

	NetworkManager::BindUdpPort();
	if (NetworkManager::GetActiveNetworkState() == NetworkState::Client)
	{
		if (!NetworkManager::ConnectToHost())
		{
			LBE_WARN("Could not connect to host!");
			Machine.LastState();
		}
	}
	else if (NetworkManager::GetActiveNetworkState() == NetworkState::Host)
	{
		NetworkManager::StartListening();
		LocalPlayer = new Player(sf::Vector2f(400, 400));
	}

	LevelManager::AddSceneObject(LocalPlayer, LocalPlayer->GetPosition());

	LocalPlayer->UpdateView(Window);
	FollowCamera.UpdateView(Window);
	DefaultView.reset(sf::FloatRect(0, 0, Window.getSize().x, Window.getSize().y));

	LocalPlayer->UpdateView(Window);

	FollowCamera.SetPos(LocalPlayer->GetPos());


}


lbe::GameState::~GameState()
{
}

void lbe::GameState::Pause()
{
	LBE_INFO("MenuState Pause");
}

void lbe::GameState::Resume()
{
	LBE_INFO("MenuState Resume");
	LocalPlayer->UpdateView(Window);
}

void lbe::GameState::Update()
{
	dt = Clock.restart();

	if (Ticker.getElapsedTime().asSeconds() >= 1.0f)
	{
		std::stringstream FpsString;
		FpsString << "fps=" << FrameCounter;
		FpsText.setString(FpsString.str());
		 
		FrameCounter = 0;
		Ticker.restart();
	}

	if (NetworkManager::GetActiveNetworkState() == NetworkState::Host)
	{
		NetworkManager::ListenForConnections();
	}
	NetworkManager::SendUpdate();
	while (NetworkManager::ReceiveUpdate()) { }
	
	MovementInput = sf::Vector2f(0, 0);
	ParseInput();

	if (bKeyPressedW)
	{
		MovementInput.y -= MovementSpeed;
	}
	if (bKeyPressedA)
	{
		MovementInput.x -= MovementSpeed;
	}
	if (bKeyPressedS)
	{
		MovementInput.y += MovementSpeed;
	}
	if (bKeyPressedD)
	{
		MovementInput.x += MovementSpeed;
	}

	LocalPlayer->Move(MovementInput, dt);
	LocalPlayer->SetPos(LevelManager::HandleCollision(LocalPlayer->GetPos()));

	LevelManager::UpdateAI();
	LevelManager::UpdateSceneObjects(dt);

	//std::cout << "MousePos: X=" << MousePos.x << " Y=" << MousePos.y << std::endl;
	sf::Vector2f ScreenMiddleToCornerOffset(-(double)Window.getSize().x / 2, -(double)Window.getSize().y / 2);
	sf::Vector2f WorldMousePos = FollowCamera.GetPos() + ScreenMiddleToCornerOffset + CurrentMousePos;
	//std::cout << "x=" << WorldMousePos.x << " y=" << WorldMousePos.y << std::endl;
	LocalPlayer->UpdatePlayerRotation(WorldMousePos, dt);

	LocalPlayer->SetHandPos(LevelManager::HandleCollision(LocalPlayer->GetLeftHandPos(), lbe::PLAYER_HAND_RADIUS), LevelManager::HandleCollision(LocalPlayer->GetRightHandPos(), lbe::PLAYER_HAND_RADIUS));
	FollowCamera.Follow(LocalPlayer->GetPos(), dt);

	LocalPlayer->UpdateWeapon();

	if (LocalPlayer->IsDead())
	{
		Machine.LastState();
	}
}

void lbe::GameState::ParseInput()
{
	sf::Event event;
	while (Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			if (NetworkManager::GetActiveNetworkState() == NetworkState::Host)
			{
				NetworkManager::StopListening();
			}
			Machine.Quit();
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				LocalPlayer->StartPerformingAttack();
			}
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			CurrentMousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::W)
			{
				bKeyPressedW = true;
			}
			if (event.key.code == sf::Keyboard::A)
			{
				bKeyPressedA = true;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				bKeyPressedS = true;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				bKeyPressedD = true;
			}
			if (event.key.code == sf::Keyboard::Escape)
			{
				if (NetworkManager::GetActiveNetworkState() == NetworkState::Host)
				{
					NetworkManager::StopListening();
				}
				Machine.LastState();
			}
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::W)
			{
				bKeyPressedW = false;
			}
			if (event.key.code == sf::Keyboard::A)
			{
				bKeyPressedA = false;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				bKeyPressedS = false;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				bKeyPressedD = false;
			}
		}
		else if (event.type == sf::Event::Resized)
		{
			LocalPlayer->UpdateView(Window);
			FollowCamera.UpdateView(Window);
			DefaultView.reset(sf::FloatRect(0, 0, Window.getSize().x, Window.getSize().y));
		}
	}
}

void lbe::GameState::Draw()
{
	Window.clear();

	FollowCamera.SetPlayerView(Window);
	//LevelManager::DrawAllLevels(Window);
	LevelManager::DrawCurrentLevel(Window, LocalPlayer->GetPos());

	Window.setView(DefaultView);
	Window.draw(FpsText);

	Window.display();

	FrameCounter++;
}

void lbe::GameState::CreateLocalPlayer(sf::Vector2f Pos, sf::Uint32 UniqueID)
{
	LocalPlayer = new Player(Pos);
	LocalPlayer->SetUniqueID(UniqueID);
}
