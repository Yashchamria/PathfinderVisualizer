#include "FrameworkPCH.h"
#include "Tile.h"

Tile::Tile()
{
	rect.setFillColor(sf::Color::Black);
}

Tile::~Tile()
{
}

void Tile::Initialize()
{
	GameObject::Initialize();
}

void Tile::PostInitialize()
{
	GameObject::PostInitialize();
}

void Tile::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void Tile::Draw(sf::RenderWindow* pWindow)
{
	GameObject::Draw(pWindow);
	pWindow->draw(rect);
}
