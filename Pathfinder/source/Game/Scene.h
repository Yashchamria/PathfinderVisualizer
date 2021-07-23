#pragma once

class Scene
{
public:	
	Scene();
	~Scene();

	//Game Core Functions
	void Initialize();
	void PostInitialize();
	void Update(float deltaTime);
	void Draw(sf::RenderWindow* pWindow);

private:
	std::vector<class GameObject*> m_pGameObjects;

	class Tile* m_pTile = nullptr;
};
