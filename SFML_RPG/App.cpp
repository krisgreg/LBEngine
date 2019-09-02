#include "App.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>

#include "Log.h"
#include "Consts.h"
#include "TextureLoader.h"
#include "SceneObjectFactory.h"

#include "MenuState.h"
#include "GameState.h"
#include "LevelEditorState.h"
#include "ImGuiTestState.h"
#include "NetworkManager.h"

lbe::App::App()
{
}


lbe::App::~App()
{
}

void lbe::App::Run()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	Window.create(sf::VideoMode(lbe::SCREEN_SIZE.x, lbe::SCREEN_SIZE.y), "SFML LBEngine", sf::Style::Default, settings);
	Window.setFramerateLimit(60);

	lbe::TextureLoader::InitTextures();
	lbe::Log::Init();
	lbe::NetworkManager::Init();
	ImGui::SFML::Init(Window);

	Machine.Run(lbe::StateMachine::Build<lbe::MenuState>(Machine, Window, true));

	// Main loop
	while (Machine.Running())
	{
		Machine.NextState();
		Machine.Update();
		Machine.Draw();
	}

	// This line causes a crash when closing the window from LevelEditorState
	//ImGui::SFML::Shutdown();
}