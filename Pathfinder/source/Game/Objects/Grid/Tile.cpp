#include "FrameworkPCH.h"
#include "Tile.h"
#include "TileEnum.h"

Tile::Tile(sf::Vector2u TileCoord, sf::Vector2f TileSize)
{
	m_type = TileType::Default;
	m_tileAnimationState = TileAnimState::Idle;

	SetTileSize(TileSize, 20.0f);
	SetTileColor(sf::Color(225, 225,225), sf::Color(25, 25, 25));

	m_body.setScale(m_tileFillScale, m_tileFillScale);
	SetAnimTileScale(m_tileFillScale);
}

void Tile::Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow)
{
	renderWindow->draw(m_body);
	renderWindow->draw(m_animBody);
}

void Tile::Update(float deltaTime)
{
	if (GetAnimTileScale() < m_tileFillScale)
	{
		SetAnimTileScale(GetAnimTileScale() + 0.08f);
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

	m_animBody.setSize(tileSize * m_tileFillScale); // To set the size equal to inner fill of main body.
	m_animBody.setOrigin((tileSize * m_tileFillScale) / 2.0f);
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
	if (m_type != tileType) { m_IsAnimationChanged = true; }

	m_type = tileType;
}

void Tile::SetTileAnimationProperty(TileAnimState tileAnimationState)
{
	if (m_tileAnimationState != tileAnimationState) { m_IsAnimationChanged = true; }

	m_tileAnimationState = tileAnimationState;
}

void Tile::SetAnimTileScale(float tileScale)
{
	m_animBody.setScale(tileScale, tileScale);
}

float Tile::GetAnimTileScale()
{
	return m_animBody.getScale().x;
}

void Tile::RepositionTile()
{
	//Setting the tile position according to the tile coord and Screen size.
	sf::Vector2f tilePos;
	sf::Vector2f tileSize = m_body.getSize();

	//TilePos = TileCoord * TileSize + OffsetTileCentre - OffsetToWindowsTopMostCorner
	tilePos.x = (m_coord.x * tileSize.x) + (tileSize.x / 2.0f) - Config::windowWidth / 2;
	tilePos.y = (m_coord.y * tileSize.y) + (tileSize.y / 2.0f) - Config::windowHeight / 2 + Config::displayHeight;

	m_body.setPosition(tilePos);
	m_animBody.setPosition(tilePos);
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

	default:
		break;
	}
}

void Tile::Animate(sf::Color color)
{
	if (m_IsAnimationChanged)
	{
		m_animBody.setFillColor(color);
		SetAnimTileScale(0.0f);
		m_IsAnimationChanged = false;
	}
}

void Tile::UpdateTileAnimationProperty()
{
	switch (m_tileAnimationState)
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
