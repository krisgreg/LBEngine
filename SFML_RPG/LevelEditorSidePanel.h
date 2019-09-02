#pragma once
#include "Button.h"
#include "Enums.h"

namespace lbe
{
	class LevelEditorSidePanel
	{
	private:
		sf::RectangleShape Background;
		sf::Vector2f TextureGridPos;
		double TextureGridSpacing;
		std::vector<lbe::Button> TextureGridButtons;
		std::vector<lbe::Button> ObjectGridButtons;
		static int SelectedTextureId;
		sf::Font Font;
		sf::Texture *TextureMap;
		lbe::Button BackgroundLayerButton;
		lbe::Button CollisionLayerButton;
		lbe::Button ObjectEditorButton;
		lbe::Button MetaDataEditorButton;
		static lbe::Layer LayerSelected;
		static std::string ObjectSelected;
		static int SelectedMetaAction;

	public:
		LevelEditorSidePanel();
		LevelEditorSidePanel(sf::Vector2f WindowSize, sf::Font &Font, sf::Texture *TextureMap);
		~LevelEditorSidePanel();

		void Draw(sf::RenderWindow &Window);
		void SetSelectedTextureId(int Id);
		void Click(sf::Vector2f ClickPos);
		bool bIsPointInBounds(sf::Vector2f Point);
		int GetSelectedTextureId();
		static void SetSelectedLayer(lbe::Layer NewLayer);
		static void SetSelectedObject(std::string SelectedObject);
		static lbe::Layer GetSelectedLayer();
		static std::string GetSelectedObjectName();
		static int GetSelectedMetaAction();

		void DebugSelectedTextureId();
	};
}