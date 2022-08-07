#include "FrameworkPCH.h"
#include "Tile.h"

Tile::Tile(const sf::Vector2u coord, const float size, TileType type, int weight, const sf::Font& font) :
	Coord(coord), Type(type), Weight(weight)
{
	m_text.setFont(font);
	m_text.setFillColor(WALL_TILE_COLOR);
	m_text.setString(std::to_string(weight));

	m_body.setFillColor(DEFAULT_TILE_COLOR);

	SetSizeAndPosition(size);
}

void Tile::Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow)
{
	renderWindow->draw(m_body);
	renderWindow->draw(m_text);
}

void Tile::Update(float deltaTime)
{
	if(m_bAnimate)
	{
		m_body.setOutlineThickness(m_body.getOutlineThickness() + (m_body.getSize().x * (float)m_animSpeed * 3.0f * deltaTime));

		if(m_body.getOutlineThickness() > 0.0f)
		{
			m_body.setOutlineThickness(0.0f);
			m_bAnimate = false;
		}
	}
}

void Tile::SetSizeAndPosition(float size)
{
	m_body.setSize({size, size});
	m_text.setCharacterSize((uint32_t)(size / 2.0f));

	size *= 1.0f / (1.0f - Config::gridOutlineStrength);
	const float posX = (Coord.x * size) - Config::windowWidth / 2.0f;
	const float posY = (Coord.y * size) - (Config::windowHeight / 2.0f) + Config::displayHeight;
	m_body.setPosition({posX, posY});
	m_text.setPosition({ posX, posY });
}

void Tile::Animate(const sf::Color color, const VisualSpeed animSpeed)
{
	m_bAnimate = true;
	m_animSpeed = animSpeed;
	m_body.setOutlineThickness(- m_body.getSize().x / 2.0f);
	m_body.setOutlineColor(m_body.getFillColor());
	m_body.setFillColor(color);
}

void Tile::SetWeightAndLabel(const int weight)
{
	Weight = weight;
	m_text.setString(std::to_string(Weight));
}