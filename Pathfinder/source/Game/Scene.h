#pragma once

enum class TileType : char;
enum class AlgorithmType : char;
enum class AlgorithmState : char;
enum class AlgorithmVisualSpeed : int;
struct AlgorithmData;


class GameObject;
class TopHUDWidget;
class Grid;
class Algorithms;

class Scene final
{
public:	
	Scene(const std::shared_ptr<sf::RenderWindow>& pWindow);
	~Scene();

	//Game Core Functions
	void Initialize() const;
	void Update(float deltaTime);
	void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) const;

private:
	std::vector<std::shared_ptr<GameObject>> m_pGameObjects;

	std::shared_ptr<Grid> m_pGrid;
	std::shared_ptr<TopHUDWidget> m_pDisplay;
	std::shared_ptr<Algorithms> m_pAlgorithm;

private:
	bool m_AlgorithmExecuted = false;

public:
	AlgorithmVisualSpeed AlgorithmSpeed;

public:
	[[nodiscard]] const std::shared_ptr<Grid>& GetGrid() const { return m_pGrid; }
	[[nodiscard]] const std::shared_ptr<TopHUDWidget>& GetDisplay() const { return m_pDisplay; }
	[[nodiscard]] const std::shared_ptr<Algorithms>& GetAlgorithms() const { return m_pAlgorithm; }

public:
	//Algorithm Helper functions
	void ExecuteAlgorithm(AlgorithmType algorithmType);
	void StopAlgorithm();

	AlgorithmData* m_pCurrentAlgorithmData = nullptr;
	AlgorithmData* m_pPreviousAlgorithmData = nullptr;

	//Top Widget helper function
	void AutoUpdateTopWidget();
};