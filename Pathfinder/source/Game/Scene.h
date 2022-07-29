#pragma once

enum class TileType : char;
enum class AlgorithmType : char;
enum class AlgorithmState : char;
enum class VisualSpeed : int;
struct AlgorithmData;


class GameObject;
class Grid;
class Display;
class Algorithms;
class Selector;

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
	std::shared_ptr<Selector> m_pSelector;
	std::shared_ptr<Display> m_pDisplay;
	std::shared_ptr<Algorithms> m_pAlgorithm;

private:
	bool m_AlgorithmExecuted = false;

public:
	VisualSpeed AlgorithmSpeed;

public:
	[[nodiscard]] const std::shared_ptr<Grid>& GetGrid() const { return m_pGrid; }
	[[nodiscard]] const std::shared_ptr<Selector>& GetSelector() const { return m_pSelector; }
	[[nodiscard]] const std::shared_ptr<Display>& GetDisplay() const { return m_pDisplay; }
	[[nodiscard]] const std::shared_ptr<Algorithms>& GetAlgorithms() const { return m_pAlgorithm; }

public:
	void ExecuteAlgorithm(AlgorithmType algorithmType);
	void StopAlgorithm();
};