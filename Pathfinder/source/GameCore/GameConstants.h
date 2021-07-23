#pragma once

namespace GameConst
{
	const static char* GAME_WINDOW_NAME = "Pathfinder Visualizer";

	const static int WINDOW_WIDTH = 852;
	const static int WINDOW_HEIGHT = 480;

	const static float WINDOW_ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

	const static float FIXED_FRAMERATE = 60.0f;

	const static float TIME_PER_FRAME = 1.0f / FIXED_FRAMERATE;
};