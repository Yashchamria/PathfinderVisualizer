#include "FrameworkPCH.h"
#include "Game.h"

#include "Game/Scene.h"
#include "Game/Input/InputManager.h"

Game::Game()
{
	//Creating the Game Window
	sf::Vector2f screenSize = sf::Vector2f(GameConst::WINDOW_WIDTH, GameConst::WINDOW_HEIGHT);
	sf::VideoMode videoMode((unsigned int)screenSize.x, (unsigned int)screenSize.y);
	m_pGameWindow = new sf::RenderWindow(videoMode, GameConst::GAME_WINDOW_NAME, sf::Style::Titlebar);
	m_pGameView = new sf::View(sf::Vector2f(0.0f, 0.0f), screenSize);

	m_pEvent = new sf::Event();
	m_pScene = new Scene(m_pGameWindow);
	m_pInputManager = new InputManager(m_pScene);

	srand((unsigned)time(0));
}

Game::~Game()
{
	delete m_pInputManager;
	delete m_pScene;
	delete m_pEvent;
	delete m_pGameView;
	delete m_pGameWindow;
}

void Game::Init()
{
	PrintGameInfo();
	m_pScene->Initialize();
}

void Game::ProcessGameWindowEvents()
{
	float currentWidth, newHeight; 

	while (m_pGameWindow->pollEvent(*m_pEvent))
	{
		switch (m_pEvent->type)
		{
		case sf::Event::Closed:
			m_pGameWindow->close();
			break;

		case sf::Event::KeyPressed:
			if (m_pEvent->key.code == sf::Keyboard::Escape)
				m_pGameWindow->close();
			break;

		case sf::Event::Resized:
			currentWidth = (float)m_pGameWindow->getSize().x;
			newHeight = currentWidth / GameConst::WINDOW_ASPECT_RATIO;
		
			m_pGameWindow->setSize(sf::Vector2u((unsigned int)currentWidth, (unsigned int)newHeight));
			//m_pGameView->setSize(newHeight * GameConst::WINDOW_ASPECT_RATIO, newHeight);
			break;

		default:
			break;
		}
		m_pInputManager->ProcessInputEvent(m_pEvent, m_pGameWindow);
	}
}

void Game::Update(float deltaTime)
{
	m_pScene->Update(deltaTime);
}

void Game::Draw()
{
	m_pGameWindow->clear(sf::Color::Black);

	m_pGameWindow->setView(*m_pGameView);

	m_pScene->Draw(m_pGameWindow);

	m_pGameWindow->display();
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
	
	std::cout << "Press '1' - To run Dijkstra's Algorithm.\n";
	std::cout << "Press '2' - .\n";
	std::cout << "Press '3' - .\n";
	std::cout << "Press '4' - .\n";
	std::cout << "Press '5' - .\n\n";
	
	std::cout << "Press '<' - To decrease the Visualization Speed.\n";
	std::cout << "Press '>' - To increase the Visualization Speed.\n\n";
	
	std::cout << "Press 'Backspace' - To clear the algorithm search from the grid.\n\n";

	std::cout << "That's all. Have a great day!!";
}

void Game::Run()
{
	Init();

	float timeSinceLastUpdate = 0.0f;
	sf::Clock clock;

	while (m_pGameWindow->isOpen())
	{
		//To get fixed time steps
		timeSinceLastUpdate += clock.restart().asSeconds();

		while (timeSinceLastUpdate > GameConst::TIME_PER_FRAME)
		{
			timeSinceLastUpdate -= GameConst::TIME_PER_FRAME;

			ProcessGameWindowEvents();

			Update(GameConst::TIME_PER_FRAME);
		}

		Draw();
	}
}