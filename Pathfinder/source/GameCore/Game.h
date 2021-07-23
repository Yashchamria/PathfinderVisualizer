#pragma once

class Game
{
public:	
	Game();
	~Game();

	//Game Core Functions
	void Init();
	void Run();

private:
	void ProcessGameWindowEvents();
	void Update(float deltaTime);
	void Draw();

private:
	sf::RenderWindow* m_pGameWindow = nullptr;
	sf::View* m_pGameView;
	sf::Event* m_pEvent;

	class Scene* m_pScene;
};
