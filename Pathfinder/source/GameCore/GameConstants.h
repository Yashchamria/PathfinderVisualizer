#pragma once

namespace GameConst
{
	const static unsigned int WINDOW_WIDTH  = 1200;
	const static unsigned int WINDOW_HEIGHT = (unsigned int)(WINDOW_WIDTH / 2.0f);

	const static float WINDOW_ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

	const static float FIXED_FRAMERATE = 60.0f;

	const static float TIME_PER_FRAME = 1.0f / FIXED_FRAMERATE;

	const static float TOP_WIDGET_WIDTH = WINDOW_WIDTH;
	const static float TOP_WIDGET_HEIGHT = (float)WINDOW_HEIGHT / 4.0f;

	const static float GRID_ASPECT_RATIO = (float)WINDOW_WIDTH / ((float)WINDOW_HEIGHT - TOP_WIDGET_HEIGHT);

	const static unsigned int GRID_COLUMNS = 32;
	const static unsigned int GRID_ROWS = (unsigned int)(GRID_COLUMNS / GRID_ASPECT_RATIO);

};