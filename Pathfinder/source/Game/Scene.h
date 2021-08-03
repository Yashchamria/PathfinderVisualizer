#pragma once

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
	std::vector<class GameObject*> m_pGameObjects;

private:
	//Grid Variables
	sf::Vector2u m_GridSize;
	unsigned int m_GridZoomOnScreen = 16;

	class Grid* m_pGrid = nullptr;

private:
	void InitializeGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow, unsigned int NumColumnZoom);

public:
	void ResizeGrid(unsigned int NumColumn);

public:
	//Grid Getter/Setters
	sf::Vector2u GetGridSize() { return m_GridSize; }

	void SetGridZoomOnScreen(unsigned int gridZoomOnScreen) { m_GridZoomOnScreen = gridZoomOnScreen; }
	unsigned int GetGridZoomOnScreen() { return m_GridZoomOnScreen; }

};
