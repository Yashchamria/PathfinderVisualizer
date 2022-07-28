//A simple facade class to keep the Input Manager clean and not exposing unnecessary functions and members to it.

#pragma once

enum class Direction : char;
class Scene;
enum class TileType : char;
enum class AlgorithmType : char;

//enum class TileSelectorMove : char
//{
//	Up,
//	Down,
//	Left,
//	Right,
//	Mouse
//};

class Command
{
public:	
	Command(Scene* pScene);
	~Command();

public:
	void ResizeGrid(int ColumnIncrement, unsigned int ScrollSteps = 6);
	void SetSelectorPosition(sf::RenderWindow* pWindow, Direction direction);
	void SetSelectorPosition(sf::RenderWindow* pWindow);

	void UpdateTileProperty(TileType tileType);
	void ExecuteAlgorithm(AlgorithmType algorithmType);
	void ClearAlgorithmSearch();
	void ClearGrid();
	void IncreaseVisualSpeed();
	void DecreaseVisualSpeed();
	void GenerateRandomGrid(int wallPercent);

private:
	class Scene* m_pScene;

	unsigned int m_ZoomInSteps = 0, m_ZoomOutSteps = 0;
	sf::Vector2u m_mouseCoord = sf::Vector2u(0, 0);

	AlgorithmType m_CurrentAlgorithm;

private:
	sf::Vector2u GetMouseTileCoord(sf::Vector2i mousePosition, sf::RenderWindow* pWindow);
	void HandleOngoingAlgorithm(bool bReRunAlgorithm = false);
	bool m_AlgorithmStopped = false;
};