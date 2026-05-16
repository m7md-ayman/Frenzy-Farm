#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../UI/Toolbar.h"
#include "../UI/BudgetBar.h"
#include "../Entities/Animal.h"
#include "../Product.h"

class Game
{
private:
	window* pWind;
	Toolbar* gameToolbar;
	Budgetbar* gameBudgetbar;

	int timerSeconds;
	int currentLevel;
	int goalAnimals;
	int animalCount;
	Animal** animals;
	int animalCapacity;

	Product** products;
	int productCount;
	int productCapacity;

public:
	int budget = 2000;
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

	void printMessage(string msg) const;
	void go();
	void go() const;

	void addAnimal(Animal* a);
	void addProduct(Product* p);

	window* getWind() const;
};