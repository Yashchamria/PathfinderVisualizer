#include "FrameworkPCH.h"
#include "Scene.h"

#include "GameCore/Objects/GameObject.h"

#include "Game/Objects/UI/TopHUDWidget.h"
#include "Objects/Grid/Grid.h"

#include "Algorithms/Algorithm.h"
#include "Algorithms/AlgorithmEnum.h"

Scene::Scene(const std::shared_ptr<sf::RenderWindow>& pWindow)
{
	m_pDisplay = std::make_shared<TopHUDWidget>(sf::Vector2f(GameConst::TOP_WIDGET_WIDTH,GameConst::TOP_WIDGET_HEIGHT), sf::Color(242, 166, 73), pWindow);
	m_pGameObjects.push_back(m_pDisplay);

	m_pGrid = std::make_shared<Grid>(sf::Vector2u(GameConst::GRID_COLUMNS, GameConst::GRID_ROWS), pWindow->getSize(), m_pDisplay->GetWidgetBoxSize());
	m_pGameObjects.push_back(m_pGrid);

	m_pAlgorithm = std::make_shared<Algorithms>(m_pGrid.get(), this);

	m_pCurrentAlgorithmData = new AlgorithmData();
	m_pPreviousAlgorithmData = new AlgorithmData();

	m_pCurrentAlgorithmData->SetName(m_pAlgorithm->GetAlgorithmName());

	AlgorithmSpeed = AlgorithmVisualSpeed::Average;
	m_pDisplay->UpdateLabel(3, VisualSpeedToString(AlgorithmSpeed));
}

Scene::~Scene()
{
	delete m_pCurrentAlgorithmData;
	delete m_pPreviousAlgorithmData;

	m_pGameObjects.clear();
}

void Scene::Initialize() const
{
	for (const auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Initialize();
	}
}

void Scene::Update(float deltaTime)
{
	for (const auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Update(deltaTime);
	}

	AutoUpdateTopWidget();

	if (m_AlgorithmExecuted)
	{
		m_AlgorithmExecuted = !(m_pAlgorithm->PlayVisualization((float)AlgorithmSpeed, deltaTime));

		if (m_pAlgorithm->GetAlgorithmState() == AlgorithmState::Visualized)
		{
			m_pDisplay->UpdateLabel(6, "Finished Visualization!!");
		}
	}

}

void Scene::Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) const
{
	for (const auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Draw(renderWindow);
	}
}

void Scene::ExecuteAlgorithm(AlgorithmType algorithmType)
{
	if (m_pAlgorithm->GetAlgorithmState() != AlgorithmState::UnExecuted)
	{
		m_pPreviousAlgorithmData->SetName(m_pAlgorithm->GetAlgorithmName());
		m_pPreviousAlgorithmData->SetTimeTaken(m_pAlgorithm->GetTimeTaken());
		m_pPreviousAlgorithmData->SetTilesExplored(m_pAlgorithm->GetTilesExplored());
		m_pPreviousAlgorithmData->SetPathCost(m_pAlgorithm->GetTotalCost());
	}

	m_AlgorithmExecuted = m_pAlgorithm->Execute(algorithmType);

	m_pCurrentAlgorithmData->SetName(m_pAlgorithm->GetAlgorithmName());
	m_pCurrentAlgorithmData->SetTimeTaken(m_pAlgorithm->GetTimeTaken());
	m_pCurrentAlgorithmData->SetTilesExplored(m_pAlgorithm->GetTilesExplored());
	m_pCurrentAlgorithmData->SetPathCost(m_pAlgorithm->GetTotalCost());
}

void Scene::StopAlgorithm()
{
	m_AlgorithmExecuted = false;
	m_pAlgorithm->Stop();
}

void Scene::AutoUpdateTopWidget()
{
	m_pDisplay->UpdateLabel(1, m_pCurrentAlgorithmData->GetName());
	m_pDisplay->UpdateLabel(2, m_pCurrentAlgorithmData->GetTimeTaken());
	m_pDisplay->UpdateLabel(4, m_pCurrentAlgorithmData->GetPathCost());
	m_pDisplay->UpdateLabel(5, m_pCurrentAlgorithmData->GetTilesExplored());

	m_pDisplay->UpdateLabel(7, m_pPreviousAlgorithmData->GetName());
	m_pDisplay->UpdateLabel(8, m_pPreviousAlgorithmData->GetTimeTaken());
	m_pDisplay->UpdateLabel(9, m_pPreviousAlgorithmData->GetPathCost());
}