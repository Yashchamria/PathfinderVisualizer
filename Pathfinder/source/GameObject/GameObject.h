#pragma once

class GameObject
{
public:	
	GameObject();
	virtual ~GameObject();

	//Game Core Functions
	virtual void Initialize();
	virtual void Update(float deltaTime);
	virtual void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow);
};
