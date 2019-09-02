#include "SaveLevelPanel.h"
#include "LevelManager.h"
#include "Consts.h"

bool lbe::SaveLevelPanel::bWindowIsActive = false;
lbe::TextBox lbe::SaveLevelPanel::AnchorPointX;
lbe::TextBox lbe::SaveLevelPanel::AnchorPointY;
lbe::TextBox lbe::SaveLevelPanel::MapSizeX;
lbe::TextBox lbe::SaveLevelPanel::MapSizeY;
lbe::TextBox lbe::SaveLevelPanel::LevelName;

lbe::SaveLevelPanel::SaveLevelPanel()
{
}

lbe::SaveLevelPanel::SaveLevelPanel(sf::Font &Font) : Font(Font)
{
	Background.setFillColor(lbe::PANEL_COLOR);
	Background.setSize(sf::Vector2f(600, 500));
	Background.setOrigin(sf::Vector2f(Background.getLocalBounds().width / 2, Background.getLocalBounds().height / 2));
	
	SaveText.setFont(Font);
	SaveText.setCharacterSize(40);
	SaveText.setFillColor(sf::Color::White);
	SaveText.setString("Save Level");
	SaveText.setOrigin(SaveText.getLocalBounds().width / 2, 0);
	SaveText.setOutlineThickness(2);
	SaveText.setOutlineColor(sf::Color::Black);

	AnchorText.setFont(Font);
	AnchorText.setCharacterSize(20);
	AnchorText.setFillColor(sf::Color::White);
	AnchorText.setString("Anchor Point:");
	AnchorText.setOutlineThickness(2);
	AnchorText.setOutlineColor(sf::Color::Black);

	AnchorPointX = lbe::TextBox(Font);
	AnchorPointX.SetSize(sf::Vector2f(100, 50));
	
	AnchorPointY = lbe::TextBox(Font);
	AnchorPointY.SetSize(sf::Vector2f(100, 50));

	SizeText.setFont(Font);
	SizeText.setCharacterSize(20);
	SizeText.setFillColor(sf::Color::White);
	SizeText.setString("Map Size:");
	SizeText.setOutlineColor(sf::Color::Black);

	MapSizeX = lbe::TextBox(Font);
	MapSizeX.SetSize(sf::Vector2f(100, 50));

	MapSizeY = lbe::TextBox(Font);
	MapSizeY.SetSize(sf::Vector2f(100, 50));

	LevelNameText.setFont(Font);
	LevelNameText.setCharacterSize(20);
	LevelNameText.setFillColor(sf::Color::White);
	LevelNameText.setString("Level Name:");
	LevelNameText.setOutlineThickness(2);
	LevelNameText.setOutlineColor(sf::Color::Black);

	LevelName = lbe::TextBox(Font);
	LevelName.SetSize(sf::Vector2f(300, 50));


	auto SaveLevelButtonClick = []
	{
		//lbe::LevelManager::SaveLevel(LevelName.GetString(), sf::Vector2i(atoi(AnchorPointX.GetString().c_str()), atoi(AnchorPointY.GetString().c_str())), sf::Vector2i(atoi(MapSizeX.GetString().c_str()), atoi(MapSizeY.GetString().c_str())));
		lbe::LevelManager::SaveLevel(LevelName.GetString());
		lbe::SaveLevelPanel::SetbWindowIsActive(false);
		return false;
	};

	SaveButton = lbe::Button("Save Level", Font, SaveLevelButtonClick);

	auto ExitButtonOnClick = []
	{
		lbe::SaveLevelPanel::SetbWindowIsActive(false);
		return false;
	};

	ExitButton = lbe::Button("X", Font, ExitButtonOnClick);
	ExitButton.SetSize(sf::Vector2f(50, 50));

	SetPosition(sf::Vector2f());
}

lbe::SaveLevelPanel::~SaveLevelPanel()
{
}

void lbe::SaveLevelPanel::Draw(sf::RenderWindow & Window)
{
	if (bWindowIsActive)
	{
		Window.draw(Background);
		Window.draw(SaveText);
		//Window.draw(AnchorText);
		//AnchorPointX.Draw(Window);
		//AnchorPointY.Draw(Window);
		//Window.draw(SizeText);
		//MapSizeX.Draw(Window);
		//MapSizeY.Draw(Window);
		Window.draw(LevelNameText);
		LevelName.Draw(Window);
		SaveButton.Draw(Window);
		ExitButton.Draw(Window);
	}
}

