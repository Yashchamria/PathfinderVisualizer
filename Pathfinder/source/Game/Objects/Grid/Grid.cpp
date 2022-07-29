#include "FrameworkPCH.h"
#include "Grid.h"

#include "Tile.h"
#include "TileType.h"
#include "Direction.h"

Grid::Grid(const sf::Vector2u gridSize, const sf::Vector2f windowSize, const sf::Vector2f displaySize) : m_gridSize(gridSize)
{
	// Setting up the widget box.
	m_pCanvas = std::make_unique<sf::RectangleShape>(sf::Vector2f(windowSize.x, windowSize.y - displaySize.y));
	m_pCanvas->setFillColor(GRID_COLOR);
	m_pCanvas->setPosition(-sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f - displaySize.y));

	const float tileSize = windowSize.x / (float)gridSize.x;

	m_pTiles.reserve(m_gridSize.x * m_gridSize.y);

	for (int x = 0; x < m_gridSize.x; x++)
	{
		for (int y = 0; y < m_gridSize.y; y++)
		{
			const auto& pTile =std::make_shared<Tile>(sf::Vector2u(x, y),
				tileSize * (1.0f - Config::gridOutlineStrength), 10, TileType::Default);
			m_pTiles.push_back(pTile);
		}
	}

	SetZoomedGridSize(gridSize.x);
}

Grid::~Grid() { m_pTiles.clear(); }

void Grid::Update(float deltaTime)
{
	for (const auto& pTile : m_pTiles)
	{
		pTile->Update(deltaTime);
	}
}

void Grid::Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow)
{
	renderWindow->draw(*m_pCanvas);
	for (const auto& pTile : m_pTiles)
	{
		pTile->Draw(renderWindow);
	}
}

void Grid::ResizeGrid(unsigned int numberOfColumns, sf::Vector2f windowSize, sf::Vector2f TopWidgetSize) const
{
	const float tileSize = windowSize.x / (float)numberOfColumns;

	for (const auto& pTile : m_pTiles)
	{
		pTile->SetSizeAndPosition(tileSize * (1.0f - Config::gridOutlineStrength));
	}
}

void Grid::ClearGrid()
{
	for (const auto& pTile : m_pTiles)
	{
		pTile->Type = TileType::Default;
		pTile->Animate(DEFAULT_TILE_COLOR);
	}

	m_startIndex = -1;
	m_endIndex = -1;
}

void Grid::ResetDefaultTiles() const
{
	for (const auto& pTile : m_pTiles)
	{
		if (pTile->Type == TileType::Default)
		{
			pTile->Animate(DEFAULT_TILE_COLOR);
		}
	}
}

void Grid::SetTileType(const sf::Vector2u coord, const TileType type)
{
	if (!IsCoordValid(coord)) { return; }
	const int index = GetTileIndex(coord);
	const auto& pTile = m_pTiles[index];

	switch(type)
	{
		case TileType::Default: case TileType::WallTile:
		{
			if (m_startIndex == index) { m_startIndex = -1; }
			if (m_endIndex == index) { m_endIndex = -1; }
			pTile->Animate(type == TileType::Default ? DEFAULT_TILE_COLOR : WALL_TILE_COLOR);
		}
		break;

		case TileType::EndTile:
		{
			if (m_startIndex == index) { m_startIndex = -1; }
			m_endIndex = index;
			pTile->Animate(END_TILE_COLOR);
		}
		break;

		case TileType::StartTile:
		{
			if (m_endIndex == index) { m_endIndex = -1; }
			m_startIndex = index;
			pTile->Animate(START_TILE_COLOR);
		}
		break;
	}
}

std::shared_ptr<Tile> Grid::GetNeighborTile(sf::Vector2u coord, const Direction direction) const
{
	coord = GetNeighborCoord(coord, direction);
	return IsCoordValid(coord) ? GetTile(coord) : std::shared_ptr<Tile>(nullptr);
}


void Grid::GenerateRandomWalls(const int wallPercent) const
{
	for (int i = 0; i < (int)m_pTiles.size() * wallPercent / 100;)
	{
		const int index = rand() % m_pTiles.size();

		if (m_pTiles[index]->Type != TileType::WallTile)
		{
			m_pTiles[index]->Type = TileType::WallTile;
			m_pTiles[index]->Animate(WALL_TILE_COLOR);
			i++;
		}
	}
}