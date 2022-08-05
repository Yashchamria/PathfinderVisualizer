#include "FrameworkPCH.h"
#include "AlgorithmManager.h"

#include "AlgorithmData.h"
#include "IAlgorithm.h"

#include "Game/Grid/Grid.h"
#include "Game/Grid/Tile.h"
#include "Game/UI/Display.h"

AlgorithmManager::AlgorithmManager(const std::shared_ptr<Grid>& pGrid, const std::shared_ptr<Display>& pDisplay)
	: m_pGrid(pGrid), m_pDisplay(pDisplay), m_pCurrentData(std::make_shared<AlgorithmData>())
{
}

void AlgorithmManager::Update(float deltaTime)
{
	uint32_t iteration = AnimSpeed;

	while(m_bAnimate && iteration > 0)
	{
		AnimationSequence& sequence = m_pCurrentAlgorithm->GetAnimationSequence();
		sequence.front().first->Animate(sequence.front().second, AnimSpeed);
		sequence.pop();

		if (sequence.empty())
		{
			m_bAnimate = false;
		}
		--iteration;
	}
}

void AlgorithmManager::Execute(std::shared_ptr<IAlgorithm> pAlgorithm)
{
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
	m_pDisplay->SetPreviousData(*m_pCurrentData);

	m_bAnimate = true;
	m_pCurrentAlgorithm = std::move(pAlgorithm);

	// Executing the search algorithm.
	const auto start{std::chrono::high_resolution_clock::now()};
	m_pCurrentData = m_pCurrentAlgorithm->OnExecute(m_pGrid);
	const auto end{std::chrono::high_resolution_clock::now()};

	m_pCurrentData->TimeTaken = std::to_string(std::chrono::duration<float>(end - start).count() * 1000.0f) + "ms";
	m_pDisplay->SetCurrentData(*m_pCurrentData);

	m_pDisplay->Log(m_pCurrentData->PathCost == InvalidData ? "Path not found!" : "Path found!");
}

void AlgorithmManager::ReExecuteIfRequired()
{
	if(!m_bAnimate)
	{
		Abort();
		return;
	}

	Abort();
	if(m_pCurrentAlgorithm)
	{
		Execute(m_pCurrentAlgorithm);
	}
}

void AlgorithmManager::Abort()
{
	m_bAnimate = false;

	if(m_pCurrentAlgorithm)
	{
		m_pCurrentAlgorithm->OnAbort();
	}
	m_pGrid->ResetDefaultTiles();
}