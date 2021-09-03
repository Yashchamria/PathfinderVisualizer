#include "FrameworkPCH.h"
#include "Grid.h"

#include "Tile.h"
#include "TileEnum.h"

Grid::Grid()
{
	m_pTileSelector = new Tile();
	m_pTileSelector->SetTileColor(sf::Color::Transparent, sf::Color(250, 109, 5));
}

Grid::~Grid()
{
	m_pStartTile = nullptr;
	m_pEndTile = nullptr;

	for (Tile* pTile : m_pTiles)
	{
		delete pTile;
	}
	
	delete m_pTileSelector;
}

void Grid::Update(float deltaTime)
{
	for (Tile* pTile : m_pTiles)
	{
		pTile->Update(deltaTime);
	}
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
		pTile->SetTileType(TileType::Default);
		pTile->UpdateTileType();
	}

	m_pStartTile = nullptr;
	m_pEndTile = nullptr;
}

void Grid::ClearAlgorithmSearch()
{
	for (Tile* pTile : m_pTiles)
	{
		if (pTile->GetTileType() == TileType::Default)
		{
			pTile->SetTileAnimationProperty(TileAnimationState::Idle);
			pTile->UpdateTileAnimationProperty();
		}
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
			m_pStartTile->SetTileType(TileType::Default);
			m_pStartTile->UpdateTileType();
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
			m_pEndTile->SetTileType(TileType::Default);
			m_pEndTile->UpdateTileType();
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

	m_pTiles[GetTilesArrayIndex(mouseTileCoord, gridSize)]->SetTileType(tileType);
	m_pTiles[GetTilesArrayIndex(mouseTileCoord, gridSize)]->UpdateTileType();
}

void Grid::UpdateTileSelector(sf::Vector2u mouseTileCoord, sf::RenderWindow* pWindow, sf::Vector2f TopWidgetSize)
{
	m_pTileSelector->SetTileCoord(mouseTileCoord, pWindow);
	m_pTileSelector->RepositionTile(pWindow, TopWidgetSize);
}

Tile* Grid::GetNeighbourTile(sf::Vector2u CurrentTileCoord, NeighbourTileDirection tileDirection)
{
	sf::Vector2u neighbourTileCoord(0, 0);
	
	switch (tileDirection)
	{
	case NeighbourTileDirection::Up:
		neighbourTileCoord = sf::Vector2u(CurrentTileCoord.x, CurrentTileCoord.y - 1);
		break;

	case NeighbourTileDirection::Down:
		neighbourTileCoord = sf::Vector2u(CurrentTileCoord.x, CurrentTileCoord.y + 1);
		break;

	case NeighbourTileDirection::Right:
		neighbourTileCoord = sf::Vector2u(CurrentTileCoord.x + 1, CurrentTileCoord.y);
		break;

	case NeighbourTileDirection::Left:
		neighbourTileCoord = sf::Vector2u(CurrentTileCoord.x - 1, CurrentTileCoord.y);
		break;
	}

	if (IsTileCoordValid(neighbourTileCoord))
	{
		return GetTile(neighbourTileCoord);
	}

	return nullptr;
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
		return m_pTiles[GetTilesArrayIndex(tileCoord, m_gridSize)]->GetTileType();
	}
	
	return TileType::InValid;
}

void Grid::GenerateRandomWalls(unsigned int wallPercent)
{
	ClearGrid();

	unsigned int totaltiles = m_pTiles.size();

	unsigned int totalWalls = (totaltiles * wallPercent) / 100;

	for (unsigned int i = 0; i < totalWalls; i++)
	{
		unsigned index = rand() % totaltiles;

		if (m_pTiles[index]->GetTileType() != TileType::WallTile)
		{
			m_pTiles[index]->SetTileType(TileType::WallTile);
			m_pTiles[index]->UpdateTileType();
		}
		else
		{
			i--;
		}
	}
}

void Grid::GenerateRandomTile(TileType tileType, unsigned int quadrant)
{
	sf::Vector2u QuadrantSize((unsigned int)(m_gridSize.x / 2.0f), (unsigned int)(m_gridSize.y / 2.0f));

	sf::Vector2u RandomTileCoord(0, 0);

	if (quadrant <= 1)
	{
		RandomTileCoord.x = (rand() % QuadrantSize.x) + QuadrantSize.x;
		RandomTileCoord.y = rand() % QuadrantSize.y;
	}
	else if (quadrant == 2)
	{
		RandomTileCoord.x = rand() % QuadrantSize.x;
		RandomTileCoord.y = rand() % QuadrantSize.y;
	}
	else if (quadrant == 3)
	{
		RandomTileCoord.x = rand() % QuadrantSize.x;
		RandomTileCoord.y = (rand() % QuadrantSize.y) + QuadrantSize.y;
	}
	else
	{
		RandomTileCoord.x = (rand() % QuadrantSize.x) + QuadrantSize.x;
		RandomTileCoord.y = (rand() % QuadrantSize.y) + QuadrantSize.y;
	}

	UpdateTileProperty(RandomTileCoord, m_gridSize, tileType);
}
