#include "FrameworkPCH.h"
#include "InputManager.h"

#include "Command.h"

#include "Game/Objects/Grid/TileEnum.h"
#include "Game/Algorithms/AlgorithmEnum.h"

InputManager::InputManager(Scene* pScene)
{
	m_pCommand = new Command(pScene);
}

InputManager::~InputManager()
{
	delete m_pCommand;
}

void InputManager::ProcessInputEvent(sf::Event* pEvent, sf::RenderWindow* pWindow)
{
	switch (pEvent->type)
	{
	case sf::Event::MouseWheelMoved:

		if		(pEvent->mouseWheel.delta > 0) { m_pCommand->ResizeGrid(-8, 5); }
		else if (pEvent->mouseWheel.delta < 0) { m_pCommand->ResizeGrid( 8, 5); }

		break;

	case sf::Event::KeyPressed:

		if (pEvent->key.code == sf::Keyboard::Subtract || pEvent->key.code == sf::Keyboard::Dash) { m_pCommand->ResizeGrid(-8, 10); }
		else if (pEvent->key.code == sf::Keyboard::Add || pEvent->key.code == sf::Keyboard::Equal) { m_pCommand->ResizeGrid(8, 10); }
		
		if		(pEvent->key.code == sf::Keyboard::S) { m_pCommand->UpdateTileProperty(TileType::StartTile); }
		else if (pEvent->key.code == sf::Keyboard::E) { m_pCommand->UpdateTileProperty(TileType::EndTile  ); }
		else if (pEvent->key.code == sf::Keyboard::W) { m_pCommand->UpdateTileProperty(TileType::WallTile ); }
		else if (pEvent->key.code == sf::Keyboard::D) { m_pCommand->UpdateTileProperty(TileType::Default  ); }
		
		if (pEvent->key.code == sf::Keyboard::R) { m_pCommand->GenerateRandomGrid(25, 2, 4); }

		if      (pEvent->key.code == sf::Keyboard::Down ) { m_pCommand->UpdateTileSelectorPosition(TileSelectorMove::Down , pWindow); }
		else if (pEvent->key.code == sf::Keyboard::Up   ) { m_pCommand->UpdateTileSelectorPosition(TileSelectorMove::Up   , pWindow); }
		else if (pEvent->key.code == sf::Keyboard::Right) { m_pCommand->UpdateTileSelectorPosition(TileSelectorMove::Right, pWindow); }
		else if (pEvent->key.code == sf::Keyboard::Left ) { m_pCommand->UpdateTileSelectorPosition(TileSelectorMove::Left , pWindow); }

		if		(pEvent->key.code == sf::Keyboard::Delete   ) { m_pCommand->ClearGrid();			}
		else if (pEvent->key.code == sf::Keyboard::Backspace) { m_pCommand->ClearAlgorithmSearch(); }

		if		(pEvent->key.code == sf::Keyboard::Comma ) { m_pCommand->ChangeVisualizationSpeed(VisualSpeed::Decrease); }
		else if (pEvent->key.code == sf::Keyboard::Period) { m_pCommand->ChangeVisualizationSpeed(VisualSpeed::Increase); }

		if (pEvent->key.code == sf::Keyboard::Num1 || pEvent->key.code == sf::Keyboard::Numpad1) { m_pCommand->ExecuteAlgorithm(AlgorithmType::BreadthFirstSearch); }
		if (pEvent->key.code == sf::Keyboard::Num2 || pEvent->key.code == sf::Keyboard::Numpad2) { m_pCommand->ExecuteAlgorithm(AlgorithmType::DepthFirstSearch); }
		if (pEvent->key.code == sf::Keyboard::Num3 || pEvent->key.code == sf::Keyboard::Numpad3) { m_pCommand->ExecuteAlgorithm(AlgorithmType::Dijkstra); }
		if (pEvent->key.code == sf::Keyboard::Num4 || pEvent->key.code == sf::Keyboard::Numpad4) { m_pCommand->ExecuteAlgorithm(AlgorithmType::AStar); }

		break;

	case sf::Event::MouseMoved:

		m_pCommand->UpdateTileSelectorPosition(TileSelectorMove::Mouse, pWindow);
		break;
				
	case sf::Event::MouseButtonPressed:

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))  { m_pCommand->UpdateTileProperty(TileType::WallTile); }
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) { m_pCommand->UpdateTileProperty(TileType::Default ); }

		break;

	default:
		break;
	}
}