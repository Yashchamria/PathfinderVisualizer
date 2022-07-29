#pragma once
#include <cstdint>

struct Config
{
public:
	constexpr static float windowWidth = 1200.0f;
	constexpr static float windowHeight = windowWidth / 2.0f;

	constexpr static float displayWidth = windowWidth;
	constexpr static float displayHeight = windowHeight / 4.0f;

	constexpr static uint64_t gridColumns = 64;
	constexpr static uint64_t gridRows = gridColumns / (windowWidth / (windowHeight - displayHeight));

	constexpr static float gridOutlineStrength = 0.1f;

	constexpr static float mouseSensitivity = 5.0f;

	#define START_TILE_COLOR      sf::Color(57, 191, 66, 255)
	#define END_TILE_COLOR        sf::Color(200, 60, 60, 255)
	#define WALL_TILE_COLOR       sf::Color(50, 50, 50, 255)
	#define DEFAULT_TILE_COLOR    sf::Color(225, 225, 225, 255)
	#define PROCESSING_TILE_COLOR sf::Color(5, 199, 242, 255)
	#define PROCESSED_TILE_COLOR  sf::Color(189, 54, 191, 255)
	#define FOUND_TILE_COLOR      sf::Color(242, 200, 5, 255)

	#define GRID_COLOR            sf::Color(10, 10, 10, 255)
	#define DISPLAY_COLOR         sf::Color(242, 166, 73, 255)

	#define SELECTOR_COLOR        sf::Color(250, 109, 5, 255)
};