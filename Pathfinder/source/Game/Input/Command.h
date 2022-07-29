//A simple facade class to keep the Input Manager clean and not exposing unnecessary functions and members to it.

#pragma once

enum class Direction : char;
class Scene;
enum class TileType : char;
enum class AlgorithmType : char;

class Command
{
public:	
	Command(Scene* pScene);
	~Command();

public:
	void ResizeGrid(float ColumnIncrement);
	void SetSelectorPosition(Direction direction);
	void SetSelectorPosition(const sf::Vector2f position);

	void UpdateTileProperty(TileType tileType);
	void ExecuteAlgorithm(AlgorithmType algorithmType);
	void ClearAlgorithmSearch();
	void ClearGrid();
	void IncreaseVisualSpeed();
	void DecreaseVisualSpeed();
	void GenerateRandomGrid(int wallPercent);

private:
	class Scene* m_pScene;

	float m_zoomSteps {0.0f};

	AlgorithmType m_CurrentAlgorithm;

private:
	void HandleOngoingAlgorithm(bool bReRunAlgorithm = false);
	bool m_AlgorithmStopped = false;
};