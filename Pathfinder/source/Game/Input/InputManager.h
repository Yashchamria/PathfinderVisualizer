#pragma once

class InputManager
{
public:	
	InputManager(class Scene* &pScene);
	~InputManager();

public:
	void ProcessInputEvent(sf::Event* pEvent, sf::RenderWindow* pWindow);

private:
	class Scene* m_pScene;
	unsigned int m_ZoomInSteps = 0, m_ZoomOutSteps = 0;

	sf::Vector2u m_MouseTileCoord = sf::Vector2u(0, 0);

private:
	void ResizeGrid(int ColumnIncrement, unsigned int ScrollSteps = 6);
	sf::Vector2u GetMouseTileCoord(sf::Vector2i mousePosition, sf::RenderWindow* pWindow);
	//void GetCurrentTileSize(unsigned int GridZoom, sf::RenderWindow* pWindow);

};