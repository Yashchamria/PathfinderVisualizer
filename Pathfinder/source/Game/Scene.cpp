#include "FrameworkPCH.h"
#include "Scene.h"

#include "GameObject/GameObject.h"

#include "Grid/Grid.h"
#include "Grid/Selector.h"

#include "UI/Display.h"
#include "Algorithm/AlgorithmManager.h"

Scene::Scene(const std::shared_ptr<sf::RenderWindow>& pWindow)
{
	// Loading the font.
	#if PF_DEBUG
	if (!m_font.loadFromFile("../Pathfinder/Data/font/Pixellari.ttf")) { std::cout << "Font not found!\n"; }
	#elif PF_RELEASE
	if (!m_font.loadFromFile("font/Pixellari.ttf")) { std::cout << "Font not found!\n"; }
	#endif

	m_pDisplay = std::make_shared<Display>(Config::displayHeight, DISPLAY_COLOR, pWindow->getSize(), sf::Vector2u(Config::gridColumns, Config::gridRows), m_font);

	m_pGrid = std::make_shared<Grid>(sf::Vector2u(Config::gridColumns, Config::gridRows), (sf::Vector2f)pWindow->getSize(),
		sf::Vector2f((float)pWindow->getSize().x, Config::displayHeight), m_font);

	const float selectorSize = pWindow->getSize().x / (float)Config::gridColumns;
	m_pSelector = std::make_shared<Selector>(sf::Vector2u(0, 0), selectorSize, sf::Color::Transparent,
		selectorSize * Config::gridOutlineStrength * 2.0f, SELECTOR_COLOR);

	m_pAlgorithmManager = std::make_shared<AlgorithmManager>(m_pGrid, m_pDisplay);

	m_pGameObjects.push_back(m_pGrid);
	m_pGameObjects.push_back(m_pDisplay);
	m_pGameObjects.push_back(m_pSelector);
	m_pGameObjects.push_back(m_pAlgorithmManager);
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

void Scene::Update(float deltaTime) const
{
	for (const auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Update(deltaTime);
	}
}

void Scene::Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) const
{
	for (const auto& pGameObject : m_pGameObjects)
	{
		pGameObject->Draw(renderWindow);
	}
}