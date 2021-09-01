#pragma once
#include "GameCore/Objects/GameObject.h"

enum class TileType : char;
enum class NeighbourTileDirection : char;
class Tile;

class Grid : public GameObject
{
public:	
	Grid();
	~Grid();

	virtual void Draw(sf::RenderWindow* pWindow) override;

private:
	sf::Vector2u m_gridSize;
	std::vector<Tile*> m_pTiles;

	class Tile* m_pTileSelector = nullptr;
	class Tile* m_pStartTile = nullptr;
	class Tile* m_pEndTile = nullptr;

public:
	void GenerateGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow, sf::Vector2f TopWidgetSize);
	void ResizeGrid(unsigned int numberOfColumns, sf::RenderWindow* pWindow, sf::Vector2f TopWidgetSize);

	void UpdateTileProperty(sf::Vector2u mouseTileCoord, sf::Vector2u gridSize, TileType tileType);
	void UpdateTileSelector(sf::Vector2u mouseTileCoord, sf::RenderWindow* pWindow, sf::Vector2f TopWidgetSize);

	void ClearGrid();
	void ClearAlgorithmSearch();

private:
	unsigned int GetTilesArrayIndex(sf::Vector2u TileCoord, sf::Vector2u GridSize) { return TileCoord.y + (TileCoord.x * GridSize.y); }

public:
	sf::Vector2u GetGridSize() { return m_gridSize; }
	unsigned int GetTotalTiles() { return m_gridSize.x * m_gridSize.y; }

	Tile* GetTile(sf::Vector2u tileCoord) { return m_pTiles[GetTilesArrayIndex(tileCoord, m_gridSize)]; }
	Tile* GetNeighbourTile(sf::Vector2u CurrentTileCoord, NeighbourTileDirection tileDirection);
	Tile* GetStartTile() { return m_pStartTile; }
	Tile* GetEndTile() { return m_pEndTile; }

	bool IsTileCoordValid(sf::Vector2u tileCoord);
	TileType GetTileState(sf::Vector2u tileCoord);


};
