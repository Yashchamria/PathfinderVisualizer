#include "FrameworkPCH.h"
#include "Grid.h"

#include "Tile.h"

Grid::Grid()
{
	m_pTileSelector = new Tile();
	m_pTileSelector->SetTileSelectorBody(sf::Color(50, 182, 148));
}

Grid::~Grid()
{
	for (Tile* pTile : m_pTiles)
	{
		delete pTile;
	}

	delete m_pTileSelector;
}

void Grid::Draw(sf::RenderWindow* pWindow)
{
	GameObject::Draw(pWindow);

	for (Tile* pTile : m_pTiles)
	{
		pTile->Draw(pWindow);
	}

	m_pTileSelector->Draw(pWindow);
}

unsigned int Grid::GetTilesArrayIndex(sf::Vector2u TileCoord, sf::Vector2u GridSize)
{
	unsigned int TileIndex = TileCoord.y + (TileCoord.x * GridSize.y);

	return TileIndex;
}

void Grid::GenerateGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow)
{
	m_pTiles.clear();
	m_pTiles.reserve(gridSize.x * gridSize.y);

	for (unsigned int x = 0; x < gridSize.x; x++)
	{
		for (unsigned int y = 0; y < gridSize.y; y++)
		{
			Tile* pTile = new Tile();
			pTile->SetTileCoord(sf::Vector2u(x, y), pWindow);
			pTile->RepositionTile(pWindow);
			//pTile->SetTileColor(sf::Color(sf::Color(rand() % 255, rand() % 255, rand() % 255, 255)));

			m_pTiles.push_back(pTile);
		}
	}
}

void Grid::ResizeGrid(unsigned int numberOfColumns, sf::RenderWindow* pWindow)
{
	float tileSize = pWindow->getSize().x / (float)numberOfColumns;

	for (Tile* pTile : m_pTiles)
	{
		pTile->SetTileSize(sf::Vector2f(tileSize, tileSize), 20.0f);
		pTile->RepositionTile(pWindow);
	}

	m_pTileSelector->SetTileSize(sf::Vector2f(tileSize, tileSize), 8.0f);
	m_pTileSelector->RepositionTile(pWindow);
}

void Grid::UpdateTileProperty(sf::Vector2u mouseTileCoord, sf::Vector2u gridSize, TileType tileType)
{
	//Storing & clearing the previous start tile.
	if (tileType == TileType::StartTile)
	{
		if (m_pStartTile)
		{
			m_pStartTile->UpdateTileProperty(TileType::Default);
		}

		m_pStartTile = m_pTiles[GetTilesArrayIndex(mouseTileCoord, gridSize)];
	}

	//Storing & clearing the previous end tile.
	if (tileType == TileType::EndTile)
	{
		if (m_pEndTile)
		{
			m_pEndTile->UpdateTileProperty(TileType::Default);
		}

		m_pEndTile = m_pTiles[GetTilesArrayIndex(mouseTileCoord, gridSize)];
	}

	m_pTiles[GetTilesArrayIndex(mouseTileCoord, gridSize)]->UpdateTileProperty(tileType);
}

void Grid::UpdateTileSelector(sf::Vector2u mouseTileCoord, sf::RenderWindow* pWindow)
{
	m_pTileSelector->SetTileCoord(mouseTileCoord, pWindow);
	m_pTileSelector->RepositionTile(pWindow);
}
