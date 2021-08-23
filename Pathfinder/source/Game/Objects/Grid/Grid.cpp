#include "FrameworkPCH.h"
#include "Grid.h"

#include "Tile.h"
#include "TileEnum.h"

Grid::Grid()
{
	m_pTileSelector = new Tile();
	m_pTileSelector->SetTileSelectorBody(sf::Color(50, 182, 148));
}

Grid::~Grid()
{
	ClearGrid();

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

void Grid::GenerateGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow, sf::Vector2f TopWidgetSize)
{
	m_gridSize = gridSize;

	m_pTiles.clear();
	m_pTiles.reserve(gridSize.x * gridSize.y);

	for (unsigned int x = 0; x < gridSize.x; x++)
	{
		for (unsigned int y = 0; y < gridSize.y; y++)
		{
			Tile* pTile = new Tile();
			pTile->SetTileCoord(sf::Vector2u(x, y), pWindow);
			pTile->RepositionTile(pWindow, TopWidgetSize);

			m_pTiles.push_back(pTile);
		}
	}
}

void Grid::ResizeGrid(unsigned int numberOfColumns, sf::RenderWindow* pWindow, sf::Vector2f TopWidgetSize)
{
	float tileSize = pWindow->getSize().x / (float)numberOfColumns;

	for (Tile* pTile : m_pTiles)
	{
		pTile->SetTileSize(sf::Vector2f(tileSize, tileSize), 20.0f);
		pTile->RepositionTile(pWindow, TopWidgetSize);
	}

	m_pTileSelector->SetTileSize(sf::Vector2f(tileSize, tileSize), 8.0f);
	m_pTileSelector->RepositionTile(pWindow, TopWidgetSize);
}

void Grid::ClearGrid()
{
	for (Tile* pTile : m_pTiles)
	{
		pTile->SetTileProperty(TileType::Default);
		pTile->UpdateTileProperty();
	}

	m_pStartTile = nullptr;
	m_pEndTile = nullptr;
}

void Grid::ClearAlgorithmSearch()
{
	for (Tile* pTile : m_pTiles)
	{
		pTile->UpdateTileProperty();
	}
}


void Grid::UpdateTileProperty(sf::Vector2u mouseTileCoord, sf::Vector2u gridSize, TileType tileType)
{
	if (!IsTileCoordValid(mouseTileCoord)) { return; }

	if (tileType == TileType::StartTile)
	{
		if (m_pEndTile)
		{
			if (m_pEndTile->GetTileCoord() == mouseTileCoord)
			{
				m_pEndTile = nullptr;
			}
		}

		if (m_pStartTile)
		{
			//Clearing the previous start tile.
			m_pStartTile->SetTileProperty(TileType::Default);
			m_pStartTile->UpdateTileProperty();
			m_pStartTile = nullptr;
		}
		m_pStartTile = m_pTiles[GetTilesArrayIndex(mouseTileCoord, gridSize)];
	}

	if (tileType == TileType::EndTile)
	{
		if (m_pStartTile)
		{
			if (m_pStartTile->GetTileCoord() == mouseTileCoord)
			{
				m_pStartTile = nullptr;
			}
		}

		if (m_pEndTile)
		{
			//Clearing the previous end tile.
			m_pEndTile->SetTileProperty(TileType::Default);
			m_pEndTile->UpdateTileProperty();
			m_pEndTile = nullptr; 
		}

		m_pEndTile = m_pTiles[GetTilesArrayIndex(mouseTileCoord, gridSize)];
	}

	if (tileType == TileType::Default || tileType == TileType::WallTile)
	{
		if (m_pStartTile)
		{
			if (m_pStartTile->GetTileCoord() == mouseTileCoord)
			{
				m_pStartTile = nullptr;
			}
		}
		if (m_pEndTile)
		{
			if (m_pEndTile->GetTileCoord() == mouseTileCoord)
			{
				m_pEndTile = nullptr;
			}
		}
	} 

	m_pTiles[GetTilesArrayIndex(mouseTileCoord, gridSize)]->SetTileProperty(tileType);
	m_pTiles[GetTilesArrayIndex(mouseTileCoord, gridSize)]->UpdateTileProperty();
}

void Grid::UpdateTileSelector(sf::Vector2u mouseTileCoord, sf::RenderWindow* pWindow, sf::Vector2f TopWidgetSize)
{
	m_pTileSelector->SetTileCoord(mouseTileCoord, pWindow);
	m_pTileSelector->RepositionTile(pWindow, TopWidgetSize);
}

bool Grid::IsTileCoordValid(sf::Vector2u tileCoord)
{
	if(tileCoord.x < 0 || (tileCoord.x > m_gridSize.x -1))
		return false;
	if (tileCoord.y < 0 || (tileCoord.y > m_gridSize.y - 1))
		return false;

	return true;
}

TileType Grid::GetTileState(sf::Vector2u tileCoord)
{
	if (IsTileCoordValid(tileCoord))
	{
		return m_pTiles[GetTilesArrayIndex(tileCoord, m_gridSize)]->GetTileState();
	}
	
	return TileType::InValid;
}
