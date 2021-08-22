#pragma once
#include "GameCore/Objects/GameObject.h"

class TopHUDWidget : public GameObject
{
public:	
	TopHUDWidget(sf::Vector2f WidgetBoxSize, sf::Color WidgetBoxColor, sf::RenderWindow * pWindow);
	~TopHUDWidget();

	virtual void Draw(sf::RenderWindow* pWindow) override;

private:
	sf::RectangleShape* m_pWidgetBox = nullptr;

private:
	void RePositionWidgetBox(sf::RenderWindow* pWindow);

public:
	void SetWidgetBoxSize(sf::Vector2f size) { m_pWidgetBox->setSize(size); }

	sf::Vector2f GetWidgetBoxSize() { return m_pWidgetBox->getSize(); }
};
