#include "FrameworkPCH.h"
#include "Grid.h"

#include "Tile.h"

Grid::Grid()
{
}

Grid::~Grid()
{
	for (Tile* pTile : m_pTiles)
	{
		delete pTile;
	}
}

void Grid::Draw(sf::RenderWindow* pWindow)
{
	GameObject::Draw(pWindow);

	for (Tile* pTile : m_pTiles)
	{
		pTile->Draw(pWindow);
	}
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
			pTile->SetTileColor(sf::Color(sf::Color(rand() % 255, rand() % 255, rand() % 255, 255)));

			m_pTiles.push_back(pTile);
		}
	}
}

void Grid::ResizeGrid(unsigned int numberOfColumns, sf::RenderWindow* pWindow)
{
	float tileSize = pWindow->getSize().x / (float)numberOfColumns;

	for (Tile* pTile : m_pTiles)
	{
		pTile->SetTileSize(sf::Vector2f(tileSize, tileSize));
		pTile->RepositionTile(pWindow);
	}
}