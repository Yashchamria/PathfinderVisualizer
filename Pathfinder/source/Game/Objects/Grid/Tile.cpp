#include "FrameworkPCH.h"
#include "Tile.h"
#include "TileEnum.h"

Tile::Tile(sf::Vector2u TileCoord, sf::Vector2f TileSize)
{
	m_tileType = TileType::Default;
	m_tileAnimationState = TileAnimationState::Idle;

	SetTileSize(TileSize, 20.0f);
	SetTileColor(sf::Color(225, 225,225), sf::Color(25, 25, 25));

	m_TileBody.setScale(m_tileFillScale, m_tileFillScale);
	SetAnimTileScale(m_tileFillScale);
}

Tile::~Tile()
{
}


void Tile::Draw(sf::RenderWindow* pWindow)
{
	GameObject::Draw(pWindow);
	pWindow->draw(m_TileBody);
	pWindow->draw(m_TileAnimationBody);
}

void Tile::Update(float deltaTime)
{
	if (GetAnimTileScale() < m_tileFillScale)
	{
		SetAnimTileScale(GetAnimTileScale() + 0.08f);
	}
	else
	{
		m_TileBody.setFillColor(m_TileAnimationBody.getFillColor());
	}
}

void Tile::SetTileSize(sf::Vector2f tileSize, float OutlineThicknessFactor)
{
	m_TileBody.setSize(tileSize);
	m_TileBody.setOrigin(tileSize.x / 2.0f, tileSize.y / 2.0f);
	m_TileBody.setOutlineThickness(tileSize.x / OutlineThicknessFactor);

	m_TileAnimationBody.setSize(tileSize * m_tileFillScale); // To set the size equal to inner fill of main body.
	m_TileAnimationBody.setOrigin((tileSize * m_tileFillScale) / 2.0f);
}

void Tile::SetTileCoord(sf::Vector2u tileCoord, sf::RenderWindow* pWindow)
{
	m_tileCoord = tileCoord;
}

void Tile::SetTileColor(sf::Color fillColor, sf::Color OutlineColor)
{
	m_TileBody.setFillColor(fillColor);
	m_TileBody.setOutlineColor(OutlineColor);

	m_TileAnimationBody.setFillColor(fillColor);
}

void Tile::SetTileType(TileType tileType)
{
	if (m_tileType != tileType) { m_IsAnimationChanged = true; }

	m_tileType = tileType;
}

void Tile::SetTileAnimationProperty(TileAnimationState tileAnimationState)
{
	if (m_tileAnimationState != tileAnimationState) { m_IsAnimationChanged = true; }

	m_tileAnimationState = tileAnimationState;
}

void Tile::SetAnimTileScale(float tileScale)
{
	m_TileAnimationBody.setScale(tileScale, tileScale);
}

float Tile::GetAnimTileScale()
{
	return m_TileAnimationBody.getScale().x;
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
	m_TileAnimationBody.setPosition(tilePos);
}

void Tile::UpdateTileType()
{
	switch (m_tileType)
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

	default:
		break;
	}
}

void Tile::Animate(sf::Color color)
{
	if (m_IsAnimationChanged)
	{
		m_TileAnimationBody.setFillColor(color);
		SetAnimTileScale(0.0f);
		m_IsAnimationChanged = false;
	}
}

void Tile::UpdateTileAnimationProperty()
{
	switch (m_tileAnimationState)
	{
	case TileAnimationState::Idle:
		Animate(sf::Color(225, 225, 225));
		break;

	case TileAnimationState::Processing:
		Animate(sf::Color(5, 199, 242));
		break;

	case TileAnimationState::Processed:
		Animate(sf::Color(189, 54, 191));
		break;

	case TileAnimationState::Found:
		Animate(sf::Color(242, 200, 5));
		break;
	}
}
