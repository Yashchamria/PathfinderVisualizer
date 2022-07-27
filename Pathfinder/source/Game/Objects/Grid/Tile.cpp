#include "FrameworkPCH.h"
#include "Tile.h"
#include "TileEnum.h"

Tile::Tile(sf::Vector2u coord, sf::Vector2f size, int weight, TileType type) :
	m_coord(coord), m_weight(weight), m_type(type), m_animState(TileAnimState::Idle)
{
	SetTileSize(size, 20.0f);
	SetTileColor(sf::Color(225, 225,225), sf::Color(25, 25, 25));

	m_body.setScale(m_fillScale, m_fillScale);
	m_animBody.setScale(m_fillScale, m_fillScale);
}

void Tile::Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow)
{
	renderWindow->draw(m_body);
	renderWindow->draw(m_animBody);
}

void Tile::Update(float deltaTime)
{
	if (m_animBody.getScale().x < m_fillScale)
	{
		m_animBody.setScale(m_animBody.getScale().x + 0.08f, m_animBody.getScale().x + 0.08f);
	}
	else
	{
		m_body.setFillColor(m_animBody.getFillColor());
	}
}

void Tile::SetTileSize(sf::Vector2f tileSize, float OutlineThicknessFactor)
{
	m_body.setSize(tileSize);
	m_body.setOrigin(tileSize.x / 2.0f, tileSize.y / 2.0f);
	m_body.setOutlineThickness(tileSize.x / OutlineThicknessFactor);

	m_animBody.setSize(tileSize * m_fillScale); // To set the size equal to inner fill of main body.
	m_animBody.setOrigin((tileSize * m_fillScale) / 2.0f);
}

void Tile::SetTileCoord(sf::Vector2u coord)
{
	m_coord = coord;
}

void Tile::SetTileColor(sf::Color fillColor, sf::Color OutlineColor)
{
	m_body.setFillColor(fillColor);
	m_body.setOutlineColor(OutlineColor);

	m_animBody.setFillColor(fillColor);
}

void Tile::SetTileType(TileType tileType)
{
	if (m_type != tileType) { m_bAnimate = true; }
	m_type = tileType;
}

void Tile::SetTileAnimationProperty(TileAnimState state)
{
	if (m_animState != state) { m_bAnimate = true; }
	m_animState = state;
}

void Tile::RepositionTile()
{
	//Setting the tile position according to the tile coord and Screen size.
	const sf::Vector2f size = m_body.getSize();

	//TilePos = TileCoord * TileSize + OffsetTileCentre - OffsetToWindowsTopMostCorner
	const float posX = (m_coord.x * size.x) + (size.x / 2.0f) - Config::windowWidth / 2;
	const float posY = (m_coord.y * size.y) + (size.y / 2.0f) - Config::windowHeight / 2 + Config::displayHeight;

	m_body.setPosition({posX, posY});
	m_animBody.setPosition({posX, posY});
}

void Tile::UpdateTileType()
{
	switch (m_type)
	{
	case TileType::StartTile:
		Animate(sf::Color(57, 191, 66));
		break;

	case TileType::EndTile:
		Animate(sf::Color(200, 60, 60));
		break;

	case TileType::WallTile:
		Animate(sf::Color::Black);
		break;

	case TileType::Default:
		Animate(sf::Color(225, 225, 225));
		break;
	}
}

void Tile::Animate(sf::Color color)
{
	if (m_bAnimate)
	{
		m_animBody.setFillColor(color);
		m_animBody.setScale(0.0f, 0.0f);
		m_bAnimate = false;
	}
}

void Tile::UpdateTileAnimationProperty()
{
	switch (m_animState)
	{
	case TileAnimState::Idle:
		Animate(sf::Color(225, 225, 225));
		break;

	case TileAnimState::Processing:
		Animate(sf::Color(5, 199, 242));
		break;

	case TileAnimState::Processed:
		Animate(sf::Color(189, 54, 191));
		break;

	case TileAnimState::Found:
		Animate(sf::Color(242, 200, 5));
		break;
	}
}