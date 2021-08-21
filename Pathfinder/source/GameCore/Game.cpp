#include "FrameworkPCH.h"
#include "Game.h"

#include "Game/Scene.h"
#include "Game/Input/InputManager.h"

Game::Game()
{
	//Creating the Game Window
	sf::Vector2f screenSize = sf::Vector2f(GameConst::WINDOW_WIDTH, GameConst::WINDOW_HEIGHT);
	sf::VideoMode videoMode((unsigned int)screenSize.x, (unsigned int)screenSize.y);
	m_pGameWindow = new sf::RenderWindow(videoMode, GameConst::GAME_WINDOW_NAME, sf::Style::Default);
	m_pGameView = new sf::View(sf::Vector2f(0.0f, 0.0f), screenSize);

	m_pEvent = new sf::Event();
	m_pScene = new Scene(m_pGameWindow);
	m_pInputManager = new InputManager(m_pScene);
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