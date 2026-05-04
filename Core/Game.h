#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../UI/Toolbar.h"
#include "../UI/BudgetBar.h"
#include "../Entities/Animal.h"

class Game
{
private:
	window* pWind;
	Toolbar* gameToolbar;
	Budgetbar* gameBudgetbar;

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


	void printMessage(string msg) const;

	void go() const;

	window* getWind() const;
};

