#include "FrameworkPCH.h"
#include "InputManager.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"

InputManager::InputManager(Scene* &pScene) : m_pScene(pScene)
{
}

InputManager::~InputManager()
{
}

void InputManager::ProcessInputEvent(sf::Event* pEvent, sf::RenderWindow* pWindow)
{
	switch (pEvent->type)
	{
	case sf::Event::MouseWheelMoved:

		if (pEvent->mouseWheel.delta > 0)	   { ResizeGrid(-4, 5); }
		else if (pEvent->mouseWheel.delta < 0) { ResizeGrid( 4, 5); }

		break;

	default:
		break;
	}
}

void InputManager::ResizeGrid(int ZoomValue, unsigned int ScrollSteps)
{
	if (m_pScene->GetGridZoomOnScreen() + ZoomValue >= 8 && 
		m_pScene->GetGridZoomOnScreen() + ZoomValue <= m_pScene->GetGridSize().x)
	{
		m_pScene->SetGridZoomOnScreen(m_pScene->GetGridZoomOnScreen() + ZoomValue);

		//For Zooming Out
		if (ZoomValue > 0)
		{
			m_ZoomOutSteps += ScrollSteps;

			if (m_ZoomOutSteps >= 10)
			{
				m_ZoomOutSteps = 0;
				m_pScene->ResizeGrid(m_pScene->GetGridZoomOnScreen());
			}
		}

		//For Zooming In
		if (ZoomValue < 0)
		{
			m_ZoomInSteps += ScrollSteps;

			if (m_ZoomInSteps >= 10)
			{
				m_ZoomInSteps = 0;
				m_pScene->ResizeGrid(m_pScene->GetGridZoomOnScreen());
			}
		}
	}
}
