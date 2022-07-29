#include "FrameworkPCH.h"
#include "Command.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"

#include "Game/Algorithms/Algorithm.h"
#include "Game/Algorithms/AlgorithmEnum.h"
#include "Game/Objects/Grid/Selector.h"
#include "Game/Objects/UI/Display.h"
#include "Game/Objects/Grid/TileType.h"
#include "Game/Objects/Grid/Direction.h"

Command::Command(Scene* pScene)
{
	m_pScene = pScene;
}

Command::~Command()
{
	m_pScene = nullptr;
}

void Command::ResizeGrid(float zoomValue)
{
	m_zoomSteps += zoomValue;

	if(m_zoomSteps > Config::mouseSensitivity)
	{
		m_zoomSteps = 0.0f;

		if (m_pScene->GetGrid()->ColumnZoomLevel + 1 <= m_pScene->GetGrid()->GridSize.x)
		{
			++m_pScene->GetGrid()->ColumnZoomLevel;
			m_pScene->GetGrid()->ResizeGrid();
			m_pScene->GetSelector()->SetSizeAndPosition(Config::windowWidth / m_pScene->GetGrid()->ColumnZoomLevel);
		}
	}
	else if (m_zoomSteps < -Config::mouseSensitivity)
	{
		m_zoomSteps = 0.0f;

		if (m_pScene->GetGrid()->ColumnZoomLevel - 1 > 16)
		{
			--m_pScene->GetGrid()->ColumnZoomLevel;
			m_pScene->GetGrid()->ResizeGrid();
			m_pScene->GetSelector()->SetSizeAndPosition(Config::windowWidth / m_pScene->GetGrid()->ColumnZoomLevel);
		}
	}
}

void Command::SetSelectorPosition(const sf::Vector2f position)
{
	m_pScene->GetSelector()->SetCoordAndPosition(position);
}

void Command::SetSelectorPosition(Direction direction)
{
	m_pScene->GetSelector()->SetCoordAndPosition(direction);
}

void Command::UpdateTileProperty(TileType tileType)
{
	m_pScene->GetGrid()->SetTileType(m_pScene->GetSelector()->GetCoord(), tileType);
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
