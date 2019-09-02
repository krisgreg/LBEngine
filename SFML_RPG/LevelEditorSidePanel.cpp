#include "LevelEditorSidePanel.h"

#include "Consts.h"
#include "SceneObjectFactory.h"
#include "SceneObjectPlaceableInEditor.h"

lbe::LevelEditorSidePanel::LevelEditorSidePanel()
{
}

int lbe::LevelEditorSidePanel::SelectedTextureId = 0;
lbe::Layer lbe::LevelEditorSidePanel::LayerSelected = lbe::Layer::Background;
std::string lbe::LevelEditorSidePanel::ObjectSelected = "";
int lbe::LevelEditorSidePanel::SelectedMetaAction = 0;

lbe::LevelEditorSidePanel::LevelEditorSidePanel(sf::Vector2f WindowSize, sf::Font &Font, sf::Texture *TextureMap) : Font(Font)
{
	Background.setPosition(0, 0);
	Background.setSize(sf::Vector2f(WindowSize.x / 4, WindowSize.y));
	Background.setFillColor(lbe::PANEL_COLOR);

	sf::Vector2f DefaultGridPos(lbe::TEXTURE_GRID_SPACING, lbe::TEXTURE_GRID_SPACING * 3 + 80);
	sf::Vector2f CurrentGridPos(DefaultGridPos);

	int TileSizeToUse = lbe::TILE_TEXTURE_SIZE;
	if (bScaleTileSizesInSidePanel)
	{
		TileSizeToUse = floor((Background.getSize().x - (lbe::TILES_PER_ROW + 1) * lbe::TEXTURE_GRID_SPACING) / lbe::TILES_PER_ROW);
		//LBE_TRACE("TileSizeToUse={0:d}", TileSizeToUse);
	}


	for (int i = 0; i < (TextureMap->getSize().x / lbe::TILE_TEXTURE_SIZE) * (TextureMap->getSize().y / lbe::TILE_TEXTURE_SIZE); i++)
	{
		if (CurrentGridPos.x + TileSizeToUse + lbe::TEXTURE_GRID_SPACING > Background.getSize().x)
		{
			CurrentGridPos.x = lbe::TEXTURE_GRID_SPACING;
			CurrentGridPos.y += TileSizeToUse + lbe::TEXTURE_GRID_SPACING;
		}
		
		sf::Vector2f NewTileTexturePos((lbe::TILE_TEXTURE_SIZE * i) % lbe::TEXTURE_MAP_SIZE.x, floor((lbe::TILE_TEXTURE_SIZE * i) / lbe::TEXTURE_MAP_SIZE.x) * lbe::TILE_TEXTURE_SIZE);

		auto TextureButtonOnClick = [this, i]
		{
			//std::cout << "Button clicked: " << i + 1 << std::endl;
			SetSelectedTextureId(i + 1);
			return false;
		};
		
		//std::cout << "x=" << CurrentGridPos.x << " y=" << CurrentGridPos.y << std::endl;

		lbe::Button TextureButton = lbe::Button("", Font, TextureButtonOnClick);
		TextureButton.SetPosition(CurrentGridPos);
		TextureButton.SetSize(sf::Vector2f(TileSizeToUse, TileSizeToUse));
		TextureButton.SetTexture(TextureMap, NewTileTexturePos);
		TextureButton.SetbDisplayOutlineOnClick(true);
		TextureGridButtons.push_back(TextureButton);

		CurrentGridPos.x += TileSizeToUse + lbe::TEXTURE_GRID_SPACING;
	}

	CurrentGridPos = DefaultGridPos;

	SceneObjectCreatorMapType* ObjectCreatorMap = SceneObjectFactory::GetMap();
	for (auto& MapEntry : *ObjectCreatorMap)
	{
		if (SceneObjectPlaceableInEditor* CurrentObject = dynamic_cast<SceneObjectPlaceableInEditor*>(MapEntry.second(sf::Vector2f(0,0))))
		{
			if (CurrentObject->IsPlaceableInEditor())
			{
				if (CurrentGridPos.x + TileSizeToUse + lbe::TEXTURE_GRID_SPACING > Background.getSize().x)
				{
					CurrentGridPos.x = lbe::TEXTURE_GRID_SPACING;
					CurrentGridPos.y += TileSizeToUse + lbe::TEXTURE_GRID_SPACING;
				}

				std::string ObjectName = MapEntry.first;
				auto ObjectButtonOnClick = [ObjectName]
				{
					lbe::LevelEditorSidePanel::SetSelectedObject(ObjectName);
					return false;
				};

				lbe::Button ObjectButton = lbe::Button("", Font, ObjectButtonOnClick);
				ObjectButton.SetPosition(CurrentGridPos);
				ObjectButton.SetSize(sf::Vector2f(TileSizeToUse, TileSizeToUse));
				ObjectButton.SetTexture(CurrentObject->GetObjectIcon(), CurrentObject->GetObjectIconRect());
				ObjectButton.SetbDisplayOutlineOnClick(true);
				ObjectGridButtons.push_back(ObjectButton);

				CurrentGridPos.x += TileSizeToUse + lbe::TEXTURE_GRID_SPACING;
			}
		}
	}


	auto BackgroundLayerButtonOnClick = []
	{
		lbe::LevelEditorSidePanel::SetSelectedLayer(lbe::Layer::Background);
		return false;
	};
	BackgroundLayerButton = lbe::Button("Background", Font, BackgroundLayerButtonOnClick);
	BackgroundLayerButton.SetPosition(sf::Vector2f(lbe::TEXTURE_GRID_SPACING, lbe::TEXTURE_GRID_SPACING));
	BackgroundLayerButton.SetSize(sf::Vector2f(Background.getSize().x / 2 - lbe::TEXTURE_GRID_SPACING * 2, 40));
	if (LayerSelected == lbe::Layer::Background)
	{
		BackgroundLayerButton.SetTextureRect(sf::IntRect(0, 40, 140, 40));
	}

	auto CollisionLayerButtonOnClick = []
	{
		lbe::LevelEditorSidePanel::SetSelectedLayer(lbe::Layer::Collision);
		return false;
	};
	CollisionLayerButton = lbe::Button("Collision", Font, CollisionLayerButtonOnClick);
	CollisionLayerButton.SetPosition(sf::Vector2f(lbe::TEXTURE_GRID_SPACING + Background.getSize().x / 2, lbe::TEXTURE_GRID_SPACING));
	CollisionLayerButton.SetSize(sf::Vector2f(Background.getSize().x / 2 - lbe::TEXTURE_GRID_SPACING * 2, 40));
	if (LayerSelected == lbe::Layer::Collision)
	{
		CollisionLayerButton.SetTextureRect(sf::IntRect(0, 40, 140, 40));
	}

	auto ObjectEditorButtonOnClick = []
	{
		lbe::LevelEditorSidePanel::SetSelectedLayer(lbe::Layer::Object);
		return false;
	};
	ObjectEditorButton = lbe::Button("Objects", Font, ObjectEditorButtonOnClick);
	ObjectEditorButton.SetPosition(sf::Vector2f(lbe::TEXTURE_GRID_SPACING, lbe::TEXTURE_GRID_SPACING * 2 + 40));
	ObjectEditorButton.SetSize(sf::Vector2f(Background.getSize().x / 2 - lbe::TEXTURE_GRID_SPACING * 2, 40));
	if (LayerSelected == lbe::Layer::Object)
	{
		ObjectEditorButton.SetTextureRect(sf::IntRect(0, 40, 140, 40));
	}

	auto MetaDataButtonOnClick = []
	{
		lbe::LevelEditorSidePanel::SetSelectedLayer(lbe::Layer::Meta);
		return false;
	};
	MetaDataEditorButton = lbe::Button("Metadata", Font, MetaDataButtonOnClick);
	MetaDataEditorButton.SetPosition(sf::Vector2f(lbe::TEXTURE_GRID_SPACING + Background.getSize().x / 2, lbe::TEXTURE_GRID_SPACING * 2 + 40));
	MetaDataEditorButton.SetSize(sf::Vector2f(Background.getSize().x / 2 - lbe::TEXTURE_GRID_SPACING * 2, 40));
	if (LayerSelected == lbe::Layer::Meta)
	{
		MetaDataEditorButton.SetTextureRect(sf::IntRect(0, 40, 140, 40));
	}
}


