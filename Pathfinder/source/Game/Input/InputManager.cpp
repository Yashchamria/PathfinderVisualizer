#include "FrameworkPCH.h"
#include "InputManager.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"

#include "Game/Algorithms/DijkstrasAlgorithm.h"

InputManager::InputManager(Scene* &pScene) : m_pScene(pScene)
{
	m_pDijkstrasAlgorithm = new DijkstrasAlgorithm();
}

InputManager::~InputManager()
{
}

void InputManager::ProcessInputEvent(sf::Event* pEvent, sf::RenderWindow* pWindow)
{
	switch (pEvent->type)
	{
	case sf::Event::MouseWheelMoved:

		//Grid Resizing Event.
		if (pEvent->mouseWheel.delta > 0)	   { ResizeGrid(-4, 5); }
		else if (pEvent->mouseWheel.delta < 0) { ResizeGrid( 4, 5); }

		break;

	case sf::Event::KeyPressed:

		//Grid Resizing Event.
		if (pEvent->key.code == sf::Keyboard::Subtract || pEvent->key.code == sf::Keyboard::Dash)  { ResizeGrid(-4, 10); }
		else if (pEvent->key.code == sf::Keyboard::Add || pEvent->key.code == sf::Keyboard::Equal) { ResizeGrid(4, 10); }
		
		//Set Start Tile.
		if (pEvent->key.code == sf::Keyboard::S)
		{
			m_pScene->UpdateTileProperty(m_MouseTileCoord, TileType::StartTile);
		}
		
		//Set End Tile.
		if (pEvent->key.code == sf::Keyboard::E)
		{
			m_pScene->UpdateTileProperty(m_MouseTileCoord, TileType::EndTile);
		}

		//Set Wall Tile.
		if (pEvent->key.code == sf::Keyboard::W)
		{
			m_pScene->UpdateTileProperty(m_MouseTileCoord, TileType::WallTile);
		}
		
		//Mouse Selector Navigation key bindings.
		if (pEvent->key.code == sf::Keyboard::Down)
		{
			if (m_MouseTileCoord.y < (m_pScene->GetZoomedGridSize().y - 1)) { m_MouseTileCoord.y += 1; }
			m_pScene->UpdateTileSelector(m_MouseTileCoord, pWindow);
		}
		else if (pEvent->key.code == sf::Keyboard::Up)
		{
			if (m_MouseTileCoord.y > 0) { m_MouseTileCoord.y -= 1; }
			m_pScene->UpdateTileSelector(m_MouseTileCoord, pWindow);
		}
		else if (pEvent->key.code == sf::Keyboard::Right)
		{
			if (m_MouseTileCoord.x < (m_pScene->GetZoomedGridSize().x - 1)) { m_MouseTileCoord.x += 1; }
			m_pScene->UpdateTileSelector(m_MouseTileCoord, pWindow);
		}
		else if (pEvent->key.code == sf::Keyboard::Left)
		{
			if (m_MouseTileCoord.x > 0) { m_MouseTileCoord.x -= 1; }
			m_pScene->UpdateTileSelector(m_MouseTileCoord, pWindow);
		}

		//Clear the Grid.
		if (pEvent->key.code == sf::Keyboard::Delete || pEvent->key.code == sf::Keyboard::Backspace)
		{
			m_pScene->ClearGrid();
		}

		//Set the pathfinding algorithm to use.
		if (pEvent->key.code == sf::Keyboard::Num1 || pEvent->key.code == sf::Keyboard::Numpad1)
		{
			m_pScene->SetAlgorithm(m_pDijkstrasAlgorithm);
			m_pScene->ExecuteAlgorithm();
		}
		break;

	case sf::Event::MouseMoved:

		//Mouse Selector Navigation.
		m_MouseTileCoord = GetMouseTileCoord(sf::Mouse::getPosition(*pWindow), pWindow);
		m_pScene->UpdateTileSelector(m_MouseTileCoord, pWindow);
		break;	
				
	default:
		break;
	}
}

void InputManager::ResizeGrid(int ZoomValue, unsigned int ScrollSteps)
{
	if (m_pScene->GetZoomedGridSize().x + ZoomValue >= 8 && 
		m_pScene->GetZoomedGridSize().x + ZoomValue <= m_pScene->GetGridSize().x)
	{

		//For Zooming Out
		if (ZoomValue > 0)
		{
			m_ZoomOutSteps += ScrollSteps;

			if (m_ZoomOutSteps >= 10)
			{
				m_pScene->SetZoomedGridSize(m_pScene->GetZoomedGridSize().x + ZoomValue);

				m_ZoomOutSteps = 0;
				m_pScene->ResizeGrid(m_pScene->GetZoomedGridSize().x);
			}
		}

		//For Zooming In
		if (ZoomValue < 0)
		{
			m_ZoomInSteps += ScrollSteps;

			if (m_ZoomInSteps >= 10)
			{
				m_pScene->SetZoomedGridSize(m_pScene->GetZoomedGridSize().x + ZoomValue);

				m_ZoomInSteps = 0;
				m_pScene->ResizeGrid(m_pScene->GetZoomedGridSize().x);
			}
		}
	}
}

sf::Vector2u InputManager::GetMouseTileCoord(sf::Vector2i mousePosition, sf::RenderWindow* pWindow)
{
	sf::Vector2u mouseTileCoord;

	float currentTileSizeOnScreen = (float)pWindow->getSize().x/(float)m_pScene->GetZoomedGridSize().x;

	float tileCoordX = (float)mousePosition.x / currentTileSizeOnScreen;
	float tileCoordY = (float)mousePosition.y / currentTileSizeOnScreen;


	mouseTileCoord = sf::Vector2u((unsigned int)floor(tileCoordX), (unsigned int)floor(tileCoordY));

	return mouseTileCoord;
}
