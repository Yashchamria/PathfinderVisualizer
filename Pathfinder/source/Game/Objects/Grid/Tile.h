#pragma once
#include "GameCore/Objects/GameObject.h"

enum class TileAnimationState : char;
enum class TileType : char;

class Tile : public GameObject
{
public:	
	Tile(sf::Vector2u TileCoord = sf::Vector2u(0, 0), sf::Vector2f TileSize = sf::Vector2f(50.0f, 50.0f));
	~Tile();

	virtual void Draw(sf::RenderWindow* pWindow) override;

private:
	sf::RectangleShape m_TileBody;
	sf::Vector2u m_tileCoord;
	TileType m_tileType;
	TileAnimationState m_tileAnimationState;
	unsigned int m_Weight = 10;

public:
	void RepositionTile(sf::RenderWindow* pWindow, sf::Vector2f TopWidgetSize);
	void UpdateTileProperty();
	void UpdateTileAnimationProperty();


public:
	void SetTileCoord(sf::Vector2u tileCoord, sf::RenderWindow* pWindow);
	void SetTileSize(sf::Vector2f tileSize, float OutlineThicknessFactor);
	void SetTileColor(sf::Color fillColor = sf::Color::White, sf::Color OutlineColor = sf::Color::Black);
	void SetTileSelectorBody(sf::Color OutlineColor);
	void SetTileProperty(TileType tileType);
	void SetTileAnimationProperty(TileAnimationState tileAnimationState);

public:
	sf::Vector2u GetTileCoord() { return m_tileCoord; }
	unsigned int GetTileWeight() { return m_Weight; }
	TileType GetTileState() { return m_tileType; }
	TileAnimationState GetTileAnimationState() { return m_tileAnimationState; }
};
