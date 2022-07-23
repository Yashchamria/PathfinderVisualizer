#pragma once
#include "GameCore/Objects/GameObject.h"

enum class TileType : char;
enum class NeighbourTileDirection : char;
class Tile;

class Grid final: public GameObject
{
public:	
	Grid(const sf::Vector2u gridSize, const sf::Vector2u windowSize, const sf::Vector2f displaySize);
	~Grid();

	void Update(float deltaTime) override;
	void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;

private:
	const sf::Vector2u m_gridSize;
	sf::Vector2u m_ZoomedGridSize;

	std::vector<Tile*> m_pTiles;

	Tile* m_pTileSelector = nullptr;
	Tile* m_pStartTile = nullptr;
	Tile* m_pEndTile = nullptr;

public:
	void ResizeGrid(unsigned int numberOfColumns, sf::Vector2u windowSize, sf::Vector2f TopWidgetSize) const;

	void UpdateTileProperty(sf::Vector2u mouseTileCoord, TileType tileType);
	void UpdateTileSelector(sf::Vector2u mouseTileCoord, sf::Vector2u windowSize, sf::Vector2f TopWidgetSize);
	void SetSelectorPosition(sf::Vector2u mouseTileCoord, sf::Vector2u windowSize, sf::Vector2f displaySize);

	void ClearGrid();
	void ClearAlgorithmSearch();

	void SetZoomedGridSize(unsigned int ColumnSize) { m_ZoomedGridSize = sf::Vector2u(ColumnSize, (unsigned int)((float)ColumnSize / GameConst::GRID_ASPECT_RATIO)); }
	sf::Vector2u GetZoomedGridSize() const { return m_ZoomedGridSize; }

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

public:
	void GenerateRandomWalls(unsigned int wallPercent);
	void GenerateRandomTile(TileType tileType, unsigned int quadrant, bool cornerBais);
	void GenerateRandomGrid(unsigned int wallPercent, unsigned int StartQuadrant, unsigned int EndQuadrant);
};
