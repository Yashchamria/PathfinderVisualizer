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
	sf::Font m_pFont;

	const static unsigned int m_NumLabels = 9;
	unsigned int m_fontSize = 20;
	
	std::vector<sf::Text*> m_pLabels;

	std::string m_Labels[m_NumLabels] = {"Algorithm - ",		  "Time Taken - ",			"Visualization Speed - ",
										 "Algorithm Cost- ",	  "Tiles Explored - ",		"Log - ",
										 "Previous Algorithm - ", "Previous Time Taken - ", "Previous Cost - "      };

	sf::Text* m_pGridSizeLabel;


private:
	void InitLabels(sf::RenderWindow* pWindow);
	void RePositionWidgetBox(sf::RenderWindow* pWindow);

public:
	void SetWidgetBoxSize(sf::Vector2f size) { m_pWidgetBox->setSize(size); }
	sf::Vector2f GetWidgetBoxSize() { return m_pWidgetBox->getSize(); }

public:
	void UpdateLabel(unsigned int LabelNum, std::string algorithmName);
};
