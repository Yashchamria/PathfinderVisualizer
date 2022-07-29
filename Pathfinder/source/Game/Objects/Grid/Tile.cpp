#include "FrameworkPCH.h"
#include "Tile.h"
#include "TileEnum.h"

Tile::Tile(const sf::Vector2u coord, const float size, int weight, TileType type) :
	m_coord(coord), m_weight(weight), Type(type)
{
	SetSizeAndPosition(size);
	m_body.setFillColor(DEFAULT_TILE_COLOR);
}

void Tile::Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow)
{
	renderWindow->draw(m_body);
}

void Tile::Update(float deltaTime)
{
	if(m_bAnimate)
	{
		m_body.setOutlineThickness(m_body.getOutlineThickness() + (m_body.getSize().x * 01.0f * deltaTime));

		if(m_body.getOutlineThickness() > 0.0f)
		{
			m_body.setOutlineThickness(0.0f);
			m_bAnimate = false;
		}
	}
}

void Tile::SetColor(const sf::Color fill, const float thickness, const sf::Color outline)
{
	m_body.setFillColor(fill);
	m_body.setOutlineThickness(thickness);
	m_body.setOutlineColor(outline);
}

void Tile::SetSizeAndPosition(float size)
{
	m_body.setSize({size, size});

	size *= 1.0f / (1.0f - Config::gridOutlineStrength);
	const float posX = (m_coord.x * size) - Config::windowWidth / 2.0f;
	const float posY = (m_coord.y * size) - (Config::windowHeight / 2.0f) + Config::displayHeight;
	m_body.setPosition({posX, posY});
}

void Tile::SetCoordAndPosition(const sf::Vector2u coord)
{
	m_coord = coord;
	SetSizeAndPosition(m_body.getSize().x);
}

void Tile::Animate(const sf::Color color)
{
	m_bAnimate = true;
	m_body.setOutlineThickness(- m_body.getSize().x / 2.0f);
	m_body.setOutlineColor(m_body.getFillColor());
	m_body.setFillColor(color);
}