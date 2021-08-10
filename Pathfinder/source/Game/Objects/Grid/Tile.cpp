#include "FrameworkPCH.h"
#include "Tile.h"

Tile::Tile(sf::Vector2u TileCoord, sf::Vector2f TileSize)
{
	SetTileSize(TileSize, 20.0f);
	SetTileColor(sf::Color::White, sf::Color::Black);
}

Tile::~Tile()
{
}

void Tile::Draw(sf::RenderWindow* pWindow)
{
	GameObject::Draw(pWindow);
	pWindow->draw(m_TileBody);
}

void Tile::SetTileSize(sf::Vector2f tileSize, float OutlineThicknessFactor)
{
	m_TileBody.setSize(tileSize);
	m_TileBody.setOrigin(tileSize.x / 2.0f, tileSize.y / 2.0f);
	m_TileBody.setOutlineThickness(tileSize.x / OutlineThicknessFactor);
}

void Tile::SetTileCoord(sf::Vector2u tileCoord, sf::RenderWindow* pWindow)
{
	m_tileCoord = tileCoord;
}

void Tile::SetTileColor(sf::Color fillColor, sf::Color OutlineColor)
{
	m_TileBody.setFillColor(fillColor);
	m_TileBody.setOutlineColor(OutlineColor);
}

void Tile::SetTileSelectorBody(sf::Color OutlineColor)
{
	m_TileBody.setFillColor(sf::Color::Transparent);
	m_TileBody.setOutlineColor(OutlineColor);
}

void Tile::RepositionTile(sf::RenderWindow* pWindow)
{
	//Setting the tile position according to the tile coord and Screen size.
	sf::Vector2f tilePos;
	sf::Vector2f tileSize = m_TileBody.getSize();
	sf::Vector2u windowSize = pWindow->getSize();

	//TilePos = TileCoord * TileSize + OffsetTileCentre - OffsetToWindowsTopMostCorner
	tilePos.x = (m_tileCoord.x * tileSize.x) + (tileSize.x / 2.0f) - ((float)windowSize.x / 2.0f);
	tilePos.y = (m_tileCoord.y * tileSize.y) + (tileSize.y / 2.0f) - ((float)windowSize.y / 2.0f);

	m_TileBody.setPosition(tilePos);
}

void Tile::UpdateTileProperty(TileType tileType)
{
	m_tileType = tileType;

	switch (tileType)
	{
	case TileType::StartTile:
		SetTileColor(sf::Color::Green);
		break;

	case TileType::EndTile:
		SetTileColor(sf::Color::Red);
		break;

	case TileType::WallTile:
		SetTileColor(sf::Color::Black);
		break;

	case TileType::Default:
		SetTileColor(sf::Color::White);
		break;

	default:
		break;
	}
}
