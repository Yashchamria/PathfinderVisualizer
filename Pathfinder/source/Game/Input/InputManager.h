#pragma once

class Command;
class Scene;

class InputManager
{
public:	
	InputManager(Scene* pScene);
	~InputManager();

public:
	void ProcessInputEvent(sf::Event* pEvent, sf::RenderWindow* pWindow);

private:
	Command* m_pCommand;
};