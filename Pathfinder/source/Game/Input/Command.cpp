#include "FrameworkPCH.h"
#include "Command.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"

#include "Game/Algorithms/Algorithm.h"
#include "Game/Algorithms/AlgorithmEnum.h"
#include "Game/Objects/UI/Display.h"
#include "Game/Objects/Grid/TileEnum.h"

Command::Command(Scene* pScene)
{
	m_pScene = pScene;
}

Command::~Command()
{
	m_pScene = nullptr;
}

void Command::ResizeGrid(int ZoomValue, unsigned int ScrollSteps)
{
	if (m_pScene->GetGrid()->GetZoomedGridSize().x + ZoomValue >= 8 && 
		m_pScene->GetGrid()->GetZoomedGridSize().x + ZoomValue <= m_pScene->GetGrid()->GetGridSize().x)
	{
		//For Zooming Out
		if (ZoomValue > 0)
		{
			m_ZoomOutSteps += ScrollSteps;

			if (m_ZoomOutSteps >= 10)
			{
				m_pScene->GetGrid()->SetZoomedGridSize(m_pScene->GetGrid()->GetZoomedGridSize().x + ZoomValue);

				m_ZoomOutSteps = 0;
				m_pScene->GetGrid()->ResizeGrid(m_pScene->GetGrid()->GetZoomedGridSize().x, sf::Vector2f(Config::windowWidth, Config::windowHeight), m_pScene->GetDisplay()->GetSize());
			}
		}

		//For Zooming In
		if (ZoomValue < 0)
		{
			m_ZoomInSteps += ScrollSteps;

			if (m_ZoomInSteps >= 10)
			{
				m_pScene->GetGrid()->SetZoomedGridSize(m_pScene->GetGrid()->GetZoomedGridSize().x + ZoomValue);

				m_ZoomInSteps = 0;
				m_pScene->GetGrid()->ResizeGrid(m_pScene->GetGrid()->GetZoomedGridSize().x, sf::Vector2f(Config::windowWidth, Config::windowHeight), m_pScene->GetDisplay()->GetSize());
			}
		}
	}
}

void Command::SetSelectorPosition(sf::RenderWindow* pWindow)
{
	m_pScene->GetGrid()->SetSelectorPosition((sf::Vector2f)sf::Mouse::getPosition(*pWindow));
}

void Command::SetSelectorPosition(sf::RenderWindow* pWindow, Direction direction)
{
	switch (direction)
	{
		case Direction::Up:
			if (m_mouseCoord.y > 0) { m_mouseCoord.y -= 1; }
			break;

		case Direction::Down:
			if (m_mouseCoord.y < (m_pScene->GetGrid()->GetZoomedGridSize().y - 1)) { m_mouseCoord.y += 1; }
			break;

		case Direction::Left:
			if (m_mouseCoord.x > 0) { m_mouseCoord.x -= 1; }
			break;

		case Direction::Right:
			if (m_mouseCoord.x < (m_pScene->GetGrid()->GetZoomedGridSize().x - 1)) { m_mouseCoord.x += 1; }
			break;
	}

	//m_pScene->GetGrid()->SetSelectorPosition(m_mouseCoord);
}

void Command::UpdateTileProperty(TileType tileType)
{
	m_pScene->GetGrid()->SetTileType(m_mouseCoord, tileType);
	HandleOngoingAlgorithm(true);
}

void Command::ExecuteAlgorithm(AlgorithmType algorithmType)
{
	m_AlgorithmStopped = false;

	HandleOngoingAlgorithm(false);
	m_CurrentAlgorithm = algorithmType;
	m_pScene->ExecuteAlgorithm(algorithmType);
}

void Command::ClearAlgorithmSearch()
{
	m_AlgorithmStopped = true;

	HandleOngoingAlgorithm(false);
	m_pScene->GetGrid()->ResetDefaultTiles();
}

void Command::ClearGrid()
{
	m_AlgorithmStopped = true;

	HandleOngoingAlgorithm(false);
	m_pScene->GetGrid()->ClearGrid();
}

void Command::IncreaseVisualSpeed()
{
	VisualSpeed newSpeed = m_pScene->AlgorithmSpeed;

	if      (newSpeed == VisualSpeed::Slow   ) { newSpeed = VisualSpeed::Average; }
	else if (newSpeed == VisualSpeed::Average) { newSpeed = VisualSpeed::Fast;    }
	else if (newSpeed == VisualSpeed::Fast   ) { newSpeed = VisualSpeed::SuperFast; }
	else if (newSpeed == VisualSpeed::SuperFast) { newSpeed = VisualSpeed::Instant; }

	m_pScene->AlgorithmSpeed = newSpeed;
	m_pScene->GetDisplay()->SetSpeed(newSpeed);
}

void Command::DecreaseVisualSpeed()
{
	VisualSpeed newSpeed = m_pScene->AlgorithmSpeed;

	if (newSpeed == VisualSpeed::Average) { newSpeed = VisualSpeed::Slow; }
	else if (newSpeed == VisualSpeed::Fast) { newSpeed = VisualSpeed::Average; }
	else if (newSpeed == VisualSpeed::SuperFast) { newSpeed = VisualSpeed::Fast; }
	else if (newSpeed == VisualSpeed::Instant) { newSpeed = VisualSpeed::SuperFast; }

	m_pScene->AlgorithmSpeed = newSpeed;
	m_pScene->GetDisplay()->SetSpeed(newSpeed);
}

void Command::GenerateRandomGrid(const int wallPercent)
{
	HandleOngoingAlgorithm(false);
	m_pScene->GetGrid()->ClearGrid();
	m_pScene->GetGrid()->GenerateRandomWalls(wallPercent);
}


//Helper Functions
sf::Vector2u Command::GetMouseTileCoord(sf::Vector2i mousePosition, sf::RenderWindow* pWindow)
{
	sf::Vector2f currentTileSizeOnScreen;
	currentTileSizeOnScreen.x = (float)pWindow->getSize().x / (float)m_pScene->GetGrid()->GetZoomedGridSize().x;
	currentTileSizeOnScreen.y = ((float)pWindow->getSize().y - m_pScene->GetDisplay()->GetSize().y) / (float)m_pScene->GetGrid()->GetZoomedGridSize().y;

	float tileCoordX = (float)mousePosition.x / currentTileSizeOnScreen.x;
	float tileCoordY = (((float)mousePosition.y - m_pScene->GetDisplay()->GetSize().y) / currentTileSizeOnScreen.y);


	sf::Vector2u mouseTileCoord = sf::Vector2u((unsigned int)floor(tileCoordX), (unsigned int)floor(tileCoordY));

	return mouseTileCoord;
}

void Command::HandleOngoingAlgorithm(bool bReRunAlgorithm)
{
	m_pScene->StopAlgorithm();

	m_pScene->GetGrid()->ResetDefaultTiles();

	if (bReRunAlgorithm && !m_AlgorithmStopped)
	{
		if (m_pScene->GetAlgorithms()->GetAlgorithmState() != AlgorithmState::Executed)
		{
			m_pScene->ExecuteAlgorithm(m_CurrentAlgorithm);
		}
	}
}
