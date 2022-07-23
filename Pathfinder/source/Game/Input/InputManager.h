#pragma once

class Command;

class InputManager
{
public:	
	InputManager(class Scene* pScene);
	~InputManager();

public:
	void ProcessInputEvent(sf::Event* pEvent, sf::RenderWindow* pWindow, const sf::Vector2f displaySize);

private:
	Command* m_pCommand;
};