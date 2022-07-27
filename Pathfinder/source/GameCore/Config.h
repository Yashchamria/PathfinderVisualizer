#pragma once
#include <cstdint>

struct Config
{
public:
	constexpr static float windowWidth = 1200.0f;
	constexpr static float windowHeight = windowWidth / 2.0f;

	constexpr static float displayWidth = windowWidth;
	constexpr static float displayHeight = windowHeight / 4.0f;

	constexpr static uint64_t gridColumns = 32;
	constexpr static uint64_t gridRows = gridColumns / (windowWidth / (windowHeight - displayHeight));
};