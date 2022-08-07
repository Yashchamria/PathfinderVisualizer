#include "FrameworkPCH.h"
#include "Display.h"
#include "Game/Algorithm/AlgorithmData.h"

Display::Display(const float height, const sf::Color color, const sf::Vector2u windowSize, const sf::Vector2u gridSize, const sf::Font& font) :
	m_pCanvas(std::make_unique<sf::RectangleShape>(sf::Vector2f((float)windowSize.x, height)))
{
	// Setting up the widget box.
	m_pCanvas->setFillColor(DISPLAY_COLOR);
	m_pCanvas->setOutlineColor(GRID_COLOR);
	m_pCanvas->setOutlineThickness(-height / 30.0f);
	const sf::Vector2f canvasPos{ -(float)windowSize.x / 2.0f, -(float)windowSize.y / 2.0f };
	m_pCanvas->setPosition(canvasPos);

	// Setting the grid size label.
	const std::string gridSizeText = std::to_string(gridSize.x) + " X " + std::to_string(gridSize.y);
	m_pLabels[0] = std::make_unique<sf::Text>(gridSizeText, font, m_fontSize + m_fontSize / 2);
	m_pLabels[0]->setFillColor(sf::Color(242, 166, 73));
	m_pLabels[0]->setOutlineThickness(3.0f);
	m_pLabels[0]->setOutlineColor(sf::Color::Black);
	m_pLabels[0]->setPosition(sf::Vector2f{ windowSize.x - 260.0f, windowSize.y - 120.0f } / 2.0f);

	// Positioning the data labels.
	const sf::Vector2f initialLabelPosition{ m_pCanvas->getPosition() + sf::Vector2f(60.0f, 20.0f) };
	sf::Vector2f labelPosition{ initialLabelPosition };

	for (int i = 1; i < m_totalLabels; i++)
	{
		m_pLabels[i] = std::make_unique<sf::Text>(m_labelText[i], font, m_fontSize);
		m_pLabels[i]->setFillColor(sf::Color::Black);
		m_pLabels[i]->setPosition(labelPosition);

		if (i % 4 == 0) { labelPosition.y += 36.0f; labelPosition.x = initialLabelPosition.x; }
		else { labelPosition.x += 285.0f; }
	}
}

void Display::Log(const std::string& message) const
{
	m_pLabels[10]->setString(m_labelText[10] + message);
}

void Display::SetSpeed(const VisualSpeed speed) const
{
	std::string speedText {};

	switch (speed)
	{
		case Normal:	speedText = "Normal"; break;
		case Fast:		speedText = "Fast"; break;
		case Peak:	speedText = "Peak"; break;
	}
	m_pLabels[9]->setString(m_labelText[9] + speedText);
}

void Display::SetCurrentData(const AlgorithmData& currentData) const
{
	m_pLabels[1]->setString(m_labelText[1] + currentData.Name);
	m_pLabels[2]->setString(m_labelText[2] + currentData.PathCost);
	m_pLabels[3]->setString(m_labelText[3] + currentData.TilesExplored);
	m_pLabels[4]->setString(m_labelText[4] + currentData.TimeTaken);
}

void Display::SetPreviousData(const AlgorithmData& previousData) const
{
	m_pLabels[5]->setString(m_labelText[5] + previousData.Name);
	m_pLabels[6]->setString(m_labelText[6] + previousData.PathCost);
	m_pLabels[7]->setString(m_labelText[7] + previousData.TilesExplored);
	m_pLabels[8]->setString(m_labelText[8] + previousData.TimeTaken);
}

void Display::Draw(const std::shared_ptr<sf::RenderWindow>& pWindow)
{
	pWindow->draw(*m_pCanvas);

	for (const auto& pText : m_pLabels)
	{
		pWindow->draw(*pText);
	}
}