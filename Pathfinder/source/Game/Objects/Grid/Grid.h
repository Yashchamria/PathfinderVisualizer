#pragma once
#include "GameCore/Objects/GameObject.h"

class Grid : public GameObject
{
public:	
	Grid();
	~Grid();

	virtual void Draw(sf::RenderWindow* pWindow) override;

private:
	std::vector<class Tile*> m_pTiles;

public:
	void GenerateGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow);
	void ResizeGrid(unsigned int numberOfColumns, sf::RenderWindow* pWindow);
};
