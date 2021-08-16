#pragma once
#include "GameCore/Objects/GameObject.h"

enum class TileType : char;
class Tile;

class Grid : public GameObject
{
public:	
	Grid();
	~Grid();

	virtual void Draw(sf::RenderWindow* pWindow) override;

private:
	std::vector<Tile*> m_pTiles;
	sf::Vector2u m_gridSize;
	class Tile* m_pTileSelector = nullptr;
	class Tile* m_pStartTile = nullptr;
	class Tile* m_pEndTile = nullptr;

private:
	unsigned int GetTilesArrayIndex(sf::Vector2u TileCoord, sf::Vector2u GridSize);

public:
	void GenerateGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow);
	void ResizeGrid(unsigned int numberOfColumns, sf::RenderWindow* pWindow);
	void ClearGrid();
	void UpdateTileProperty(sf::Vector2u mouseTileCoord, sf::Vector2u gridSize, TileType tileType);
	void UpdateTileSelector(sf::Vector2u mouseTileCoord, sf::RenderWindow* pWindow);

//Variables for pathfinder algorithms
public:
	Tile* GetStartTile() { return m_pStartTile; }
	Tile* GetEndTile() { return m_pEndTile; }
	Tile* GetTile(sf::Vector2u tileCoord) { return m_pTiles[GetTilesArrayIndex(tileCoord, m_gridSize)]; }

	sf::Vector2u GetGridSize() { return m_gridSize; }
	unsigned int GetTotalTiles() { return m_gridSize.x * m_gridSize.y; }

	bool IsTileCoordValid(sf::Vector2u tileCoord);
	TileType GetTileState(sf::Vector2u tileCoord);
};
