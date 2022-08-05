#pragma once

enum class Direction : char
{
	Up,
	Down,
	Right,
	Left
};

static sf::Vector2u GetNeighborCoord(sf::Vector2u coord, const Direction direction)
{
	switch (direction)
	{
		case Direction::Up:    --coord.y; break;
		case Direction::Down:  ++coord.y; break;
		case Direction::Left:  --coord.x; break;
		case Direction::Right: ++coord.x; break;
	}
	return coord;
}

static std::vector<sf::Vector2u> GetNeighborCoords(const sf::Vector2u coord)
{
	return std::vector<sf::Vector2u> { {coord.x + 1, coord.y}, {coord.x - 1, coord.y}, {coord.x, coord.y + 1}, {coord.x, coord.y - 1} };
}