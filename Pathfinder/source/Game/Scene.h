#pragma once

enum class TileType : char;
enum class AlgorithmType : char;
enum class AlgorithmState : char;
enum class AlgorithmVisualSpeed : int;
struct AlgorithmData;


class GameObject;
class TopHUDWidget;
class Grid;
class Algorithm;

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
	TopHUDWidget* m_pTopHUDWidget;

	Grid* m_pGrid = nullptr;
	sf::Vector2u m_GridSize;
	sf::Vector2u m_ZoomedGridSize;
	
	Algorithm* m_pAlgorithm = nullptr;
	AlgorithmVisualSpeed m_AlgorithmSpeed;

private:
	bool m_AlgorithmExecuted = false;

private:
	void InitializeGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow, unsigned int NumColumnZoom, sf::Vector2f TopWidgetSize);

public:
	//Grid helper functions
	void ResizeGrid(unsigned int NumColumn, sf::Vector2f TopWidgetSize);
	void ClearGrid();
	void ClearAlgorithmSearch();
	void UpdateTileSelector(sf::Vector2u mouseTileCoord, sf::RenderWindow* pWindow);
	void UpdateTileProperty(sf::Vector2u mouseTileCoord, TileType tileType);

	void SetZoomedGridSize(unsigned int ColumnSize) { m_ZoomedGridSize = sf::Vector2u(ColumnSize, (unsigned int)((float)ColumnSize / GameConst::GRID_ASPECT_RATIO)); }
	sf::Vector2u GetGridSize() { return m_GridSize; }
	sf::Vector2u GetZoomedGridSize() { return m_ZoomedGridSize; }

	//Algorithm Helper functions
	void ExecuteAlgorithm(AlgorithmType algorithmType);
	void StopAlgorithm();
	void SetAlgorithmVisualSpeed(AlgorithmVisualSpeed speed) { m_AlgorithmSpeed = speed; }
	AlgorithmVisualSpeed GetAlgorithmVisualSpeed() { return m_AlgorithmSpeed; }
	AlgorithmState GetAlgorithmState();


	AlgorithmData* m_pCurrentAlgorithmData = nullptr;
	AlgorithmData* m_pPreviousAlgorithmData = nullptr;

	//Top Widget helper function
	void AutoUpdateTopWidget();
	void UpdateTopWidgetLabels(unsigned int LabelNum, std::string AppendString);
	void UpdateWidgetLog(std::string AppendString);

	sf::Vector2f GetTopWidgetSize();
};
