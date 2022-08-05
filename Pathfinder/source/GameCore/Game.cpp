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
	std::cout << "Welcome to Pathfinder Visualizer!!\n\n";

	std::cout << "Let's go over the simple shortcuts to navigate through the application.\n\n";

	std::cout << "Use 'Mouse pointer' - To change the Tile Selector location.\n";
	std::cout << "Or, Use 'Arrow Keys' - To change the Tile Selector location.\n\n";

	std::cout << "Use 'Scroll Wheel' - To Zoom In or Zoom Out the Grid.\n";
	std::cout << "Or, Press '+' - To Zoom Out.\n";
	std::cout << "And Press '-' - To Zoom In.\n\n";
	
	std::cout << "Press 'S' - To set the Start Tile.\n";
	std::cout << "Press 'E' - To set the End Tile.\n";
	std::cout << "Press 'W' - To create the Wall Tile.\n";
	std::cout << "Press 'D' - To reset the tile to Default Tile.\n\n";
	
	std::cout << "Press 'Delete' - To clear the entire grid.\n\n";
	
	std::cout << "Press '1' - To run Breadth First Search.\n";
	std::cout << "Press '2' - To run Depth First Search.\n";
	std::cout << "Press '3' - To run Dijkstra's Algorithm.\n";
	std::cout << "Press '4' - To run Astar(*) Algorithm.\n";
	std::cout << "Press 'R' - To Generate Random Start, End and Wall Tiles.\n\n";
	
	std::cout << "Press '<' - To decrease the Visualization Speed.\n";
	std::cout << "Press '>' - To increase the Visualization Speed.\n\n";
	
	std::cout << "Press 'Backspace' - To clear the algorithm search from the grid.\n\n";

	std::cout << "That's all. Have a great day!!";
}