#pragma once

class Grid;
class Tile;
struct AlgorithmData;

typedef std::queue<std::pair<std::shared_ptr<Tile>, sf::Color>> AnimationSequence;

class IAlgorithm
{
public:
	virtual ~IAlgorithm() = default;

public:
	virtual std::shared_ptr<AlgorithmData> OnExecute(const std::shared_ptr<Grid>& pGrid) = 0;

	virtual void OnAbort()
	{
		AnimationSequence empty;
		m_animationSequence.swap(empty);
	}

public:
	[[nodiscard]] AnimationSequence& GetAnimationSequence() { return m_animationSequence; }

protected:
	AnimationSequence m_animationSequence;
};