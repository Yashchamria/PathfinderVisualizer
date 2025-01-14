#pragma once

class Scene;
class InputManager;

class Game final
{
public:	
	Game(const std::string& windowName, const sf::Vector2u windowSize);
	void Run() const;

private:
	void ProcessGameWindowEvents(const std::shared_ptr<sf::Event>& event) const;
	void Draw() const;

private:
	static void PrintGameInfo();

private:
	std::shared_ptr<sf::RenderWindow> m_pRenderWindow;
	std::shared_ptr<sf::View> m_pView;
	std::shared_ptr<Scene> m_pScene;
	std::shared_ptr<InputManager> m_pInputManager;
};