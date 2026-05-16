#include "FoodArea.h"
#include "../Core/Game.h"
#include "../Config/GameConfig.h"
#include <string>
using namespace std;

FoodArea::FoodArea(Game* r_pGame, point r_point, int r_width, int r_height, int initialFood)
	: Drawable(r_pGame, r_point, r_width, r_height), foodCounter(initialFood)
{
}

void FoodArea::draw() const
{
	if (foodCounter <= 0)
		return;

	window* w = pGame->getWind();
	int x = RefPoint.x;
	int y = RefPoint.y;

	w->SetPen(DARKGREEN, 2);
	w->SetBrush(GREEN);
	w->DrawRectangle(x, y, x + width, y + height);

	w->SetPen(BLACK, 1);
	w->SetFont(14, BOLD, SWISS, "Arial");
	w->DrawString(x + width / 2 - 8, y + height / 2 - 8, to_string(foodCounter));
}

bool FoodArea::isClicked(int x, int y) const
{
	return x >= RefPoint.x && x <= RefPoint.x + width &&
		y >= RefPoint.y && y <= RefPoint.y + height;
}

bool FoodArea::overlaps(int ax, int ay, int aw, int ah) const
{
	if (foodCounter <= 0)
		return false;

	int bx = RefPoint.x;
	int by = RefPoint.y;
	return !(ax + aw < bx || ax > bx + width || ay + ah < by || ay > by + height);
}

void FoodArea::consumeFood(int amount)
{
	foodCounter -= amount;
	if (foodCounter < 0)
		foodCounter = 0;
}
