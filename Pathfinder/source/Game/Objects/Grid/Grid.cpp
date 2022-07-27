#include "FrameworkPCH.h"
#include "Grid.h"

#include "Tile.h"
#include "TileEnum.h"

Grid::Grid(const sf::Vector2u gridSize, const sf::Vector2u windowSize, const sf::Vector2f displaySize) : m_gridSize(gridSize)
{
	m_pSelector = std::make_shared<Tile>();
	m_pSelector->SetTileColor(sf::Color::Transparent, sf::Color(250, 109, 5));

	m_pTiles.reserve(m_gridSize.x * m_gridSize.y);

	for (int x = 0; x < m_gridSize.x; x++)
	{
		for (int y = 0; y < m_gridSize.y; y++)
		{
			const auto& pTile = std::make_shared<Tile>();
			pTile->SetTileCoord(sf::Vector2u(x, y));
			pTile->RepositionTile();
			m_pTiles.push_back(pTile);
		}
	}

	SetZoomedGridSize(gridSize.x);
	ResizeGrid(gridSize.x, windowSize, displaySize);
}

Grid::~Grid()
{
	m_pTiles.clear();
}

void Grid::Update(float deltaTime)
{
	for (const auto& pTile : m_pTiles)
	{
		pTile->Update(deltaTime);
	}
}

void Grid::Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow)
{
	for (const auto& pTile : m_pTiles)
	{
		pTile->Draw(renderWindow);
	}

	m_pSelector->Draw(renderWindow);
}

void Grid::ResizeGrid(unsigned int numberOfColumns, sf::Vector2u windowSize, sf::Vector2f TopWidgetSize) const
{
	const float tileSize = windowSize.x / (float)numberOfColumns;

	for (const auto& pTile : m_pTiles)
	{
		pTile->SetTileSize(sf::Vector2f(tileSize, tileSize), 20.0f);
		pTile->RepositionTile();
	}

	m_pSelector->SetTileSize(sf::Vector2f(tileSize, tileSize), 8.0f);
	m_pSelector->RepositionTile();
}

void Grid::SetSelectorPosition(sf::Vector2u mouseTileCoord)
{
	if (mouseTileCoord.x < m_ZoomedGridSize.x && mouseTileCoord.y < m_ZoomedGridSize.y)
	{
		UpdateTileSelector(mouseTileCoord);
	}
}

void Grid::ClearGrid()
{
	for (const auto& pTile : m_pTiles)
	{
		pTile->SetTileType(TileType::Default);
		pTile->UpdateTileType();
	}

	m_startIndex = -1;
	m_endIndex = -1;
}

void Grid::ClearAlgorithmSearch() const
{
	for (const auto& pTile : m_pTiles)
	{
		if (pTile->GetTileType() == TileType::Default)
		{
			pTile->SetTileAnimationProperty(TileAnimState::Idle);
			pTile->UpdateTileAnimationProperty();
		}
	}
}


void Grid::UpdateTileProperty(sf::Vector2u mouseTileCoord, TileType tileType)
{
	if (!IsCoordValid(mouseTileCoord)) { return; }

	if (tileType == TileType::StartTile)
	{
		if (m_endIndex != -1)
		{
			if (m_endIndex == GetTileIndex(mouseTileCoord))
			{
				m_endIndex = -1;
			}
		}

		if (m_startIndex != -1)
		{
			//Clearing the previous start tile.
			GetStartTile()->SetTileType(TileType::Default);
			GetStartTile()->UpdateTileType();
			m_startIndex = -1;
		}
		m_startIndex = GetTileIndex(mouseTileCoord);
	}

	if (tileType == TileType::EndTile)
	{
		if (m_startIndex != -1)
		{
			if (m_startIndex == GetTileIndex(mouseTileCoord))
			{
				m_startIndex = -1;
			}
		}

		if (m_endIndex != -1)
		{
			//Clearing the previous end tile.
			GetEndTile()->SetTileType(TileType::Default);
			GetEndTile()->UpdateTileType();
			m_endIndex = -1;
		}

		m_endIndex = GetTileIndex(mouseTileCoord);
	}

	if (tileType == TileType::Default || tileType == TileType::WallTile)
	{
		if (m_startIndex != -1)
		{
			if (m_startIndex == GetTileIndex(mouseTileCoord))
			{
				m_startIndex = -1;
			}
		}
		if (m_endIndex != -1)
		{
			if (m_endIndex == GetTileIndex(mouseTileCoord))
			{
				m_endIndex = -1;
			}
		}
	} 

	m_pTiles[GetTileIndex(mouseTileCoord)]->SetTileType(tileType);
	m_pTiles[GetTileIndex(mouseTileCoord)]->UpdateTileType();
}

void Grid::UpdateTileSelector(sf::Vector2u mouseTileCoord)
{
	m_pSelector->SetTileCoord(mouseTileCoord);
	m_pSelector->RepositionTile();
}

std::shared_ptr<Tile> Grid::GetNeighborTile(sf::Vector2u coord, const Direction direction) const
{
	switch (direction)
	{
		case Direction::Up: --coord.y; break;
		case Direction::Down: ++coord.y; break;
		case Direction::Right: ++coord.x; break;
		case Direction::Left: --coord.x; break;
	}
	return IsCoordValid(coord) ? GetTile(coord) : std::shared_ptr<Tile>(nullptr);
}

void Grid::GenerateRandomWalls(const int wallPercent) const
{
	for (int i = 0; i < m_pTiles.size() * wallPercent / 100;)
	{
		const int index = rand() % m_pTiles.size();

		if (m_pTiles[index]->GetTileType() != TileType::WallTile)
		{
			m_pTiles[index]->SetTileType(TileType::WallTile);
			m_pTiles[index]->UpdateTileType();
			i++;
		}
	}
}