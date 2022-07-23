#include "FrameworkPCH.h"
#include "Command.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"

#include "Game/Algorithms/Algorithm.h"
#include "Game/Algorithms/AlgorithmEnum.h"
#include "Game/Objects/UI/TopHUDWidget.h"

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
				m_pScene->GetGrid()->ResizeGrid(m_pScene->GetGrid()->GetZoomedGridSize().x, sf::Vector2u(1200, 600), m_pScene->GetDisplay()->GetWidgetBoxSize());
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
				m_pScene->GetGrid()->ResizeGrid(m_pScene->GetGrid()->GetZoomedGridSize().x, sf::Vector2u(1200, 600), m_pScene->GetDisplay()->GetWidgetBoxSize());
			}
		}
	}
}

void Command::UpdateTileSelectorPosition(TileSelectorMove moveType, sf::RenderWindow* pWindow, const sf::Vector2f displaySize)
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
		if (m_MouseTileCoord.y < (m_pScene->GetGrid()->GetZoomedGridSize().y - 1)) { m_MouseTileCoord.y += 1; }
		break;	
	
	case TileSelectorMove::Left:
		if (m_MouseTileCoord.x > 0) { m_MouseTileCoord.x -= 1; }
		break;
	
	case TileSelectorMove::Right:
		if (m_MouseTileCoord.x < (m_pScene->GetGrid()->GetZoomedGridSize().x - 1)) { m_MouseTileCoord.x += 1; }
		break;
	}

	m_pScene->GetGrid()->UpdateTileSelector(m_MouseTileCoord, pWindow->getSize(), displaySize);
}

void Command::UpdateTileProperty(TileType tileType)
{
	m_pScene->GetGrid()->UpdateTileProperty(m_MouseTileCoord, tileType);
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
	m_pScene->GetGrid()->ClearAlgorithmSearch();
}

void Command::ClearGrid()
{
	m_AlgorithmStopped = true;

	HandleOngoingAlgorithm(false);
	m_pScene->GetGrid()->ClearGrid();
}

void Command::ChangeVisualizationSpeed(VisualSpeed visualSpeed)
{
	AlgorithmVisualSpeed newSpeed = m_pScene->AlgorithmSpeed;

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

	m_pScene->AlgorithmSpeed = newSpeed;
	m_pScene->GetDisplay()->UpdateLabel(3, VisualSpeedToString(newSpeed));
}

void Command::GenerateRandomGrid(unsigned int wallPercent, unsigned int StartQuadrant, unsigned int EndQuadrant)
{
	HandleOngoingAlgorithm(false);

	m_pScene->GetGrid()->GenerateRandomGrid(wallPercent, StartQuadrant, EndQuadrant);
}


//Helper Functions
sf::Vector2u Command::GetMouseTileCoord(sf::Vector2i mousePosition, sf::RenderWindow* pWindow)
{
	sf::Vector2f currentTileSizeOnScreen;
	currentTileSizeOnScreen.x = (float)pWindow->getSize().x / (float)m_pScene->GetGrid()->GetZoomedGridSize().x;
	currentTileSizeOnScreen.y = ((float)pWindow->getSize().y - m_pScene->GetDisplay()->GetWidgetBoxSize().y) / (float)m_pScene->GetGrid()->GetZoomedGridSize().y;

	float tileCoordX = (float)mousePosition.x / currentTileSizeOnScreen.x;
	float tileCoordY = (((float)mousePosition.y - m_pScene->GetDisplay()->GetWidgetBoxSize().y) / currentTileSizeOnScreen.y);


	sf::Vector2u mouseTileCoord = sf::Vector2u((unsigned int)floor(tileCoordX), (unsigned int)floor(tileCoordY));

	return mouseTileCoord;
}

void Command::HandleOngoingAlgorithm(bool bReRunAlgorithm)
{
	m_pScene->StopAlgorithm();

	m_pScene->GetGrid()->ClearAlgorithmSearch();

	if (bReRunAlgorithm && !m_AlgorithmStopped)
	{
		if (m_pScene->GetAlgorithms()->GetAlgorithmState() != AlgorithmState::Executed)
		{
			m_pScene->ExecuteAlgorithm(m_CurrentAlgorithm);
		}
	}
}
