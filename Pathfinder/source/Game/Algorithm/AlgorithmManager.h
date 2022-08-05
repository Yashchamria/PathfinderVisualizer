#pragma once
#include "GameCore/Objects/GameObject.h"

class Tile;
class Grid;
class Display;
class IAlgorithm;
struct AlgorithmData;

class AlgorithmManager final: public GameObject
{
public:
	AlgorithmManager(const std::shared_ptr<Grid>& pGrid, const std::shared_ptr<Display>& pDisplay);
	void Update(float deltaTime) override;

public:
	void Execute(std::shared_ptr<IAlgorithm> pAlgorithm);
	void ReExecuteIfRequired();
	void Abort();

public:
	VisualSpeed AnimSpeed {Normal};

private:
	bool m_bAnimate {false};

private:
	const std::shared_ptr<Grid> m_pGrid {};
	const std::shared_ptr<Display> m_pDisplay {};

private:
	std::shared_ptr<IAlgorithm> m_pCurrentAlgorithm;
	std::shared_ptr<AlgorithmData> m_pCurrentData;
};