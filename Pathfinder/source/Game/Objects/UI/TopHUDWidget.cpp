#include "FrameworkPCH.h"

#include "TopHUDWidget.h"

TopHUDWidget::TopHUDWidget(sf::Vector2f WidgetBoxSize, sf::Color WidgetBoxColor, const std::shared_ptr<sf::RenderWindow>& pWindow)
{
	//Setting up the widget box
	m_pWidgetBox = new sf::RectangleShape(WidgetBoxSize);
	m_pWidgetBox->setFillColor(WidgetBoxColor);
	m_pWidgetBox->setOutlineColor(sf::Color(25, 25, 25));
	m_pWidgetBox->setOutlineThickness(-WidgetBoxSize.y / 30.0f);

	RePositionWidgetBox(pWindow.get());

	InitLabels(pWindow.get());
}

TopHUDWidget::~TopHUDWidget()
{
	delete m_pWidgetBox;
	delete m_pGridSizeLabel;

	for (sf::Text* pText : m_pLabels)
	{
		delete pText;
	}

	m_pLabels.clear();
	m_pLabels.shrink_to_fit();
}

void TopHUDWidget::Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow)
{
	renderWindow->draw(*m_pWidgetBox);

	for (sf::Text* pText : m_pLabels)
	{
		renderWindow->draw(*pText);
	}

	renderWindow->draw(*m_pGridSizeLabel);
}

void TopHUDWidget::RePositionWidgetBox(sf::RenderWindow* pWindow)
{
	sf::Vector2f widgetPosition(0.0f, 0.0f);
	sf::Vector2u windowSize = pWindow->getSize();

	widgetPosition.x = widgetPosition.x - ((float)windowSize.x / 2.0f);
	widgetPosition.y = widgetPosition.y - ((float)windowSize.y / 2.0f);

	m_pWidgetBox->setPosition(widgetPosition);
}

void TopHUDWidget::InitLabels(sf::RenderWindow* pWindow)
{
	//Setting up the Text
	std::string debugPath = "../../Pathfinder/Data/font/Pixellari.ttf";
	std::string releasePath = "font/Pixellari.ttf";

	if (!m_pFont.loadFromFile(debugPath))
	{
		if (!m_pFont.loadFromFile(releasePath))
		{
			std::cout << "Font not found!\n";
		}
	}

	m_pLabels.reserve(m_NumLabels);

	sf::Vector2f FirstLabelPos = m_pWidgetBox->getPosition() + sf::Vector2f(80.0f, 20.0f);
	sf::Vector2f FollowingLabelPos = FirstLabelPos;

	for (unsigned int i = 0; i < m_NumLabels; i++)
	{
		sf::Text* ptempText = new sf::Text(m_Labels[i], m_pFont, m_fontSize);
		ptempText->setFillColor(sf::Color::Black);
		ptempText->setPosition(FollowingLabelPos);

		m_pLabels.push_back(ptempText);

		FollowingLabelPos.y += 36.0f;
		if (i == 0) { continue; }
		if ((i + 1) % 3 == 0) { FollowingLabelPos.x += 350; FollowingLabelPos.y = FirstLabelPos.y; }

	}

	std::string gridSize = (std::to_string)((int)GameConst::GRID_COLUMNS);
	gridSize.append(" X ");
	gridSize.append((std::to_string)((int)GameConst::GRID_ROWS));

	m_pGridSizeLabel = new sf::Text(gridSize, m_pFont, m_fontSize + m_fontSize / 2);
	m_pGridSizeLabel->setFillColor(sf::Color(242, 166, 73));
	m_pGridSizeLabel->setOutlineThickness(4.0f);
	m_pGridSizeLabel->setOutlineColor(sf::Color::Black);

	sf::Vector2u windowSize = pWindow->getSize();
	sf::Vector2f LabelPosition(0.0f, 0.0f);

	LabelPosition.x = LabelPosition.x + ((float)windowSize.x / 2.0f) - 130.0f;
	LabelPosition.y = LabelPosition.y + ((float)windowSize.y / 2.0f) - 60.0f;
	m_pGridSizeLabel->setPosition(LabelPosition);
}

void TopHUDWidget::UpdateLabel(unsigned int LabelNum, std::string AppendString)
{
	LabelNum--;

	if (LabelNum < m_NumLabels)
	{
		m_pLabels[LabelNum]->setString(m_Labels[LabelNum] + AppendString);
	}
}
