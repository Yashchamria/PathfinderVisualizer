#pragma once
#include "GameObject/GameObject.h"

struct AlgorithmData;
enum VisualSpeed : uint32_t;

class Display final : public GameObject
{
public:
	Display(const float height, const sf::Color color, const sf::Vector2u windowSize, const sf::Vector2u gridSize);
	void Draw(const std::shared_ptr<sf::RenderWindow>& pWindow) override;

public:
	void Log(const std::string& message) const;
	void SetSpeed(const VisualSpeed speed) const;
	void SetCurrentData(const AlgorithmData& currentData) const;
	void SetPreviousData(const AlgorithmData& previousData) const;

private:
	std::unique_ptr<sf::RectangleShape> m_pCanvas{};
	sf::Font m_pFont;

	uint8_t m_fontSize = 19;
	static constexpr uint8_t m_totalLabels = 11;

	std::unique_ptr<sf::Text> m_pLabels[m_totalLabels]{};

	const std::string m_labelText[m_totalLabels] =
	{
		"Grid Size",
		"Current - ",
		"Cost- ",
		"Explored - ",
		"Time - ",
		"Previous - ",
		"Cost - ",
		"Explored - ",
		"Time - ",
		"Speed - ",
		"Log - ",
	};

public:
	sf::Vector2f GetSize() const { return m_pCanvas->getSize(); }
};