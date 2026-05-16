#include "BudgetBar.h"
#include "../Entities/FoodArea.h"
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
	if (pGame->budget >= config.chickCost)
	{
		// Deduct money
		pGame->budget -= config.chickCost;

		// Create a random position in playing area
		point p;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> distX(range_min_x, range_max_x);
		uniform_int_distribution<int> distY(range_min_y, range_max_y);
		p.x = distX(gen);
		p.y = distY(gen);

		// Spawn drawn chicken (not chick photo)
		Chicken* newChicken = new Chicken(pGame, p, 55, 60);
		pGame->addAnimal(newChicken);

		// Redraw budget bar (the budget text will update)
		pGame->clearBudget();
		string budgetStr = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budgetStr);
	}
}

WaterIcon::WaterIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void WaterIcon::draw() const
{
	window* pWind = pGame->getWind();
	pWind->SetPen(BLUE, 2);
	pWind->SetBrush(LIGHTBLUE);
	pWind->DrawRectangle(RefPoint.x, RefPoint.y, RefPoint.x + width, RefPoint.y + height);
	pWind->SetPen(BLACK, 1);
	pWind->SetFont(14, BOLD, SWISS, "Arial");
	pWind->DrawString(RefPoint.x + 12, RefPoint.y + 15, "H2O");
}

void WaterIcon::onClick()
{
	if (pGame->budget < config.waterCost)
		return;

	pGame->budget -= config.waterCost;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> distX(range_min_x, range_max_x);
	uniform_int_distribution<int> distY(range_min_y, range_max_y);

	point p;
	p.x = distX(gen);
	p.y = distY(gen);

	FoodArea* grass = new FoodArea(pGame, p, config.foodAreaWidth, config.foodAreaHeight,
		config.foodAreaInitialCounter);
	pGame->addFoodArea(grass);

	pGame->clearBudget();
	pGame->printBudget("BUDGET = $" + to_string(pGame->budget));
}

// ---------- Budgetbar ----------
Budgetbar::Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height)
	: Drawable(r_pGame, r_point, r_width, r_height)
{
	iconsImages[BICON_CHICK] = "images\\chick.jpg";
	iconsImages[BICON_WATER] = "images\\chick.jpg";
	point p; p.x = 0; p.y = config.toolBarHeight;

	iconsList = new BudgetbarIcon * [BUDGET_ICON_COUNT];
	iconsList[BICON_CHICK] = new ChickIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[BICON_CHICK]);
	p.x += config.iconWidth;
	iconsList[BICON_WATER] = new WaterIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[BICON_WATER]);
}

Budgetbar::~Budgetbar()
{
	for (int i = 0; i < BUDGET_ICON_COUNT; i++)
		delete iconsList[i];
	delete[] iconsList;
}

void Budgetbar::draw() const
{
	// Draw the icons
	for (int i = 0; i < BUDGET_ICON_COUNT; i++)
		iconsList[i]->draw();

	window* pWind = pGame->getWind();

	pWind->SetPen(config.bkGrndColor, 1);
	pWind->SetBrush(config.bkGrndColor);
	pWind->DrawRectangle(BUDGET_ICON_COUNT * config.iconWidth, config.toolBarHeight,
		pWind->GetWidth(), 2 * config.toolBarHeight);

	// ========== FEATURE 2: Draw budget/cost text ==========
	pWind->SetPen(BLACK, 1);
	pWind->SetFont(18, PLAIN, SWISS, "Arial");

	int textX = BUDGET_ICON_COUNT * config.iconWidth + 20;
	int textY = config.toolBarHeight + 10;

	// Budget
	string budgetText = "Budget: $" + to_string(pGame->budget);
	pWind->DrawString(textX, textY, budgetText);

	// Costs
	int costX = textX + 200;
	pWind->DrawString(costX, textY,
		"Chick: $" + to_string(config.chickCost) +
		"  Water: $" + to_string(config.waterCost));
	// =====================================================

	// Draw separator line
	pWind->SetPen(BLACK, 3);
	pWind->DrawLine(0, 2 * config.toolBarHeight, pWind->GetWidth(), 2 * config.toolBarHeight);
}

bool Budgetbar::handleClick(int x, int y)
{
	if (x > BUDGET_ICON_COUNT * config.iconWidth)
		return false;

	int clickedIconIndex = x / config.iconWidth;
	iconsList[clickedIconIndex]->onClick();

	return false;
}