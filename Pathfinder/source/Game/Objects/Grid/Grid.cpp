#include "FrameworkPCH.h"
#include "Grid.h"

#include "Tile.h"
#include "TileType.h"
#include "Direction.h"

Grid::Grid(const sf::Vector2u gridSize, const sf::Vector2f windowSize, const sf::Vector2f displaySize)
	: GridSize(gridSize), m_columnZoomLevel(gridSize.x)
{
	// Setting up the widget box.
	m_pCanvas = std::make_unique<sf::RectangleShape>(sf::Vector2f(windowSize.x, windowSize.y - displaySize.y));
	m_pCanvas->setFillColor(GRID_COLOR);
	m_pCanvas->setPosition(-sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f - displaySize.y));

	const float tileSize = windowSize.x / (float)gridSize.x;

	m_pTiles.reserve(GridSize.x * GridSize.y);

	for (int x = 0; x < GridSize.x; x++)
	{
		for (int y = 0; y < GridSize.y; y++)
		{
			const auto& pTile =std::make_shared<Tile>(sf::Vector2u(x, y),
				tileSize * (1.0f - Config::gridOutlineStrength), 10, TileType::Default);
			m_pTiles.push_back(pTile);
		}
	}
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

void Grid::Zoom(const float value)
{
	auto resizeTiles = [&]
	{
		const float tileSize = Config::windowWidth / (float)m_columnZoomLevel;

		for (const auto& pTile : m_pTiles)
		{
			pTile->SetSizeAndPosition(tileSize * (1.0f - Config::gridOutlineStrength));
		}
	};

	m_zoomSteps += value;

	if (m_zoomSteps > Config::mouseSensitivity)
	{
		m_zoomSteps = 0.0f;

		if (m_columnZoomLevel + 1 <= GridSize.x)
		{
			++m_columnZoomLevel;
			resizeTiles();
		}
	}
	else if (m_zoomSteps < -Config::mouseSensitivity)
	{
		m_zoomSteps = 0.0f;

		if (m_columnZoomLevel - 1 > 16)
		{
			--m_columnZoomLevel;
			resizeTiles();
		}
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
		case TileType::Default:
		{
			if (m_startIndex == index) { m_startIndex = -1; }
			if (m_endIndex == index) { m_endIndex = -1; }

			pTile->Type = TileType::Default;
			pTile->Animate(DEFAULT_TILE_COLOR);
		}
		break;

		case TileType::WallTile:
		{
			if (m_startIndex == index) { m_startIndex = -1; }
			if (m_endIndex == index) { m_endIndex = -1; }

			pTile->Type = TileType::WallTile;
			pTile->Animate(WALL_TILE_COLOR);
		}
		break;

		case TileType::EndTile:
		{
			if (m_endIndex != -1)
			{
				m_pTiles[m_endIndex]->Type = TileType::Default;
				m_pTiles[m_endIndex]->Animate(DEFAULT_TILE_COLOR);
			}

			if (m_startIndex == index) { m_startIndex = -1; }
			m_endIndex = index;
			pTile->Type = TileType::EndTile;
			pTile->Animate(END_TILE_COLOR);
		}
		break;

		case TileType::StartTile:
		{
			if(m_startIndex != -1)
			{
				m_pTiles[m_startIndex]->Type = TileType::Default;
				m_pTiles[m_startIndex]->Animate(DEFAULT_TILE_COLOR);
			}

			if (m_endIndex == index) { m_endIndex = -1; }
			m_startIndex = index;
			pTile->Type = TileType::StartTile;
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

std::vector<uint32_t> Grid::GetValidNeighborIndices(const uint32_t index) const
{
	const sf::Vector2u coord = m_pTiles[index]->Coord;

	std::vector<uint32_t> neighbors;

	for (const auto& neighbor : GetNeighborCoords(coord))
	{
		if(IsCoordValid(neighbor))
		{
			neighbors.push_back(GetTileIndex(neighbor));
		}
	}

	return neighbors;
}


void Grid::GenerateRandomWalls(const int wallPercent) const
{
	for (int i = 0, loop = 0; i < (int)m_pTiles.size() * wallPercent / 100;)
	{
		const int index = rand() % m_pTiles.size();

		if (m_pTiles[index]->Type != TileType::WallTile)
		{
			m_pTiles[index]->Type = TileType::WallTile;
			m_pTiles[index]->Animate(WALL_TILE_COLOR);
			i++;
		}

		if(++loop > GridSize.x * GridSize.y) { break; }
	}
}