#include "LevelEditorState.h"

#include <sstream>

#include "StateMachine.h"
#include "Consts.h"
#include "TextureLoader.h"
#include "SceneObjectFactory.h"
#include "SceneObjectPlaceableInEditor.h"

lbe::LevelEditorState::LevelEditorState(StateMachine & machine, sf::RenderWindow & window, bool replace)
	: State{ machine, window, replace }, Window(window), Machine(machine)
{
	LBE_INFO("Loading LevelEditorState..");

	if (!AsimovFont.loadFromFile("./assets/fonts/Asimov.otf"))
	{
		LBE_WARN("Could not load font file Asimov.otf");
	}

	auto OpenLevelButtonOnClick = [this]
	{
		OpenLevel(LevelInput.GetString());
		return false;
	};

	OpenLevelButton = lbe::Button("Open Level..", AsimovFont, OpenLevelButtonOnClick);
	OpenLevelButton.SetPosition(sf::Vector2f(window.getSize().x - 200, window.getSize().y - 50));

	auto SaveLevelButtonOnClick = [this]
	{
		SetSaveLevelPanelbWindowIsActive(true);
		return false;
	};

	SaveLevelButton = lbe::Button("Save Level", AsimovFont, SaveLevelButtonOnClick);
	SaveLevelButton.SetPosition(sf::Vector2f(window.getSize().x - 200, window.getSize().y - 150));

	FpsText.setFont(AsimovFont);
	FpsText.setCharacterSize(20);
	FpsText.setFillColor(sf::Color::Green);
	FpsText.setOutlineThickness(1);
	FpsText.setOutlineColor(sf::Color::Black);

	LevelInput = lbe::TextBox(AsimovFont);
	LevelInput.SetPosition(sf::Vector2f(window.getSize().x - 200, window.getSize().y - 100));
	if (lbe::DEBUG_LEVEL_EDITOR)
	{
		LevelInput.SetText("demoZone1");
	}

	Camera.UpdateView(Window);
	DefaultView.reset(sf::FloatRect(0, 0, Window.getSize().x, Window.getSize().y));
	
	LevelManager::LoadLevels();

	SidePanel = lbe::LevelEditorSidePanel(sf::Vector2f(Window.getSize().x, Window.getSize().y), AsimovFont, &TextureLoader::TileTextureMap);

	SaveLevelPanel = lbe::SaveLevelPanel(AsimovFont);
	SaveLevelPanel.SetPosition(sf::Vector2f(Window.getSize().x / 2, Window.getSize().y / 2));
}


lbe::LevelEditorState::~LevelEditorState()
{
}

void lbe::LevelEditorState::Pause()
{
	LBE_INFO("MenuState Pause");
}

void lbe::LevelEditorState::Resume()
{
	LBE_INFO("MenuState Resume");
	UpdateWidgets();
}

void lbe::LevelEditorState::Update()
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

	MovementInput = sf::Vector2f(0, 0);
	ParseInput();

	if (!CurrentlySelectedTextBox)
	{
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
	}

	if (bLMBPressed)
	{
		sf::Vector2f ScreenMiddleToCornerOffset(-(double)Camera.GetViewSize().x / 2, -(double)Camera.GetViewSize().y / 2);
		sf::Vector2f WorldMousePos = Camera.GetPos() + ScreenMiddleToCornerOffset + MousePos * (Camera.GetViewSize().x / Window.getSize().x);

		if (lbe::LevelEditorSidePanel::GetSelectedLayer() == lbe::Layer::Background)
		{
			LevelManager::PaintTile(CurrentOpenLevel, WorldMousePos, SidePanel.GetSelectedTextureId(), lbe::Layer::Background);
		}
		else if (lbe::LevelEditorSidePanel::GetSelectedLayer() == lbe::Layer::Collision)
		{
			LevelManager::PaintTile(CurrentOpenLevel, WorldMousePos, 1, lbe::Layer::Collision);
		}
		else if (lbe::LevelEditorSidePanel::GetSelectedLayer() == lbe::Layer::Meta)
		{
			LevelManager::PaintTile(CurrentOpenLevel, WorldMousePos, 0, lbe::Layer::Meta);
		}
	}
	else if (bRMBPressed)
	{
		//LBE_TRACE("MousePos: x={0:02.3f} y={1:02.3f}", MousePos.x, MousePos.y);
		sf::Vector2f ScreenMiddleToCornerOffset(-(double)Camera.GetViewSize().x / 2, -(double)Camera.GetViewSize().y / 2);
		sf::Vector2f WorldMousePos = Camera.GetPos() + ScreenMiddleToCornerOffset + MousePos * (Camera.GetViewSize().x / Window.getSize().x);
		//LBE_TRACE("WorldMousePos: x={0:02.3f} y={1:02.3f}", WorldMousePos.x, WorldMousePos.y);

		if (lbe::LevelEditorSidePanel::GetSelectedLayer() == lbe::Layer::Background)
		{
			LevelManager::PaintTile(CurrentOpenLevel, WorldMousePos, 0, lbe::Layer::Background);
		}
		else if (lbe::LevelEditorSidePanel::GetSelectedLayer() == lbe::Layer::Collision)
		{
			LevelManager::PaintTile(CurrentOpenLevel, WorldMousePos, 0, lbe::Layer::Collision);
		}
		else if (lbe::LevelEditorSidePanel::GetSelectedLayer() == lbe::Layer::Object)
		{
			for (auto &i : lbe::LevelManager::GetSceneObjectsFromLevel(WorldMousePos))
			{
				if (SceneObjectPlaceableInEditor* Object = dynamic_cast<SceneObjectPlaceableInEditor*>(i))
				{
					if (Object->IsPointInHitbox(WorldMousePos))
					{
						Object->MarkForDeletion();
						LevelManager::DeleteMarkedSceneObjects();
					}
				}
			}
		}
		else if (lbe::LevelEditorSidePanel::GetSelectedLayer() == lbe::Layer::Meta)
		{
			LevelManager::PaintTile(CurrentOpenLevel, WorldMousePos, 1, lbe::Layer::Meta);
		}
	}

	if (bMMBPressed)
	{
		sf::Vector2f WorldMousePos = MousePos * (Camera.GetViewSize().x / Window.getSize().x);
		Camera.SetPos(Camera.GetPos() + (PreviousMousePos - WorldMousePos));
		PreviousMousePos = WorldMousePos;
	}
	
	Camera.Move(MovementInput, dt);
	//Camera.DebugCameraViewSize();
}

