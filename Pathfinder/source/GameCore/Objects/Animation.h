#pragma once

enum class AnimationPreset : char
{
	BounceBack,
};

class Animation
{
public:	
	Animation(sf::RectangleShape* pBody, sf::Color bodyColor, AnimationPreset bodyAnimation);
	virtual ~Animation();

private:
	float m_animationSpeed = 3.0f;

private:
	void SetAnimation();
	void BounceBack(sf::RectangleShape* pBody);

public:
	void PlayAnimation();

	//Game Core Functions
	//virtual void Initialize();
	//virtual void Update(float deltaTime);
	//virtual void Draw(sf::RenderWindow* pWindow);
};
