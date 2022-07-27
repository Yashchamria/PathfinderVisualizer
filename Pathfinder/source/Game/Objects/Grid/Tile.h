#pragma once
#include "Tile.h"
#include "GameCore/Objects/GameObject.h"

enum class TileAnimState : char;
enum class TileType : char;

class Tile : public GameObject
{
public:	
	Tile(sf::Vector2u coord, sf::Vector2f size, int weight, TileType type);
	virtual void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;
	virtual void Update(float deltaTime) override;

private:
	sf::Vector2u m_coord {};
	int m_weight;
	TileType m_type;

	sf::RectangleShape m_body;
	TileAnimState m_animState;

	float m_fillScale = 0.92f;

	sf::RectangleShape m_animBody;

private:
	bool m_bAnimate {false};

private:
	void Animate(const sf::Color color);

public:
	void RepositionTile();
	void UpdateTileType();
	void UpdateTileAnimationProperty();

public:
	void SetTileCoord(sf::Vector2u tileCoord);
	void SetTileSize(sf::Vector2f tileSize, float OutlineThicknessFactor);
	void SetTileColor(sf::Color fillColor = sf::Color::White, sf::Color OutlineColor = sf::Color::Black);
	void SetTileType(TileType tileType);
	void SetTileAnimationProperty(TileAnimState tileAnimationState);

public:
	sf::Vector2u GetCoord() const { return m_coord; }
	unsigned int GetWeight() const { return m_weight; }
	TileType GetType() const { return m_type; }
	TileAnimState GetAnimState() const { return m_animState; }
};