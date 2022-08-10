#include "FrameworkPCH.h"
#include "Game.h"

#include "Game/Scene.h"
#include "Game/Input/InputManager.h"

Game::Game(const std::string& windowName, const sf::Vector2u windowSize)
{
	m_pRenderWindow = std::make_shared<sf::RenderWindow>
	(sf::VideoMode(windowSize.x, windowSize.y), windowName, sf::Style::Titlebar);
	m_pRenderWindow->setFramerateLimit(Config::fps);
	m_pView = std::make_shared<sf::View>(sf::Vector2f(0.0f, 0.0f), (sf::Vector2f)windowSize);

	m_pScene = std::make_shared<Scene>(m_pRenderWindow);
	m_pInputManager = std::make_shared<InputManager>(m_pScene);

	srand(static_cast<unsigned>(time(nullptr)));
}

void Game::ProcessGameWindowEvents(const std::shared_ptr<sf::Event>& pEvent) const
{
	while (m_pRenderWindow->pollEvent(*pEvent))
	{
		switch (pEvent->type)
		{
		case sf::Event::Closed:
			m_pRenderWindow->close();
			break;

		case sf::Event::KeyPressed:
			if (pEvent->key.code == sf::Keyboard::Escape)
				m_pRenderWindow->close();
			break;

		default:
			break;
		}
		m_pInputManager->ProcessInputEvent(pEvent, m_pRenderWindow);
	}
}

void Game::Run() const
{
	PrintGameInfo();
	m_pScene->Initialize();

	sf::Clock clock {};
	float timeSinceLastUpdate = 0.0f;
	const auto event = std::make_shared<sf::Event>();

	while (m_pRenderWindow->isOpen())
	{
		ProcessGameWindowEvents(event);
		m_pScene->Update(1.0f / Config::fps);
		Draw();
	}
}

void Game::Draw() const
{
	m_pRenderWindow->clear(sf::Color::Black);
	m_pRenderWindow->setView(*m_pView);
	m_pScene->Draw(m_pRenderWindow);
	m_pRenderWindow->display();
}

void Game::PrintGameInfo()
{
	std::cout << "****************** Grid Navigation ******************\n\n";

	std::cout << "Mouse/Arrow keys: Moves the tile selector.\n";
	std::cout << "Mouse scroll:     Zoom in or out.\n";

	std::cout << "\n****************** Tile Creation ******************\n\n";

	std::cout << "S:      Creates start tile.\n";
	std::cout << "E:      Creates end tile.\n";
	std::cout << "W:      Creates wall tile.\n";
	std::cout << "D:      Creates default tile.\n";
	std::cout << "+/-:    Adjust the tile weight.\n";
	std::cout << "R:      Generates random grid layout.\n";
	std::cout << "Delete: Clears the grid.\n";

	std::cout << "\n****************** Algorithm Visualization ******************\n\n";

	
	std::cout << "1:         Runs Breadth First Search\n";
	std::cout << "2:         Runs Depth First Search\n";
	std::cout << "3:         Runs Dijkstra's Algorithm\n";
	std::cout << "4:         Runs AStar(*) Algorithm\n";
	
	std::cout << "</>:       Adjust the visualization speed.\n";
	std::cout << "Backspace: Clears the algorithm search.\n";

	std::cout << "\n******************************************************\n";
}