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

void Command::ResizeGrid(int ZoomValue, unsigned int ScrollSteps)
{
	const auto& pGrid = m_pScene->GetGrid();

	if (pGrid->GetZoomedGridSize().x + ZoomValue >= 8 && pGrid->GetZoomedGridSize().x + ZoomValue <= pGrid->GetGridSize().x)
	{
		//For Zooming Out
		if (ZoomValue > 0)
		{
			m_ZoomOutSteps += ScrollSteps;

			if (m_ZoomOutSteps >= 10)
			{
				pGrid->SetZoomedGridSize(pGrid->GetZoomedGridSize().x + ZoomValue);

				m_ZoomOutSteps = 0;
				pGrid->ResizeGrid(pGrid->GetZoomedGridSize().x, sf::Vector2f(Config::windowWidth, Config::windowHeight), m_pScene->GetDisplay()->GetSize());

				m_pScene->GetSelector()->SetSizeAndPosition(Config::windowWidth / pGrid->GetZoomedGridSize().x);
			}
		}

		//For Zooming In
		if (ZoomValue < 0)
		{
			m_ZoomInSteps += ScrollSteps;

			if (m_ZoomInSteps >= 10)
			{
				pGrid->SetZoomedGridSize(pGrid->GetZoomedGridSize().x + ZoomValue);

				m_ZoomInSteps = 0;
				pGrid->ResizeGrid(pGrid->GetZoomedGridSize().x, sf::Vector2f(Config::windowWidth, Config::windowHeight), m_pScene->GetDisplay()->GetSize());
				m_pScene->GetSelector()->SetSizeAndPosition(Config::windowWidth / pGrid->GetZoomedGridSize().x);
			}
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
