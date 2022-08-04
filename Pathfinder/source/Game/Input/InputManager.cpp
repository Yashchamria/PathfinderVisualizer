#include "FrameworkPCH.h"
#include "InputManager.h"
#include "Game/Scene.h"

#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Selector.h"
#include "Game/Objects/Grid/TileType.h"
#include "Game/Objects/Grid/Direction.h"

#include "Game/Algorithm/AlgorithmManager.h"
#include "Game/Algorithm/BreadthFirstSearch.h"
#include "Game/Algorithm/DepthFirstSearch.h"
#include "Game/Algorithm/DijkstrasAlgorithm.h"
#include "Game/Algorithm/Astar.h"


InputManager::InputManager(const std::shared_ptr<Scene>& pScene): m_pScene(pScene)
{
}

void InputManager::ProcessInputEvent(const std::shared_ptr<sf::Event>& pEvent, const std::shared_ptr<sf::Window>& pWindow)
{
	switch (pEvent->type)
	{
		case sf::Event::KeyPressed:
		{
			switch (pEvent->key.code)
			{
				case sf::Keyboard::Subtract: case sf::Keyboard::Dash:
					m_pScene->GetGrid()->Zoom(-8);
					break;

				case sf::Keyboard::Add: case sf::Keyboard::Equal:
					m_pScene->GetGrid()->Zoom(8);
					break;

				case sf::Keyboard::S:
					m_pScene->GetGrid()->SetTileType(m_pScene->GetSelector()->GetCoord(), TileType::StartTile);
					m_pScene->GetAlgorithmManager()->ReExecuteIfRequired();
					break;

				case sf::Keyboard::E:
					m_pScene->GetGrid()->SetTileType(m_pScene->GetSelector()->GetCoord(), TileType::EndTile);
					m_pScene->GetAlgorithmManager()->ReExecuteIfRequired();
					break;

				case sf::Keyboard::W:
					m_pScene->GetGrid()->SetTileType(m_pScene->GetSelector()->GetCoord(), TileType::WallTile);
					m_pScene->GetAlgorithmManager()->ReExecuteIfRequired();
					break;

				case sf::Keyboard::D:
					m_pScene->GetGrid()->SetTileType(m_pScene->GetSelector()->GetCoord(), TileType::Default);
					m_pScene->GetAlgorithmManager()->ReExecuteIfRequired();
					break;

				case sf::Keyboard::R:
					m_pScene->GetGrid()->GenerateRandomWalls(20);
					m_pScene->GetAlgorithmManager()->ReExecuteIfRequired();
					break;

				case sf::Keyboard::Up:
					m_pScene->GetSelector()->SetCoordAndPosition(Direction::Up);
					break;

				case sf::Keyboard::Down:
					m_pScene->GetSelector()->SetCoordAndPosition(Direction::Down);
					break;

				case sf::Keyboard::Right:
					m_pScene->GetSelector()->SetCoordAndPosition(Direction::Right);
					break;

				case sf::Keyboard::Left:
					m_pScene->GetSelector()->SetCoordAndPosition(Direction::Left);
					break;

				case sf::Keyboard::Delete:
					m_pScene->GetAlgorithmManager()->Abort();
					m_pScene->GetGrid()->ClearGrid();
					break;

				case sf::Keyboard::BackSpace:
					m_pScene->GetAlgorithmManager()->Abort();
					break;

				case sf::Keyboard::Comma:
					// TODO - Decrement visualization speed.
					break;

				case sf::Keyboard::Period:
					// TODO - Increment visualization speed.
					break;

				case sf::Keyboard::Num1: case sf::Keyboard::Numpad1:
					m_pScene->GetAlgorithmManager()->Abort();
					m_pScene->GetAlgorithmManager()->Execute(std::make_shared<BreadthFirstSearch>());
					break;

				case sf::Keyboard::Num2: case sf::Keyboard::Numpad2:
					m_pScene->GetAlgorithmManager()->Abort();
					m_pScene->GetAlgorithmManager()->Execute(std::make_shared<DepthFirstSearch>());
					break;

				case sf::Keyboard::Num3: case sf::Keyboard::Numpad3:
					m_pScene->GetAlgorithmManager()->Abort();
					m_pScene->GetAlgorithmManager()->Execute(std::make_shared<DijkstrasAlgorithm>());
					break;

				case sf::Keyboard::Num4: case sf::Keyboard::Numpad4:
					m_pScene->GetAlgorithmManager()->Abort();
					m_pScene->GetAlgorithmManager()->Execute(std::make_shared<AStar>());
					break;
			}
		}
		break;

		case sf::Event::MouseMoved:
		{
			m_pScene->GetSelector()->SetCoordAndPosition((sf::Vector2f)sf::Mouse::getPosition(*pWindow));
		}
		break;

		case sf::Event::MouseWheelMoved:
		{
			if (pEvent->mouseWheel.delta > 0)
			{
				m_pScene->GetGrid()->Zoom(-8);
			}
			else if (pEvent->mouseWheel.delta < 0)
			{
				m_pScene->GetGrid()->Zoom(8);
			}
		}
		break;

		case sf::Event::MouseButtonPressed:
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				m_pScene->GetGrid()->SetTileType(m_pScene->GetSelector()->GetCoord(), TileType::WallTile);
				m_pScene->GetAlgorithmManager()->ReExecuteIfRequired();
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				m_pScene->GetGrid()->SetTileType(m_pScene->GetSelector()->GetCoord(), TileType::Default);
				m_pScene->GetAlgorithmManager()->ReExecuteIfRequired();
			}
		}
		break;
	}
}