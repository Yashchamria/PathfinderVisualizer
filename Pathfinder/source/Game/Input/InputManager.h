#pragma once

class Command;

class InputManager
{
public:	
	InputManager(class Scene* pScene);
	~InputManager();

public:
	void ProcessInputEvent(sf::Event* pEvent, sf::RenderWindow* pWindow);

private:
	Command* m_pCommand;
};