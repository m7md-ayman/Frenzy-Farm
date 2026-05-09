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

	// ========== FEATURE 1: Status bar data ==========
	int timerSeconds;
	int currentLevel;
	int goalAnimals;
	int animalCount;			// total animals on field (chicken + cow)
	// ===============================================

	// ========== FEATURE 10 & 15: Animal list ==========
	Animal** animals;			// array of pointers to animals
	int animalCapacity;			// current max size of array
	// ==================================================

	// ========== FEATURE 7: Product list ==========
	Product** products;
	int productCount;
	int productCapacity;
	// ============================================

public:
	int budget = 2000;
	Wolf* myWolf;
	Chicken* myChicken;

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

	// FEATURE 1
	void drawStatusBar() const;

	void printMessage(string msg) const;
	void go();
	void go() const;

	// Dynamic add helpers used by UI icons.
	void addAnimal(Animal* a);
	void addProduct(Product* p);

	window* getWind() const;
};