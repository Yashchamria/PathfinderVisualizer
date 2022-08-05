#pragma once

class Scene;
class Grid;
class Selector;
class Display;
class AlgorithmManager;

class InputManager
{
public:	
	InputManager(const std::shared_ptr<Scene>& pScene);

public:
	void ProcessInputEvent(const std::shared_ptr<sf::Event>& pEvent, const std::shared_ptr<sf::Window>& pWindow) const;

private:
	const std::shared_ptr<Grid> m_pGrid;
	const std::shared_ptr<Selector> m_pSelector;
	const std::shared_ptr<Display> m_pDisplay;
	const std::shared_ptr<AlgorithmManager> m_pAlgorithmManager;
};