#pragma once
#include "GameObject/GameObject.h"

class Tile;
enum class TileType : char;
enum class Direction : char;

class Grid final: public GameObject
{
public:	
	Grid(const sf::Vector2u gridSize, const sf::Vector2f windowSize, const sf::Vector2f displaySize, const sf::Font& font);
	~Grid() override;

	void Update(float deltaTime) override;
	void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;

public:
	const sf::Vector2u GridSize;

private:
	std::unique_ptr<sf::RectangleShape> m_pCanvas{};
	std::vector<std::shared_ptr<Tile>> m_pTiles;
	int m_startIndex {-1}, m_endIndex {-1};

private:
	uint64_t m_columnZoomLevel;
	float m_zoomSteps;

public:
	void Zoom(const float value);
	void SetTileType(const sf::Vector2u coord, const TileType type);

	void ClearGrid();
	void ResetDefaultTiles() const;

	void GenerateRandomWalls(const int wallPercent) const;

public:
	[[nodiscard]] const std::shared_ptr<Tile>& GetTile(const sf::Vector2u coord) const { return m_pTiles[GetTileIndex(coord)]; }
	[[nodiscard]] const std::shared_ptr<Tile>& GetTile(const uint32_t index) const { return m_pTiles[index]; }

	[[nodiscard]] std::shared_ptr<Tile> GetNeighborTile(sf::Vector2u coord, const Direction direction) const;
	[[nodiscard]] std::vector<uint32_t> GetValidNeighborIndices(const uint32_t index) const;

	[[nodiscard]] uint32_t GetStartIndex() const { return m_startIndex; }
	[[nodiscard]] std::shared_ptr<Tile> GetStartTile() const { return IsIndexValid(m_startIndex) ? m_pTiles[m_startIndex] : std::shared_ptr<Tile>(nullptr); }

	[[nodiscard]] uint32_t GetEndIndex() const { return m_endIndex; }
	[[nodiscard]] std::shared_ptr<Tile> GetEndTile() const { return IsIndexValid(m_endIndex) ? m_pTiles[m_endIndex] : std::shared_ptr<Tile>(nullptr); }

	[[nodiscard]] bool IsCoordValid(const sf::Vector2u coord) const { return coord.x < GridSize.x && coord.y < GridSize.y; }
	[[nodiscard]] bool IsIndexValid(const int index) const { return index > -1 && index < m_pTiles.size(); }
	[[nodiscard]] int GetTileIndex(const sf::Vector2u coord) const { return coord.y + coord.x * GridSize.y; }

	[[nodiscard]] uint64_t GetZoomLevel() const { return m_columnZoomLevel; }
};