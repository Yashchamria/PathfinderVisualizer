#pragma once

enum class TileType : char;
class GameObject;
class Grid;
class IAlgorithm;
class DijkstrasAlgorithm;

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
	//Grid Variables
	sf::Vector2u m_GridSize;
	sf::Vector2u m_ZoomedGridSize;
	Grid* m_pGrid = nullptr;
	IAlgorithm* m_pAlgorithm = nullptr;

private:
	void InitializeGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow, unsigned int NumColumnZoom);

public:
	void ResizeGrid(unsigned int NumColumn);
	void ClearGrid();
	void UpdateTileSelector(sf::Vector2u mouseTileCoord, sf::RenderWindow* pWindow);
	void UpdateTileProperty(sf::Vector2u mouseTileCoord, TileType tileType);

public:
	//Grid Getter/Setters
	void SetZoomedGridSize(unsigned int ColumnSize)
	{
		m_ZoomedGridSize = sf::Vector2u(ColumnSize, 
										(unsigned int)((float)ColumnSize / GameConst::WINDOW_ASPECT_RATIO));
	}

	void SetAlgorithm(IAlgorithm* Algorithm) { m_pAlgorithm = Algorithm; }
	void ExecuteAlgorithm();

	sf::Vector2u GetGridSize() { return m_GridSize; }
	sf::Vector2u GetZoomedGridSize() { return m_ZoomedGridSize; }

public:

	//Algorithms declaration
	DijkstrasAlgorithm* m_pDijkstrasAlgorithm = nullptr;

};