lbe::LevelEditorSidePanel::~LevelEditorSidePanel()
{
}

void lbe::LevelEditorSidePanel::Draw(sf::RenderWindow &Window)
{
	Window.draw(Background);
	BackgroundLayerButton.Draw(Window);
	CollisionLayerButton.Draw(Window);
	ObjectEditorButton.Draw(Window);
	MetaDataEditorButton.Draw(Window);
	if (LayerSelected == lbe::Layer::Background)
	{
		for (auto &i : TextureGridButtons)
		{
			i.Draw(Window);
		}
	}
	else if (LayerSelected == lbe::Layer::Collision)
	{

	}
	else if (LayerSelected == lbe::Layer::Object)
	{
		for (auto &i : ObjectGridButtons)
		{
			i.Draw(Window);
		}
	}
	else if (LayerSelected == lbe::Layer::Meta)
	{

	}
}

void lbe::LevelEditorSidePanel::SetSelectedTextureId(int Id)
{
	//std::cout << "Texture Id to set:         " << Id << std::endl;
	//std::cout << "Texture Id before setting: " << SelectedTextureId << std::endl;
	SelectedTextureId = Id;
	//std::cout << "Texture Id after setting:  " << SelectedTextureId << std::endl;
}

void lbe::LevelEditorSidePanel::Click(sf::Vector2f ClickPos)
{
	//std::cout << "Side Panel Clicked" << std::endl;
	if (BackgroundLayerButton.bIsPointInBounds(ClickPos))
	{
		BackgroundLayerButton.click();
		BackgroundLayerButton.SetTextureRect(sf::IntRect(0, 40, 140, 40));
		CollisionLayerButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
		ObjectEditorButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
		MetaDataEditorButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
		
	}
	else if (CollisionLayerButton.bIsPointInBounds(ClickPos))
	{
		CollisionLayerButton.click();
		CollisionLayerButton.SetTextureRect(sf::IntRect(0, 40, 140, 40));
		BackgroundLayerButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
		ObjectEditorButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
		MetaDataEditorButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
	}
	else if (ObjectEditorButton.bIsPointInBounds(ClickPos))
	{
		ObjectEditorButton.click();
		ObjectEditorButton.SetTextureRect(sf::IntRect(0, 40, 140, 40));
		CollisionLayerButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
		BackgroundLayerButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
		MetaDataEditorButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
	}
	else if (MetaDataEditorButton.bIsPointInBounds(ClickPos))
	{
		MetaDataEditorButton.click();
		MetaDataEditorButton.SetTextureRect(sf::IntRect(0, 40, 140, 40));
		CollisionLayerButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
		BackgroundLayerButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
		ObjectEditorButton.SetTextureRect(sf::IntRect(0, 0, 140, 40));
	}
	else
	{
		if (lbe::LevelEditorSidePanel::LayerSelected == lbe::Layer::Background)
		{
			for (auto &TexButton : TextureGridButtons)
			{
				if (TexButton.bIsPointInBounds(ClickPos))
				{
					for (auto &Button : TextureGridButtons)
					{
						Button.deselect();
					}
					TexButton.click();
					//std::cout << "A Button Was clicked" << std::endl;
				}
			}
		}
		else if (lbe::LevelEditorSidePanel::LayerSelected == lbe::Layer::Object)
		{
			for (auto &ObjectButton : ObjectGridButtons)
			{
				if (ObjectButton.bIsPointInBounds(ClickPos))
				{
					for (auto &Button : ObjectGridButtons)
					{
						Button.deselect();
					}
					ObjectButton.click();
				}
			}
		}
	}
}

bool lbe::LevelEditorSidePanel::bIsPointInBounds(sf::Vector2f Point)
{
	return Background.getGlobalBounds().contains(Point);
}

int lbe::LevelEditorSidePanel::GetSelectedTextureId()
{
	return SelectedTextureId;
}

void lbe::LevelEditorSidePanel::SetSelectedLayer(lbe::Layer NewLayer)
{
	LayerSelected = NewLayer;
	//std::cout << "LayerSelected=" << LayerSelected << std::endl;
}

void lbe::LevelEditorSidePanel::SetSelectedObject(std::string SelectedObject)
{
	ObjectSelected = SelectedObject;
}

lbe::Layer lbe::LevelEditorSidePanel::GetSelectedLayer()
{
	return LayerSelected;
}

std::string lbe::LevelEditorSidePanel::GetSelectedObjectName()
{
	return ObjectSelected;
}

int lbe::LevelEditorSidePanel::GetSelectedMetaAction()
{
	return SelectedMetaAction;
}

void lbe::LevelEditorSidePanel::DebugSelectedTextureId()
{
	LBE_TRACE("SelectedTextureId={0:d}", SelectedTextureId);
}
