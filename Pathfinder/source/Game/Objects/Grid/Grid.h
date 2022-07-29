#pragma once
#include "GameCore/Objects/GameObject.h"

class Tile;
enum class TileType : char;
enum class Direction : char;

class Grid final: public GameObject
{
public:	
	Grid(const sf::Vector2u gridSize, const sf::Vector2f windowSize, const sf::Vector2f displaySize);
	~Grid() override;

	void Update(float deltaTime) override;
	void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;

private:
	std::unique_ptr<sf::RectangleShape> m_pCanvas{};
	std::vector<std::shared_ptr<Tile>> m_pTiles;
	int m_startIndex {-1}, m_endIndex {-1};

	const sf::Vector2u m_gridSize;
	sf::Vector2u m_ZoomedGridSize;

public:
	void ResizeGrid(unsigned int numberOfColumns, sf::Vector2f windowSize, sf::Vector2f TopWidgetSize) const;
	void SetTileType(const sf::Vector2u coord, const TileType type);

	void ClearGrid();
	void ResetDefaultTiles() const;

	void SetZoomedGridSize(unsigned int ColumnSize) { m_ZoomedGridSize = sf::Vector2u(ColumnSize, (unsigned int)((float)ColumnSize / ((float)m_gridSize.x / (float)m_gridSize.y))); }
	[[nodiscard]] sf::Vector2u GetZoomedGridSize() const { return m_ZoomedGridSize; }

private:
	[[nodiscard]] bool IsIndexValid(const int index) const { return index > -1 && index < m_pTiles.size(); }
	[[nodiscard]] int GetTileIndex(const sf::Vector2u coord) const { return coord.y + coord.x * m_gridSize.y; }

public:
	[[nodiscard]] sf::Vector2u GetGridSize() { return m_gridSize; }
	[[nodiscard]] bool IsCoordValid(const sf::Vector2u coord) const { return coord.x < m_gridSize.x&& coord.y < m_gridSize.y; }

public:
	void GenerateRandomWalls(const int wallPercent) const;

public:
	[[nodiscard]] const std::shared_ptr<Tile>& GetTile(const sf::Vector2u coord) const { return m_pTiles[GetTileIndex(coord)]; }
	[[nodiscard]] std::shared_ptr<Tile> GetNeighborTile(sf::Vector2u coord, const Direction direction) const;
	[[nodiscard]] std::shared_ptr<Tile> GetStartTile() const { return IsIndexValid(m_startIndex) ? m_pTiles[m_startIndex] : std::shared_ptr<Tile>(nullptr); }
	[[nodiscard]] std::shared_ptr<Tile> GetEndTile() const { return IsIndexValid(m_endIndex) ? m_pTiles[m_endIndex] : std::shared_ptr<Tile>(nullptr); }
};