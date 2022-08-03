#pragma once

class Scene;

class InputManager
{
public:	
	InputManager(const std::shared_ptr<Scene>& pScene);

public:
	void ProcessInputEvent(const std::shared_ptr<sf::Event>& pEvent, const std::shared_ptr<sf::Window>& pWindow);

private:
	const std::shared_ptr<Scene> m_pScene;
};