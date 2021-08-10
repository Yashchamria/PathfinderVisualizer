#pragma once
#include "GameCore/Objects/GameObject.h"

enum class TileType : char
{
	Default,
	StartTile,
	EndTile,
	WallTile,
};

class Tile : public GameObject
{
public:	
	Tile(sf::Vector2u TileCoord = sf::Vector2u(0, 0), sf::Vector2f TileSize = sf::Vector2f(50.0f, 50.0f));
	~Tile();

	virtual void Draw(sf::RenderWindow* pWindow) override;

private:
	sf::RectangleShape m_TileBody;
	sf::Vector2u m_tileCoord;
	TileType m_tileType = TileType::Default;

public:
	void RepositionTile(sf::RenderWindow* pWindow);
	void UpdateTileProperty(TileType tileType);

public:
	void SetTileCoord(sf::Vector2u tileCoord, sf::RenderWindow* pWindow);
	void SetTileSize(sf::Vector2f tileSize, float OutlineThicknessFactor);
	void SetTileColor(sf::Color fillColor = sf::Color::White, sf::Color OutlineColor = sf::Color::Black);
	void SetTileSelectorBody(sf::Color OutlineColor);
};
