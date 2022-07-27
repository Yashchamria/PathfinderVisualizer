#pragma once
#include "GameCore/Objects/GameObject.h"

enum class TileAnimState : char;
enum class TileType : char;

class Tile : public GameObject
{
public:	
	Tile(sf::Vector2u TileCoord = sf::Vector2u(0, 0), sf::Vector2f TileSize = sf::Vector2f(50.0f, 50.0f));

	virtual void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;
	virtual void Update(float deltaTime) override;

private:
	sf::RectangleShape m_body;
	sf::Vector2u m_coord;
	TileType m_type;
	TileAnimState m_tileAnimationState;
	unsigned int m_Weight = 10;

	float m_tileFillScale = 0.92f;

	void Animate(sf::Color tileColor);
	sf::RectangleShape m_animBody;
	bool m_IsAnimationChanged = false;

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

private:
	void SetAnimTileScale(float tileScale);
	float GetAnimTileScale();

public:
	sf::Vector2u GetTileCoord() { return m_coord; }
	unsigned int GetTileWeight() { return m_Weight; }
	TileType GetTileType() { return m_type; }
	TileAnimState GetTileAnimationState() { return m_tileAnimationState; }
};