#include "Budgetbar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
#include <string>
using namespace std;

BudgetbarIcon::BudgetbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
}

void BudgetbarIcon::draw() const
{
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

ChickIcon::ChickIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

// FEATURE 2: onClick now uses Game's addAnimal and updates budget
void ChickIcon::onClick()
{
	const int CHICK_COST = 100;
	if (pGame->budget >= CHICK_COST)
	{
		// Deduct money
		pGame->budget -= CHICK_COST;

		// Create a random position in playing area
		point p;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> distX(range_min_x, range_max_x);
		uniform_int_distribution<int> distY(range_min_y, range_max_y);
		p.x = distX(gen);
		p.y = distY(gen);

		// Create a chicken and add to game
		Chick* newChick = new Chick(pGame, p, 50, 50, image_path);
		pGame->addAnimal(newChick);		// FEATURE 10: this method now exists

		// Redraw budget bar (the budget text will update)
		pGame->clearBudget();
		string budgetStr = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budgetStr);
	}
}

CowIcon::CowIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void CowIcon::onClick()
{
	const int COW_COST = 100;
	if (pGame->budget >= COW_COST)
	{
		pGame->budget -= COW_COST;

		point p;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> distX(range_min_x, range_max_x);
		uniform_int_distribution<int> distY(range_min_y, range_max_y);
		p.x = distX(gen);
		p.y = distY(gen);

		Cow* newCow = new Cow(pGame, p, 50, 50, image_path);
		pGame->addAnimal(newCow);

		pGame->clearBudget();
		string budgetStr = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budgetStr);
	}
}

// ---------- Budgetbar ----------
Budgetbar::Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height)
	: Drawable(r_pGame, r_point, r_width, r_height)
{
	iconsImages[ICON_CHICK] = "images\\chick.jpg";
	iconsImages[ICON_COW] = "images\\cow.jpg";
	point p; p.x = 0; p.y = config.toolBarHeight;

	iconsList = new BudgetbarIcon * [ANIMAL_COUNT];
	iconsList[ICON_CHICK] = new ChickIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_CHICK]);
	p.x += config.iconWidth;
	iconsList[ICON_COW] = new CowIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_COW]);
}

Budgetbar::~Budgetbar()
{
	for (int i = 0; i < ANIMAL_COUNT; i++)
		delete iconsList[i];
	delete[] iconsList;
}

void Budgetbar::draw() const
{
	// Draw the icons
	for (int i = 0; i < ANIMAL_COUNT; i++)
		iconsList[i]->draw();

	window* pWind = pGame->getWind();

	// ========== FEATURE 2: Draw budget/cost text ==========
	pWind->SetPen(BLACK, 1);
	pWind->SetFont(18, PLAIN, SWISS, "Arial");

	int textX = ANIMAL_COUNT * config.iconWidth + 50;
	int textY = config.toolBarHeight + 10;

	// Budget
	string budgetText = "Budget: $" + to_string(pGame->budget);
	pWind->DrawString(textX, textY, budgetText);

	// Costs
	int costX = textX + 200;
	pWind->DrawString(costX, textY, "Chicken: $100   Cow: $100");
	// =====================================================

	// Draw separator line
	pWind->SetPen(BLACK, 3);
	pWind->DrawLine(0, 2 * config.toolBarHeight, pWind->GetWidth(), 2 * config.toolBarHeight);
}

bool Budgetbar::handleClick(int x, int y)
{
	if (x > ANIMAL_COUNT * config.iconWidth)
		return false;

	int clickedIconIndex = x / config.iconWidth;
	iconsList[clickedIconIndex]->onClick();

	return false;
}