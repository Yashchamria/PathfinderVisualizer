#include "FrameworkPCH.h"
#include "InputManager.h"
#include "Game/Scene.h"

#include "Game/Grid/Grid.h"
#include "Game/Grid/Selector.h"
#include "Game/Grid/TileType.h"
#include "Game/Grid/Direction.h"

#include "Game/Algorithm/AlgorithmManager.h"
#include "Game/Algorithm/BreadthFirstSearch.h"
#include "Game/Algorithm/DepthFirstSearch.h"
#include "Game/Algorithm/DijkstrasAlgorithm.h"
#include "Game/Algorithm/Astar.h"
#include "Game/UI/Display.h"


InputManager::InputManager(const std::shared_ptr<Scene>& pScene):
	m_pGrid(pScene->GetGrid()), m_pSelector(pScene->GetSelector()), m_pDisplay(pScene->GetDisplay()), m_pAlgorithmManager(pScene->GetAlgorithmManager())
{
	m_pDisplay->SetSpeed(Normal);
}

void InputManager::ProcessInputEvent(const std::shared_ptr<sf::Event>& pEvent, const std::shared_ptr<sf::Window>& pWindow) const
{
	switch (pEvent->type)
	{
		case sf::Event::KeyPressed:
		{
			switch (pEvent->key.code)
			{
				case sf::Keyboard::Subtract: case sf::Keyboard::Dash:
					m_pGrid->Zoom(-8);
					m_pSelector->SetSizeAndPosition(Config::windowWidth / (float)m_pGrid->GetZoomLevel());
					break;

				case sf::Keyboard::Add: case sf::Keyboard::Equal:
					m_pGrid->Zoom(8);
					m_pSelector->SetSizeAndPosition(Config::windowWidth / (float)m_pGrid->GetZoomLevel());
					break;

				case sf::Keyboard::S:
					m_pGrid->SetTileType(m_pSelector->GetCoord(), TileType::StartTile);
					m_pAlgorithmManager->ReExecuteIfRequired();
					break;

				case sf::Keyboard::E:
					m_pGrid->SetTileType(m_pSelector->GetCoord(), TileType::EndTile);
					m_pAlgorithmManager->ReExecuteIfRequired();
					break;

				case sf::Keyboard::W:
					m_pGrid->SetTileType(m_pSelector->GetCoord(), TileType::WallTile);
					m_pAlgorithmManager->ReExecuteIfRequired();
					break;

				case sf::Keyboard::D:
					m_pGrid->SetTileType(m_pSelector->GetCoord(), TileType::Default);
					m_pAlgorithmManager->ReExecuteIfRequired();
					break;

				case sf::Keyboard::R:
					m_pGrid->GenerateRandomWalls(20);
					m_pAlgorithmManager->ReExecuteIfRequired();
					break;

				case sf::Keyboard::Up:
					m_pSelector->SetCoordAndPosition(Direction::Up);
					break;

				case sf::Keyboard::Down:
					m_pSelector->SetCoordAndPosition(Direction::Down);
					break;

				case sf::Keyboard::Right:
					m_pSelector->SetCoordAndPosition(Direction::Right);
					break;

				case sf::Keyboard::Left:
					m_pSelector->SetCoordAndPosition(Direction::Left);
					break;

				case sf::Keyboard::Delete:
					m_pAlgorithmManager->Abort();
					m_pGrid->ClearGrid();
					break;

				case sf::Keyboard::BackSpace:
					m_pAlgorithmManager->Abort();
					break;

				case sf::Keyboard::Comma:
					switch (m_pAlgorithmManager->AnimSpeed)
					{
						case Fast: m_pAlgorithmManager->AnimSpeed = Normal; break;
						case Peak: m_pAlgorithmManager->AnimSpeed = Fast; break;
					}
					m_pDisplay->SetSpeed(m_pAlgorithmManager->AnimSpeed);
					break;

				case sf::Keyboard::Period:
					switch (m_pAlgorithmManager->AnimSpeed)
					{
						case Normal: m_pAlgorithmManager->AnimSpeed = Fast; break;
						case Fast:   m_pAlgorithmManager->AnimSpeed = Peak; break;
					}
					m_pDisplay->SetSpeed(m_pAlgorithmManager->AnimSpeed);
					break;

				case sf::Keyboard::Num1: case sf::Keyboard::Numpad1:
					m_pAlgorithmManager->Abort();
					m_pAlgorithmManager->Execute(std::make_shared<BreadthFirstSearch>());
					break;

				case sf::Keyboard::Num2: case sf::Keyboard::Numpad2:
					m_pAlgorithmManager->Abort();
					m_pAlgorithmManager->Execute(std::make_shared<DepthFirstSearch>());
					break;

				case sf::Keyboard::Num3: case sf::Keyboard::Numpad3:
					m_pAlgorithmManager->Abort();
					m_pAlgorithmManager->Execute(std::make_shared<DijkstrasAlgorithm>());
					break;

				case sf::Keyboard::Num4: case sf::Keyboard::Numpad4:
					m_pAlgorithmManager->Abort();
					m_pAlgorithmManager->Execute(std::make_shared<AStar>());
					break;
			}
		}
		break;

		case sf::Event::MouseMoved:
		{
			m_pSelector->SetCoordAndPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*pWindow)));
		}
		break;

		case sf::Event::MouseWheelMoved:
		{
			if (pEvent->mouseWheel.delta > 0)
			{
				m_pGrid->Zoom(-8);
				m_pSelector->SetSizeAndPosition(Config::windowWidth / (float)m_pGrid->GetZoomLevel());
			}
			else if (pEvent->mouseWheel.delta < 0)
			{
				m_pGrid->Zoom(8);
				m_pSelector->SetSizeAndPosition(Config::windowWidth / (float)m_pGrid->GetZoomLevel());
			}
		}
		break;

		case sf::Event::MouseButtonPressed:
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				m_pGrid->SetTileType(m_pSelector->GetCoord(), TileType::WallTile);
				m_pAlgorithmManager->ReExecuteIfRequired();
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				m_pGrid->SetTileType(m_pSelector->GetCoord(), TileType::Default);
				m_pAlgorithmManager->ReExecuteIfRequired();
			}
		}
		break;
	}
}