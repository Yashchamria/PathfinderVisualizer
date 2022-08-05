#pragma once
#include "GameObject/GameObject.h"

enum class Direction : char;

class Selector : public GameObject
{
public:	
	Selector(const sf::Vector2u coord, const float size, const sf::Color fill, const float thickness, const sf::Color outline);
	virtual void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;

private:
	sf::Vector2u m_coord {};
	sf::RectangleShape m_body;

public:
	void SetSizeAndPosition(float size);
	void SetCoordAndPosition(const sf::Vector2f worldPosition);
	void SetCoordAndPosition(const Direction direction);
	void SetCoordAndPosition(const sf::Vector2u coord);

public:
	[[nodiscard]] sf::Vector2u GetCoord() const { return m_coord; }
};