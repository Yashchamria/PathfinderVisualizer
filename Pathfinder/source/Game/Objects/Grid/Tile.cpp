#include "FrameworkPCH.h"
#include "Tile.h"
#include "TileEnum.h"

Tile::Tile(sf::Vector2u TileCoord, sf::Vector2f TileSize)
{
	m_tileType = TileType::Default;
	m_tileAnimationState = TileAnimationState::Idle;

	SetTileSize(TileSize, 20.0f);
	SetTileColor(sf::Color(225, 225,225), sf::Color(25, 25, 25));

	m_TileBody.setScale(0.92f, 0.92f);
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

void Tile::SetTileProperty(TileType tileType)
{
	m_tileType = tileType;
}

void Tile::SetTileAnimationProperty(TileAnimationState tileAnimationState)
{
	m_tileAnimationState = tileAnimationState;
}

void Tile::RepositionTile(sf::RenderWindow* pWindow, sf::Vector2f TopWidgetSize)
{
	//Setting the tile position according to the tile coord and Screen size.
	sf::Vector2f tilePos;
	sf::Vector2f tileSize = m_TileBody.getSize();
	sf::Vector2u windowSize = pWindow->getSize();

	//TilePos = TileCoord * TileSize + OffsetTileCentre - OffsetToWindowsTopMostCorner
	tilePos.x = (m_tileCoord.x * tileSize.x) + (tileSize.x / 2.0f) - ((float)windowSize.x / 2.0f);
	tilePos.y = (m_tileCoord.y * tileSize.y) + (tileSize.y / 2.0f) - ((float)windowSize.y / 2.0f) + TopWidgetSize.y;

	m_TileBody.setPosition(tilePos);
}

void Tile::UpdateTileProperty()
{

	switch (m_tileType)
	{
	case TileType::StartTile:
		SetTileColor(sf::Color(57, 191, 66));
		break;

	case TileType::EndTile:
		SetTileColor(sf::Color(200, 60, 60));
		break;

	case TileType::WallTile:
		SetTileColor(sf::Color::Black);
		break;

	case TileType::Default:
		SetTileColor(sf::Color(225, 225, 225));
		break;

	default:
		break;
	}
}

void Tile::UpdateTileAnimationProperty()
{
	switch (m_tileAnimationState)
	{
	case TileAnimationState::Idle:
		SetTileColor(sf::Color(225, 225, 225));
		break;

	case TileAnimationState::Processing:
		SetTileColor(sf::Color(5, 199, 242));
		break;

	case TileAnimationState::Processed:
		SetTileColor(sf::Color(189, 54, 191));
		break;

	case TileAnimationState::Found:
		SetTileColor(sf::Color(242, 200, 5));
		break;
	}
}
