#pragma once
#include "GameCore/Objects/GameObject.h"

class TopHUDWidget : public GameObject
{
public:	
	TopHUDWidget(sf::Vector2f WidgetBoxSize, sf::Color WidgetBoxColor, const std::shared_ptr<sf::RenderWindow>& pWindow);
	~TopHUDWidget();

	void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;

private:
	sf::RectangleShape* m_pWidgetBox = nullptr;
	sf::Font m_pFont;

	const static unsigned int m_NumLabels = 9;
	unsigned int m_fontSize = 20;
	
	sf::Text* m_pGridSizeLabel;
	std::vector<sf::Text*> m_pLabels;

	std::string m_Labels[m_NumLabels] = {"Algorithm - "         ,	 "Time Taken - "         ,		"Visualization Speed - ",
										 "Path Cost- "          ,	 "Tiles Explored - "     ,		"Log - ",
										 "Previous Algorithm - ",    "Previous Time Taken - ",		"Previous Cost - "      };



private:
	void InitLabels(sf::RenderWindow* pWindow);
	void RePositionWidgetBox(sf::RenderWindow* pWindow);

public:
	void SetWidgetBoxSize(sf::Vector2f size) { m_pWidgetBox->setSize(size); }
	sf::Vector2f GetWidgetBoxSize() { return m_pWidgetBox->getSize(); }

public:
	void UpdateLabel(unsigned int LabelNum, std::string AppendString);
};
