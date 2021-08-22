#pragma once

enum class TileType : char;
class GameObject;
class TopHUDWidget;
class Grid;
class IAlgorithm;
class DijkstrasAlgorithm;

enum class AlgorithmVisualizationSpeed
{
	Slow = 8,
	Average = 24,
	Fast = 72,
	Instant = 216,
};

class Scene
{
public:	
	Scene(sf::RenderWindow* pWindow);
	~Scene();

	//Game Core Functions
	void Initialize();
	void Update(float deltaTime);
	void Draw(sf::RenderWindow* pWindow);

private:
	sf::RenderWindow* m_pWindow;
	std::vector<GameObject*> m_pGameObjects;

private:
	//HUD Variables
	TopHUDWidget* m_pTopHUDWidget;

	//Grid Variables
	Grid* m_pGrid = nullptr;
	IAlgorithm* m_pAlgorithm = nullptr;

	sf::Vector2u m_GridSize;
	sf::Vector2u m_ZoomedGridSize;
	
	AlgorithmVisualizationSpeed m_AlgorithmSpeed = AlgorithmVisualizationSpeed::Instant;
private:
	void InitializeGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow, unsigned int NumColumnZoom, sf::Vector2f TopWidgetSize);

public:
	void ResizeGrid(unsigned int NumColumn, sf::Vector2f TopWidgetSize);
	void ClearGrid();
	void ClearAlgorithmSearch();
	void UpdateTileSelector(sf::Vector2u mouseTileCoord, sf::RenderWindow* pWindow);
	void UpdateTileProperty(sf::Vector2u mouseTileCoord, TileType tileType);

public:
	//Grid Getter/Setters
	void SetZoomedGridSize(unsigned int ColumnSize) { m_ZoomedGridSize = sf::Vector2u(ColumnSize, (unsigned int)((float)ColumnSize / GameConst::GRID_ASPECT_RATIO)); }

	void SetAlgorithm(IAlgorithm* Algorithm) { m_pAlgorithm = Algorithm; }
	void ExecuteAlgorithm();
	bool IsAlogrithmRunning() { return m_AlgorithmExecuted; }
	void StopAlgorithm();
	void AlgorithmVisualizationSpeed(AlgorithmVisualizationSpeed speed) { m_AlgorithmSpeed = speed; }

	IAlgorithm* GetCurrentAlgorithm() { return m_pAlgorithm; }

	sf::Vector2u GetGridSize() { return m_GridSize; }
	sf::Vector2u GetZoomedGridSize() { return m_ZoomedGridSize; }

	sf::Vector2f GetTopWidgetSize();

public:
	bool m_AlgorithmExecuted = false;
	bool m_AnimationPending = false;

	
	//Algorithms declaration
	DijkstrasAlgorithm* m_pDijkstrasAlgorithm = nullptr;

};
