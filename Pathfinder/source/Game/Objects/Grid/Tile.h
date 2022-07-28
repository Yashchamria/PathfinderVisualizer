#pragma once
#include "Tile.h"
#include "GameCore/Objects/GameObject.h"

enum class TileAnimState : char;
enum class TileType : char;

class Tile : public GameObject
{
public:	
	Tile(const sf::Vector2u coord, const float size, int weight, TileType type);
	virtual void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;
	virtual void Update(float deltaTime) override;

private:
	sf::Vector2u m_coord {};
	int m_weight;
	sf::RectangleShape m_body;

public:
	TileType Type;
	void Animate(const sf::Color color);

public:
	void SetSizeAndPosition(float size);
	void SetCoordAndPosition(const sf::Vector2u coord);
	void SetColor(const sf::Color fill, const float thickness, const sf::Color outline);

public:
	[[nodiscard]] sf::Vector2u GetCoord() const { return m_coord; }
	[[nodiscard]] unsigned int GetWeight() const { return m_weight; }
	[[nodiscard]] float GetSize() const { return m_body.getSize().x; }

private:
	bool m_bAnimate {false};
};