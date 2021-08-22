#include "FrameworkPCH.h"

#include "TopHUDWidget.h"

TopHUDWidget::TopHUDWidget(sf::Vector2f WidgetBoxSize, sf::Color WidgetBoxColor, sf::RenderWindow* pWindow)
{
	m_pWidgetBox = new sf::RectangleShape(WidgetBoxSize);
	m_pWidgetBox->setFillColor(WidgetBoxColor);
	m_pWidgetBox->setOutlineColor(sf::Color::Black);
	m_pWidgetBox->setOutlineThickness(-WidgetBoxSize.y / 30.0f);

	RePositionWidgetBox(pWindow);
}

TopHUDWidget::~TopHUDWidget()
{
	delete m_pWidgetBox;
}

void TopHUDWidget::Draw(sf::RenderWindow* pWindow)
{
	GameObject::Draw(pWindow);

	pWindow->draw(*m_pWidgetBox);
}

void TopHUDWidget::RePositionWidgetBox(sf::RenderWindow* pWindow)
{
	sf::Vector2f widgetPosition(0.0f, 0.0f);
	sf::Vector2u windowSize = pWindow->getSize();

	//TilePos = TileCoord * TileSize + OffsetTileCentre - OffsetToWindowsTopMostCorner
	widgetPosition.x = widgetPosition.x - ((float)windowSize.x / 2.0f);
	widgetPosition.y = widgetPosition.y - ((float)windowSize.y / 2.0f);

	m_pWidgetBox->setPosition(widgetPosition);
}
