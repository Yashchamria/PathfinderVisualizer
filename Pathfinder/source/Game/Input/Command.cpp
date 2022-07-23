#include "FrameworkPCH.h"
#include "Command.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"

#include "Game/Algorithms/Algorithm.h"
#include "Game/Algorithms/AlgorithmEnum.h"

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
				m_pScene->ResizeGrid(m_pScene->GetZoomedGridSize().x, sf::Vector2u(1200, 600), m_pScene->GetTopWidgetSize());
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
				m_pScene->ResizeGrid(m_pScene->GetZoomedGridSize().x, sf::Vector2u(1200, 600), m_pScene->GetTopWidgetSize());
			}
		}
	}
}

void Command::UpdateTileSelectorPosition(TileSelectorMove moveType, sf::RenderWindow* pWindow)
{
	switch (moveType)
	{
	case TileSelectorMove::Mouse:
		m_MouseTileCoord = GetMouseTileCoord(sf::Mouse::getPosition(*pWindow), pWindow);
		break;

	case TileSelectorMove::Up:
		if (m_MouseTileCoord.y > 0) { m_MouseTileCoord.y -= 1; }
		break;

	case TileSelectorMove::Down:
		if (m_MouseTileCoord.y < (m_pScene->GetZoomedGridSize().y - 1)) { m_MouseTileCoord.y += 1; }
		break;	
	
	case TileSelectorMove::Left:
		if (m_MouseTileCoord.x > 0) { m_MouseTileCoord.x -= 1; }
		break;
	
	case TileSelectorMove::Right:
		if (m_MouseTileCoord.x < (m_pScene->GetZoomedGridSize().x - 1)) { m_MouseTileCoord.x += 1; }
		break;
	}

	m_pScene->UpdateTileSelector(m_MouseTileCoord, pWindow);
}

void Command::UpdateTileProperty(TileType tileType)
{
	m_pScene->UpdateTileProperty(m_MouseTileCoord, tileType);
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
	m_pScene->ClearAlgorithmSearch();
}

void Command::ClearGrid()
{
	m_AlgorithmStopped = true;

	HandleOngoingAlgorithm(false);
	m_pScene->ClearGrid();
}

void Command::ChangeVisualizationSpeed(VisualSpeed visualSpeed)
{
	AlgorithmVisualSpeed newSpeed = m_pScene->GetAlgorithmVisualSpeed();

	if (visualSpeed == VisualSpeed::Increase)
	{
		if      (newSpeed == AlgorithmVisualSpeed::Slow   ) { newSpeed = AlgorithmVisualSpeed::Average; }
		else if (newSpeed == AlgorithmVisualSpeed::Average) { newSpeed = AlgorithmVisualSpeed::Fast;    }
		else if (newSpeed == AlgorithmVisualSpeed::Fast   ) { newSpeed = AlgorithmVisualSpeed::SuperFast; }
		else if (newSpeed == AlgorithmVisualSpeed::SuperFast) { newSpeed = AlgorithmVisualSpeed::Instant; }

	}
	else if (visualSpeed == VisualSpeed::Decrease)
	{
		if		(newSpeed == AlgorithmVisualSpeed::Average) { newSpeed = AlgorithmVisualSpeed::Slow;    }
		else if (newSpeed == AlgorithmVisualSpeed::Fast   ) { newSpeed = AlgorithmVisualSpeed::Average; }
		else if (newSpeed == AlgorithmVisualSpeed::SuperFast) { newSpeed = AlgorithmVisualSpeed::Fast;    }
		else if (newSpeed == AlgorithmVisualSpeed::Instant) { newSpeed = AlgorithmVisualSpeed::SuperFast; }
	}

	m_pScene->SetAlgorithmVisualSpeed(newSpeed);
	m_pScene->UpdateTopWidgetLabels(3, VisualSpeedToString(newSpeed));
}

void Command::GenerateRandomGrid(unsigned int wallPercent, unsigned int StartQuadrant, unsigned int EndQuadrant)
{
	HandleOngoingAlgorithm(false);

	m_pScene->GenerateRandomGrid(wallPercent, StartQuadrant, EndQuadrant);
}


//Helper Functions
sf::Vector2u Command::GetMouseTileCoord(sf::Vector2i mousePosition, sf::RenderWindow* pWindow)
{
	sf::Vector2f currentTileSizeOnScreen;
	currentTileSizeOnScreen.x = (float)pWindow->getSize().x / (float)m_pScene->GetZoomedGridSize().x;
	currentTileSizeOnScreen.y = ((float)pWindow->getSize().y - m_pScene->GetTopWidgetSize().y) / (float)m_pScene->GetZoomedGridSize().y;

	float tileCoordX = (float)mousePosition.x / currentTileSizeOnScreen.x;
	float tileCoordY = (((float)mousePosition.y - m_pScene->GetTopWidgetSize().y) / currentTileSizeOnScreen.y);


	sf::Vector2u mouseTileCoord = sf::Vector2u((unsigned int)floor(tileCoordX), (unsigned int)floor(tileCoordY));

	return mouseTileCoord;
}

void Command::HandleOngoingAlgorithm(bool bReRunAlgorithm)
{
	m_pScene->StopAlgorithm();

	m_pScene->ClearAlgorithmSearch();

	if (bReRunAlgorithm && !m_AlgorithmStopped)
	{
		if (m_pScene->GetAlgorithmState() != AlgorithmState::Executed)
		{
				m_pScene->ExecuteAlgorithm(m_CurrentAlgorithm);
		}
	}
}
