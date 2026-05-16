#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../UI/Toolbar.h"
#include "../UI/BudgetBar.h"
#include "../Entities/Animal.h"
#include "../Entities/FoodArea.h"
#include "../Product.h"
#include <string>
#include <fstream>

class Game
{
private:
	window* pWind;
	Toolbar* gameToolbar;
	Budgetbar* gameBudgetbar;

	int timerSeconds;
	int currentLevel;
	int goalBudget;
	int animalCount;
	Animal** animals;
	int animalCapacity;

	Product** products;
	int productCount;
	int productCapacity;

	FoodArea** foodAreas;
	int foodCount;
	int foodCapacity;

	bool isPaused;
	int frameCount;
	int secondsSinceWolfSpawn;

public:
	int budget;
	Wolf* myWolf;
	Chicken* myChicken;
	Warehouse* myWarehouse;

	Game();
	~Game();

	clicktype getMouseClick(int& x, int& y) const;
	string getSrting() const;
	window* CreateWind(int, int, int, int) const;
	void createToolbar();
	void createBudgetbar();
	void clearBudget() const;
	void printBudget(string msg) const;
	void clearStatusBar() const;
	void drawStatusBar() const;
	void drawFieldBoundaries() const;
	void printMessage(string msg) const;
	void go();
	void go() const;

	void addAnimal(Animal* a);
	void addProduct(Product* p);
	void addFoodArea(FoodArea* f);
	void removeAnimal(int index);
	void removeProduct(int index);
	void removeFoodArea(int index);
	void removeDepletedFoodAreas();

	window* getWind() const;

	void pauseGame();
	void resumeGame();
	void restartGame();

	void saveGame(string filename);
	void loadGame(string filename);

	void checkWolfCollisions();
	void checkFoodCollisions();
	void updateWolfSpawns();
	void spawnProductAtAnimal(Animal* a, bool isEgg);

	void checkLevelUp();
	int getGoalBudgetForLevel(int level) const;
	int getTimeForLevel(int level) const;

	void checkChickGrowth();
	void handlePlayingAreaClick(int x, int y);
	void handleProductClick(int x, int y);
	void handleWolfClick(int x, int y);

	void showWinScreen();
	void showGameOverScreen();

	int getAnimalCount() const { return animalCount; }
	int getCurrentLevel() const { return currentLevel; }
	bool getIsPaused() const { return isPaused; }
};
