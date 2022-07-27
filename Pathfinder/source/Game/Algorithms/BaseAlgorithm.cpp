#include "FrameworkPCH.h"
#include "BaseAlgorithm.h"

#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileEnum.h"
#include "AlgorithmEnum.h"

BaseAlgorithm::BaseAlgorithm(Grid* pGrid)
{
	m_pGrid = pGrid;
}

BaseAlgorithm::~BaseAlgorithm()
{
	m_pGrid = nullptr;
}

bool BaseAlgorithm::Execute(AlgorithmType algorithmType)
{
	auto algorithmStart = std::chrono::steady_clock::now(); //To calculate the time algorithm took.

	Init();

	OnExecute();//Main loop

	auto algorithmEnd = std::chrono::steady_clock::now();
	auto algorithmDuration = algorithmEnd - algorithmStart;
	m_algorithmDuration = (std::to_string)(std::chrono::duration <double, std::milli>(algorithmDuration).count());
	m_algorithmDuration = m_algorithmDuration.substr(0, 6) + " ms";

	m_AlgorithmState = AlgorithmState::Visualizing;
	if (IsPathFound()) { GetFinalPathAnimationSequence(); }

	return true;
}

void BaseAlgorithm::Stop()
{
	m_pathfound = false;

	m_stopExecution = false;
	m_switchSpeed = 3.0f;

	std::queue<std::pair<Tile*, TileAnimState>> empty;
	std::swap(m_PendingTileAnimation, empty);

	m_ClosestPreviousTile.clear();

	OnStop();
}

void BaseAlgorithm::Init()
{
	m_tilesExplored = 0;
	m_pathCost = 0;
	m_AlgorithmState = AlgorithmState::Executing;

	OnInit();
}

bool BaseAlgorithm::PlayVisualization(int speed, float deltaTime)
{
	if (speed == (int)VisualSpeed::Instant)
	{
		while (!m_PendingTileAnimation.empty())
		{
			m_PendingTileAnimation.front().first->SetTileAnimationProperty(m_PendingTileAnimation.front().second);
			m_PendingTileAnimation.front().first->UpdateTileAnimationProperty();
			m_PendingTileAnimation.pop();
		}
	}

	if (m_PendingTileAnimation.empty())
	{
		m_AlgorithmState = AlgorithmState::Visualized;
		return true;
	}

	m_PendingTileAnimation.front().first->SetTileAnimationProperty(m_PendingTileAnimation.front().second);
	m_PendingTileAnimation.front().first->UpdateTileAnimationProperty();

	m_switchSpeed += (deltaTime * speed);

	if (m_switchSpeed >= 1.0f)
	{
		m_PendingTileAnimation.pop();
		m_switchSpeed = 0.0f;
	}

	return false;
}

void BaseAlgorithm::GetFinalPathAnimationSequence()
{
	std::stack<Tile*> FinalPath;

	Tile* pEndTile = m_pGrid->GetEndTile().get();
	Tile* pStartTile = m_pGrid->GetStartTile().get();

	bool pathfinsihed = false;
	Tile* pTile = m_ClosestPreviousTile[pEndTile];

	while (!pathfinsihed)
	{
		FinalPath.push(pTile);
		pTile = m_ClosestPreviousTile[FinalPath.top()];
		pathfinsihed = pTile->GetCoord() == pStartTile->GetCoord();
	}

	while (!FinalPath.empty())
	{
		m_PendingTileAnimation.push(std::make_pair(FinalPath.top(), TileAnimState::Found));
		m_pathCost += FinalPath.top()->GetWeight();
		FinalPath.pop();
	}

	m_pathCost += pEndTile->GetWeight(); //Adding cost to go to the end tile.
}

void BaseAlgorithm::AddToTileAnimationArray(Tile* pTile, TileAnimState tileAnimation)
{
	if (pTile->GetCoord() != m_pGrid->GetStartTile()->GetCoord() && pTile->GetCoord() != m_pGrid->GetEndTile()->GetCoord())
	{
		m_PendingTileAnimation.push(std::make_pair(pTile, tileAnimation)); //For Visualization
	}
}

void BaseAlgorithm::AddToClosestPreviousTile(Tile* pTile, Tile* pPreviousTile)
{
	if (pTile && pPreviousTile)
	{
		if (m_ClosestPreviousTile.find(pTile) == m_ClosestPreviousTile.end())
		{
			m_ClosestPreviousTile.insert_or_assign(pTile, pPreviousTile);
		}
	}
}
