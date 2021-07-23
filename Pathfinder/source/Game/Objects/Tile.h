#pragma once
#include "../../GameCore/Objects/GameObject.h"


class Tile : public GameObject
{
public:	
	Tile();
	~Tile();

	//Game Core Functions
	virtual void Initialize() override;
	virtual void PostInitialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow* pWindow) override;

	sf::RectangleShape rect{sf::Vector2f(100.0f, 100.0f)};
};
