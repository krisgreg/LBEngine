#include "MenuState.h"

#include "GameState.h"
#include "LevelEditorState.h"
#include "ImGuiTestState.h"

#include "StateMachine.h"
#include "Log.h"
#include "NetworkManager.h"

lbe::MenuState::MenuState(StateMachine & machine, sf::RenderWindow & window, bool replace)
	: State{ machine, window, replace }, Window(window), Machine(machine)
{
	LBE_INFO("Loading MenuState..");

	if (!AsimovFont.loadFromFile("./assets/fonts/Asimov.otf"))
	{
		std::cout << "Could not load font file Asimov.otf" << std::endl;
	}

	CreateWidgets(Machine, Window);
}

lbe::MenuState::~MenuState()
{
}

void lbe::MenuState::Pause()
{
	LBE_INFO("MenuState Pause");
}

void lbe::MenuState::Resume()
{
	LBE_INFO("MenuState Resume");
	CreateWidgets(Machine, Window);
}

void lbe::MenuState::Update()
{
	sf::Event event;
	while (Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			Machine.Quit();
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				for (auto &button : Buttons)
				{
					if (button.bIsPointInBounds(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
					{
						button.click();
					}
				}
			}
		}
		else if (event.type == sf::Event::Resized)
		{
			CreateWidgets(Machine, Window);
		}
	}
}

void lbe::MenuState::Draw()
{
	Window.clear();

	Window.setView(DefaultView);
	Window.draw(MainMenuHeadline);
	for (auto &button : Buttons)
	{
		button.Draw(Window);
	}

	Window.display();
}

void lbe::MenuState::CreateWidgets(StateMachine& machine, sf::RenderWindow& window)
{
	DefaultView.reset(sf::FloatRect(0, 0, Window.getSize().x, Window.getSize().y));

	Buttons.clear();

	auto PlayAsHostButtonOnClick = [&machine, &window]
	{
		NetworkManager::SetActiveNetworkState(lbe::NetworkState::Host);
		machine.Run(lbe::StateMachine::Build<lbe::GameState>(machine, window, false));
		return false;
	};
	lbe::Button PlayAsHostButton = lbe::Button("Play Game", AsimovFont, PlayAsHostButtonOnClick);
	PlayAsHostButton.SetPosition(sf::Vector2f(window.getSize().x / 2 - 100, 300));
	Buttons.push_back(PlayAsHostButton);

	auto PlayAsClientButtonOnClick = [&machine, &window]
	{
		NetworkManager::SetActiveNetworkState(lbe::NetworkState::Client);
		machine.Run(lbe::StateMachine::Build<lbe::GameState>(machine, window, false));
		return false;
	};
	lbe::Button PlayAsClientButton = lbe::Button("Join Game", AsimovFont, PlayAsClientButtonOnClick);
	PlayAsClientButton.SetPosition(sf::Vector2f(window.getSize().x / 2 - 100, 400));
	Buttons.push_back(PlayAsClientButton);

	auto LevelEditorButtonOnClick = [&machine, &window]
	{
		NetworkManager::SetActiveNetworkState(lbe::NetworkState::Host);
		machine.Run(lbe::StateMachine::Build<lbe::LevelEditorState>(machine, window, false));
		return false;
	};
	lbe::Button LevelEditorButton = lbe::Button("Level Editor", AsimovFont, LevelEditorButtonOnClick);
	LevelEditorButton.SetPosition(sf::Vector2f(window.getSize().x / 2 - 100, 500));
	Buttons.push_back(LevelEditorButton);

	lbe::Button ExitButton = lbe::Button("Exit Game", AsimovFont, [&machine]() -> bool { machine.Quit(); return false; });
	ExitButton.SetPosition(sf::Vector2f(window.getSize().x / 2 - 100, 600));
	Buttons.push_back(ExitButton);

	MainMenuHeadline.setFont(AsimovFont);
	MainMenuHeadline.setCharacterSize(80);
	MainMenuHeadline.setFillColor(sf::Color::Blue);
	MainMenuHeadline.setString("Main Menu");
	MainMenuHeadline.setPosition(sf::Vector2f(window.getSize().x / 2 - 200, 120));
	MainMenuHeadline.setOutlineThickness(2);
	MainMenuHeadline.setOutlineColor(sf::Color::Black);
}
