#pragma once
#include <SFML/Graphics.hpp>
#include "Log.h"

namespace lbe
{
	const sf::Color SKIN_TONE_PALE(255, 224, 189, 255);
	const sf::Color SKIN_TONE_LIGHT(255, 205, 148, 255);
	const sf::Color SKIN_TONE_TAN(234, 192, 134, 255);
	const sf::Color SKIN_TONE_ORANGE(255, 173, 96, 255);
	const sf::Color SKIN_TONE_YELLOW(255, 227, 159, 255);
	const sf::Color COLOR_DARK_RED(150, 0, 0, 255);
	const sf::Color COLOR_DEEP_RED(174, 0, 0, 255);
	const sf::Color COLOR_MEDIUM_RED(199, 0, 0, 255);
	const sf::Color COLOR_LIGHT_RED(225, 0, 0, 255);
	const sf::Color COLOR_BRIGHT_RED(255, 0, 0, 255);
	const sf::Color PANEL_COLOR(31, 31, 31, 191);
	const sf::Color COLLISION_TILE_COLOR(255, 0, 0, 127);
	const sf::Color ANCHOR_TILE_COLOR(0, 255, 0, 127);
	const sf::Color SIZE_TILE_COLOR(0, 0, 255, 127);

	const int TILE_SIZE = 96;
	const int TILE_TEXTURE_SIZE = 32;
	const int TEXTURE_GRID_SPACING = 10;
	const sf::Vector2i TEXTURE_MAP_SIZE(256, 128);
	const int TILES_PER_ROW = TEXTURE_MAP_SIZE.x / TILE_TEXTURE_SIZE;
	const bool bScaleTileSizesInSidePanel = true;

	const double PLAYER_INNER_RADIUS = 35.f;
	const double PLAYER_OUTLINE_THICKNESS = 10.f;
	const double PLAYER_RADIUS = PLAYER_INNER_RADIUS + PLAYER_OUTLINE_THICKNESS;
	const double PLAYER_INNER_HAND_RADIUS = 7.f;
	const double PLAYER_OUTLINE_HAND_THICKNESS = 4.f;
	const double PLAYER_HAND_RADIUS = PLAYER_INNER_HAND_RADIUS + PLAYER_OUTLINE_HAND_THICKNESS;
	const float PLAYER_ARM_DISTANCE = 65.f;
	const double PLAYER_SLASH_REACH = 170.f;
	const bool USING_PLAYER_SPRITE = true;

	const sf::Vector2f SCREEN_SIZE(1280, 720);

	const spdlog::level::level_enum LOGGER_LEVEL = spdlog::level::trace;
	const bool DEBUG_LEVEL_EDITOR = true;
	const bool DEBUG_LEVEL_LOADER = false;
	const bool DEBUG_PLAYER_HANDS = false;
	const bool DEBUG_ENEMIES = false;

	//const std::string PATH_TEXTURE_MAP("./assets/textures/simple_tile_atlas_512x256.png");
	//const std::string PATH_TEXTURE_MAP("./assets/textures/Sprite_Stone_Floors_001.png");
	const std::string PATH_TEXTURE_MAP("./assets/textures/Sprite_Stone_Floors_002.png");
	const std::string PATH_PLAYER_WEAPON_BASIC("./assets/textures/BasicSword-3.png");
	const std::string PATH_PLAYER_WEAPON("./assets/textures/Sword_Model_1.0_2H.png");
	const std::string PATH_TEST_ENEMY_SPRITE("./assets/textures/Sprite_Enemy_Slimeball.png");
	const std::string PATH_BUTTON_SPRITES("./assets/textures/Sprite_Buttons.png");
	const std::string PATH_PLAYER_BODY_SPRITE("./assets/textures/RippedBody.png");
	const std::string PATH_PLAYER_HAND_SPRITE("./assets/textures/RippedHand.png");

	const double PI = 3.14159265359f;
	const double HalfC = PI / 180;

	//const std::string HOST_IP("localhost");
	const std::string HOST_IP("lordbinary.ddns.net");
	const unsigned short HOST_PORT = 48636;
	// Only set to a different port than the HOST_PORT for testing locally 
	const unsigned short CLIENT_PORT = HOST_PORT; 
	const sf::String PACKET_TYPE_TICK_UPDATE("TU");
	const sf::String PACKET_TYPE_REMOVE_OBJECT("RO");
	const sf::String PACKET_TYPE_SPAWN_OBJECT("SO");
}