void lbe::LevelEditorState::ParseInput()
{
	sf::Event event;
	while (Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			Machine.Quit();
			//LBE_TRACE("Machine.Quit()");
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			MousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
			if (LevelInput.bIsPointInBounds(MousePos))
			{
				CurrentlySelectedTextBox = &LevelInput;
			}
			else
			{
				CurrentlySelectedTextBox = nullptr;
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (SaveLevelPanel.bIsPointInBounds(MousePos) && SaveLevelPanel.GetbIsWindowActive())
					{
						SaveLevelPanel.Click(MousePos);
					}
					else if (SidePanel.bIsPointInBounds(MousePos))
					{
						SidePanel.Click(MousePos);
					}
					else if (OpenLevelButton.bIsPointInBounds(MousePos))
					{
						OpenLevelButton.click();
					}
					else if (SaveLevelButton.bIsPointInBounds(MousePos))
					{
						SaveLevelButton.click();
					}
					else if (lbe::LevelEditorSidePanel::GetSelectedLayer() == lbe::Layer::Object)
					{
						sf::Vector2f ScreenMiddleToCornerOffset(-(double)Camera.GetViewSize().x / 2, -(double)Camera.GetViewSize().y / 2);
						sf::Vector2f WorldMousePos = Camera.GetPos() + ScreenMiddleToCornerOffset + MousePos * (Camera.GetViewSize().x / Window.getSize().x);
						lbe::LevelManager::AddSceneObject(lbe::SceneObjectFactory::CreateInstance(lbe::LevelEditorSidePanel::GetSelectedObjectName(), WorldMousePos), WorldMousePos);
					}
					else
					{
						bLMBPressed = true;
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					bRMBPressed = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Middle)
				{
					bMMBPressed = true;
					sf::Vector2f WorldMousePos = MousePos * (Camera.GetViewSize().x / Window.getSize().x);
					PreviousMousePos = WorldMousePos;
				}
			}
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			MousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				bLMBPressed = false;
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				bRMBPressed = false;
			}
			else if (event.mouseButton.button == sf::Mouse::Middle)
			{
				bMMBPressed = false;
			}
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
				lbe::LevelEditorSidePanel::SetSelectedLayer(lbe::Layer::Background);
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
		else if (event.type == sf::Event::TextEntered)
		{
			if (CurrentlySelectedTextBox)
			{
				CurrentlySelectedTextBox->EnterText(event.text.unicode);
			}
			else if (SaveLevelPanel.GetbIsWindowActive())
			{
				SaveLevelPanel.EnterText(event.text.unicode);
			}
		}
		else if (event.type == sf::Event::MouseWheelScrolled)
		{
			Camera.ZoomView(event.mouseWheelScroll.delta);
		}
		else if (event.type == sf::Event::Resized)
		{
			UpdateWidgets();
		}
	}
}

void lbe::LevelEditorState::Draw()
{
	Window.clear();

	Camera.SetPlayerView(Window);
	LevelManager::DrawLevel(Window, CurrentOpenLevel);

	Window.setView(DefaultView);
	SidePanel.Draw(Window);
	OpenLevelButton.Draw(Window);
	LevelInput.Draw(Window);
	SaveLevelButton.Draw(Window);
	SaveLevelPanel.Draw(Window);
	Window.draw(FpsText);

	Window.display();

	FrameCounter++;
}

void lbe::LevelEditorState::UpdateWidgets()
{
	Camera.UpdateView(Window);
	DefaultView.reset(sf::FloatRect(0, 0, Window.getSize().x, Window.getSize().y));
	OpenLevelButton.SetPosition(sf::Vector2f(Window.getSize().x - 200, Window.getSize().y - 50));
	SaveLevelPanel.SetPosition(sf::Vector2f(Window.getSize().x / 2, Window.getSize().y / 2));
	SidePanel = lbe::LevelEditorSidePanel(sf::Vector2f(Window.getSize().x, Window.getSize().y), AsimovFont, &TextureLoader::TileTextureMap);
	LevelInput.SetPosition(sf::Vector2f(Window.getSize().x - 200, Window.getSize().y - 100));
	SaveLevelButton.SetPosition(sf::Vector2f(Window.getSize().x - 200, Window.getSize().y - 150));
}

void lbe::LevelEditorState::OpenLevel(std::string LevelName)
{
	CurrentOpenLevel = LevelName;
	Camera.SetPos(LevelManager::GetLevelAnchorPointByName(LevelName) + sf::Vector2i(Window.getSize().x / 4, Window.getSize().y / 2));
	lbe::SaveLevelPanel::SetTextBoxValues(LevelManager::GetLevelAnchorPointByName(LevelName) / lbe::TILE_SIZE, LevelManager::GetLevelSizeByName(LevelName), LevelName);
}

void lbe::LevelEditorState::SetSaveLevelPanelbWindowIsActive(bool NewValue)
{
	SaveLevelPanel.SetbWindowIsActive(NewValue);
}