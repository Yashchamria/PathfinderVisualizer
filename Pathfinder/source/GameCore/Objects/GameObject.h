#pragma once

class GameObject
{
public:	
	GameObject();
	virtual ~GameObject();

	//Game Core Functions
	virtual void Initialize();
	virtual void Update(float deltaTime);
	virtual void Draw(sf::RenderWindow* pWindow);
};