void lbe::SaveLevelPanel::SetPosition(sf::Vector2f NewPos)
{
	Background.setPosition(NewPos);
	SaveText.setPosition(sf::Vector2f(Background.getGlobalBounds().left + (Background.getGlobalBounds().width / 2) , Background.getGlobalBounds().top + 20));
	AnchorText.setPosition(sf::Vector2f(Background.getPosition().x - (Background.getSize().x / 2) + 10, Background.getPosition().y - (Background.getSize().y / 2) + 90));
	AnchorPointX.SetPosition(sf::Vector2f(AnchorText.getPosition().x + AnchorText.getLocalBounds().width + 10, Background.getPosition().y - (Background.getSize().y / 2) + 80));
	AnchorPointY.SetPosition(sf::Vector2f(AnchorPointX.GetPosition().x + AnchorPointX.GetSize().x + 10, Background.getPosition().y - (Background.getSize().y / 2) + 80));
	SizeText.setPosition(sf::Vector2f(Background.getPosition().x - (Background.getSize().x / 2) + 10, Background.getPosition().y - (Background.getSize().y / 2) + 150));
	MapSizeX.SetPosition(sf::Vector2f(SizeText.getPosition().x + SizeText.getLocalBounds().width + 10, Background.getPosition().y - (Background.getSize().y / 2) + 140));
	MapSizeY.SetPosition(sf::Vector2f(MapSizeX.GetPosition().x + MapSizeX.GetSize().x + 10, Background.getPosition().y - (Background.getSize().y / 2) + 140));
	LevelNameText.setPosition(sf::Vector2f(Background.getPosition().x - (Background.getSize().x / 2) + 10, Background.getPosition().y - (Background.getSize().y / 2) + 210));
	LevelName.SetPosition(sf::Vector2f(LevelNameText.getPosition().x + LevelNameText.getLocalBounds().width + 10, Background.getPosition().y - (Background.getSize().y / 2) + 200));
	SaveButton.SetPosition(sf::Vector2f(Background.getGlobalBounds().left + Background.getGlobalBounds().width / 2 - 100, Background.getGlobalBounds().top + Background.getGlobalBounds().height - 70));
	ExitButton.SetPosition(sf::Vector2f(Background.getGlobalBounds().left + Background.getGlobalBounds().width - 60, Background.getGlobalBounds().top + 10));

	CreateWidgets();
}

void lbe::SaveLevelPanel::CreateWidgets()
{
}

void lbe::SaveLevelPanel::SetTextBoxValues(sf::Vector2i AnchorPoint, sf::Vector2i Size, std::string NewLevelName)
{
	AnchorPointX.SetText(std::to_string(AnchorPoint.x));
	AnchorPointY.SetText(std::to_string(AnchorPoint.y));
	MapSizeX.SetText(std::to_string(Size.x));
	MapSizeY.SetText(std::to_string(Size.y));
	LevelName.SetText(NewLevelName);
}

void lbe::SaveLevelPanel::SetbWindowIsActive(bool NewValue)
{
	bWindowIsActive = NewValue;
}

bool lbe::SaveLevelPanel::GetbIsWindowActive()
{
	return bWindowIsActive;
}

bool lbe::SaveLevelPanel::bIsPointInBounds(sf::Vector2f Point)
{
	return Background.getGlobalBounds().contains(Point);
}

void lbe::SaveLevelPanel::Click(sf::Vector2f MousePos)
{
	if (AnchorPointX.bIsPointInBounds(MousePos))
	{
		CurrentlySelectedTextBox = &AnchorPointX;
	}
	else if (AnchorPointY.bIsPointInBounds(MousePos))
	{
		CurrentlySelectedTextBox = &AnchorPointY;
	}
	else if (MapSizeX.bIsPointInBounds(MousePos))
	{
		CurrentlySelectedTextBox = &MapSizeX;
	}
	else if (MapSizeY.bIsPointInBounds(MousePos))
	{
		CurrentlySelectedTextBox = &MapSizeY;
	}
	else if (LevelName.bIsPointInBounds(MousePos))
	{
		CurrentlySelectedTextBox = &LevelName;
	}
	else
	{
		CurrentlySelectedTextBox = nullptr;

		if (SaveButton.bIsPointInBounds(MousePos))
		{
			SaveButton.click();
		}
		else if (ExitButton.bIsPointInBounds(MousePos))
		{
			ExitButton.click();
		}
	}
}

void lbe::SaveLevelPanel::EnterText(sf::Uint32 Unicode)
{
	if (CurrentlySelectedTextBox)
	{
		CurrentlySelectedTextBox->EnterText(Unicode);
	}
}
