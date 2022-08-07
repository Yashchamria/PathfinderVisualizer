#pragma once
#include "GameObject/GameObject.h"

enum class TileType : char;

class Tile : public GameObject
{
public:
	Tile(const sf::Vector2u coord, const float size, TileType type, int weight, const sf::Font& font);
	virtual void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;
	virtual void Update(float deltaTime) override;

public:
	void SetSizeAndPosition(float size);
	void Animate(const sf::Color color, const VisualSpeed animSpeed = Normal);

	void SetWeightAndLabel(const int weight);

public:
	const sf::Vector2u Coord {};
	TileType Type {};
	int Weight {1};

private:
	sf::RectangleShape m_body;
	sf::Text m_text;

private:
	bool m_bAnimate {false};
	VisualSpeed m_animSpeed {Normal};
};