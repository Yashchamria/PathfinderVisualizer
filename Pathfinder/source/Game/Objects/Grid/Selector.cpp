#include "FrameworkPCH.h"
#include "Selector.h"
#include "Direction.h"

Selector::Selector(const sf::Vector2u coord, const float size, const sf::Color fill, const float thickness, const sf::Color outline) :
	m_coord(coord)
{
	SetSizeAndPosition(size);
	m_body.setFillColor(fill);
	m_body.setOutlineThickness(thickness);
	m_body.setOutlineColor(outline);
}

void Selector::Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow)
{
	renderWindow->draw(m_body);
}

void Selector::SetSizeAndPosition(float size)
{
	m_body.setSize({size, size});

	const float posX = (m_coord.x * size) - Config::windowWidth / 2.0f;
	const float posY = (m_coord.y * size) - (Config::windowHeight / 2.0f) + Config::displayHeight;

	m_body.setPosition({ posX, posY });
}

void Selector::SetCoordAndPosition(const sf::Vector2f worldPosition)
{
	const float size = m_body.getSize().x;
	const unsigned int coordX = floor(worldPosition.x / size);
	const unsigned int coordY = floor((worldPosition.y - Config::displayHeight) / size);

	SetCoordAndPosition(sf::Vector2u{coordX, coordY});
}

void Selector::SetCoordAndPosition(const Direction direction)
{
	SetCoordAndPosition(GetNeighborCoord(m_coord, direction));
}

void Selector::SetCoordAndPosition(const sf::Vector2u coord)
{
	const float size = m_body.getSize().x;
	const unsigned int zoomX = (unsigned int)(Config::windowWidth / size);
	const unsigned int zoomY = (unsigned int)((Config::windowHeight - Config::displayHeight) / size);

	if (coord.x > zoomX - 1 || coord.y > zoomY - 1) { return; }
	m_coord = coord;

	const float posX = (m_coord.x * size) - Config::windowWidth / 2.0f;
	const float posY = (m_coord.y * size) - (Config::windowHeight / 2.0f) + Config::displayHeight;
	m_body.setPosition({ posX, posY });
}
