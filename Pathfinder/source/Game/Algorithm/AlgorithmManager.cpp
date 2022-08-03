#include "FrameworkPCH.h"
#include "AlgorithmManager.h"

#include <utility>

#include "IAlgorithm.h"
#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/UI/Display.h"


AlgorithmManager::AlgorithmManager(const std::shared_ptr<Grid>& pGrid, const std::shared_ptr<Display>& pDisplay)
	: m_pGrid(pGrid), m_pDisplay(pDisplay)
{
}

void AlgorithmManager::Update(float deltaTime)
{
	if(m_bAnimate)
	{
		m_pDisplay->Log("");
		//if (IsPathFound())
		//{
		//	m_pScene->GetDisplay()->Log("Path Found! Visualizing Path!");
		//}
		//else
		//{
		//	m_pScene->GetDisplay()->Log("Path Not Found! Visualizing Path!");
		//}
		//
		//if (m_pCurrentAlgorithm)
		//{
		//	return m_pCurrentAlgorithm->PlayVisualization(speed, deltaTime);
		//}
		//
		//return true;
	}
}

void AlgorithmManager::Execute(std::shared_ptr<IAlgorithm> pAlgorithm)
{
	//Check for start and end tile
	if (m_pGrid->GetStartTile() == nullptr)
	{
		m_pDisplay->Log("Select Start Tile (Press 'S')");
		return;
	} 
	if (m_pGrid->GetEndTile() == nullptr)
	{
		m_pDisplay->Log("Select End Tile (Press 'E')");
		return;
	}

	m_pDisplay->Log("Executing Algorithm");

	const auto start{ std::chrono::high_resolution_clock::now() };

	m_pCurrentAlgorithm = std::move(pAlgorithm);
	m_bAnimate = true;
	//m_pCurrentAlgorithm.swap(pAlgorithm);

	const auto end{ std::chrono::high_resolution_clock::now() };
	std::cout << "Processing Time: " << (std::chrono::duration<float>)(end - start) << "\n";
}
void AlgorithmManager::Abort()
{
}
/*
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
		pathfinsihed = pTile->Coord == pStartTile->Coord;
	}

	while (!FinalPath.empty())
	{
		m_PendingTileAnimation.push(std::make_pair(FinalPath.top(), TileAnimState::Found));
		m_pathCost += FinalPath.top()->Weight;
		FinalPath.pop();
	}

	m_pathCost += pEndTile->Weight; //Adding cost to go to the end tile.
}


void BaseAlgorithm::AddToTileAnimationArray(Tile* pTile, TileAnimState tileAnimation)
{
	if (pTile->Coord != m_pGrid->GetStartTile()->Coord && pTile->Coord != m_pGrid->GetEndTile()->Coord)
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
*/