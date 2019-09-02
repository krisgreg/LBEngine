#include "ImGuiTestState.h"

#include <ImGui\imgui.h>
#include <ImGui\imgui-SFML.h>

#include "StateMachine.h"
#include "Log.h"

lbe::ImGuiTestState::ImGuiTestState(StateMachine & machine, sf::RenderWindow & window, bool replace)
	: State{ machine, window, replace }, Window(window), Machine(machine)
{
	LBE_INFO("Loading ImGuiTestState..");

	Camera.UpdateView(Window);
	DefaultView.reset(sf::FloatRect(0, 0, Window.getSize().x, Window.getSize().y));

	ImGui::NewFrame();
}


lbe::ImGuiTestState::~ImGuiTestState()
{
	ImGui::EndFrame();
}

void lbe::ImGuiTestState::Pause()
{
	LBE_INFO("MenuState Pause");
}

void lbe::ImGuiTestState::Resume()
{
	LBE_INFO("MenuState Resume");
}

void lbe::ImGuiTestState::Update()
{
	dt = Clock.restart();

	sf::Event event;
	while (Window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		if (event.type == sf::Event::Closed)
		{
			Machine.Quit();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Machine.LastState();
			}
		}
	}

	ImGui::SFML::Update(Window, dt);
	ImGui::ShowDemoWindow();
	ImGui::Begin("Sample window");
	if (ImGui::Button("Click me"))
	{
		LBE_WARN("Button was clicked!");
	}
	ImGui::End();
}

void lbe::ImGuiTestState::Draw()
{
	Window.clear();

	Camera.SetPlayerView(Window);

	Window.setView(DefaultView);

	ImGui::SFML::Render(Window);

	Window.display();
}