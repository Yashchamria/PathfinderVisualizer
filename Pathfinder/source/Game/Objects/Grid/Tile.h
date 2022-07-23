#pragma once
#include "GameCore/Objects/GameObject.h"

enum class TileAnimationState : char;
enum class TileType : char;

class Tile : public GameObject
{
public:	
	Tile(sf::Vector2u TileCoord = sf::Vector2u(0, 0), sf::Vector2f TileSize = sf::Vector2f(50.0f, 50.0f));

	virtual void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;
	virtual void Update(float deltaTime) override;

private:
	sf::RectangleShape m_TileBody;
	sf::Vector2u m_tileCoord;
	TileType m_tileType;
	TileAnimationState m_tileAnimationState;
	unsigned int m_Weight = 10;

	float m_tileFillScale = 0.92f;

	void Animate(sf::Color tileColor);
	sf::RectangleShape m_TileAnimationBody;
	bool m_IsAnimationChanged = false;

public:
	void RepositionTile(sf::Vector2u windowSize, sf::Vector2f TopWidgetSize);
	void UpdateTileType();
	void UpdateTileAnimationProperty();

public:
	void SetTileCoord(sf::Vector2u tileCoord);
	void SetTileSize(sf::Vector2f tileSize, float OutlineThicknessFactor);
	void SetTileColor(sf::Color fillColor = sf::Color::White, sf::Color OutlineColor = sf::Color::Black);
	void SetTileType(TileType tileType);
	void SetTileAnimationProperty(TileAnimationState tileAnimationState);

private:
	void SetAnimTileScale(float tileScale);
	float GetAnimTileScale();

public:
	sf::Vector2u GetTileCoord() { return m_tileCoord; }
	unsigned int GetTileWeight() { return m_Weight; }
	TileType GetTileType() { return m_tileType; }
	TileAnimationState GetTileAnimationState() { return m_tileAnimationState; }
};