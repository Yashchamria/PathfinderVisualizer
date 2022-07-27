#include "FrameworkPCH.h"
#include "Scene.h"

#include "GameCore/Objects/GameObject.h"

#include "Game/Objects/UI/Display.h"
#include "Objects/Grid/Grid.h"

#include "Algorithms/Algorithm.h"
#include "Algorithms/AlgorithmEnum.h"

Scene::Scene(const std::shared_ptr<sf::RenderWindow>& pWindow)
{
	m_pDisplay = std::make_shared<Display>(Config::displayHeight, sf::Color(242, 166, 73), pWindow->getSize(), sf::Vector2u(Config::gridColumns, Config::gridRows));

	m_pGrid = std::make_shared<Grid>(sf::Vector2u(Config::gridColumns, Config::gridRows), pWindow->getSize(),
		sf::Vector2f(pWindow->getSize().x, Config::displayHeight));

	m_pGameObjects.push_back(m_pGrid);
	m_pGameObjects.push_back(m_pDisplay);

	m_pAlgorithm = std::make_shared<Algorithms>(m_pGrid.get(), this);

	AlgorithmSpeed = VisualSpeed::Average;
	m_pDisplay->SetSpeed(AlgorithmSpeed);
}

Scene::~Scene()
{
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

	if (m_AlgorithmExecuted)
	{
		m_AlgorithmExecuted = !m_pAlgorithm->PlayVisualization((float)AlgorithmSpeed, deltaTime);

		if (m_pAlgorithm->GetAlgorithmState() == AlgorithmState::Visualized)
		{
			m_pDisplay->Log("Finished Visualization!!");
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
		m_pDisplay->SetPreviousData({ m_pAlgorithm->GetAlgorithmName(), m_pAlgorithm->GetTimeTaken(),
		m_pAlgorithm->GetTotalCost(), m_pAlgorithm->GetTilesExplored() });
	}

	m_AlgorithmExecuted = m_pAlgorithm->Execute(algorithmType);

	m_pDisplay->SetCurrentData({ m_pAlgorithm->GetAlgorithmName(), m_pAlgorithm->GetTimeTaken(),
	m_pAlgorithm->GetTotalCost(), m_pAlgorithm->GetTilesExplored() });
}

void Scene::StopAlgorithm()
{
	m_AlgorithmExecuted = false;
	m_pAlgorithm->Stop();
}