#pragma once
#include "GameCore/Objects/GameObject.h"

enum class TileType : char;

class Grid : public GameObject
{

public:	
	Grid();
	~Grid();

	virtual void Draw(sf::RenderWindow* pWindow) override;

private:
	std::vector<class Tile*> m_pTiles;
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
};
