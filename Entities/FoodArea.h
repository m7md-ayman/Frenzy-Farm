#pragma once
#include "../Core/Drawable.h"

class FoodArea : public Drawable
{
	int foodCounter;

public:
	FoodArea(Game* r_pGame, point r_point, int r_width, int r_height, int initialFood);

	void draw() const override;
	bool isClicked(int x, int y) const;
	bool overlaps(int ax, int ay, int aw, int ah) const;

	int getFoodCounter() const { return foodCounter; }
	void consumeFood(int amount = 1);
	bool isDepleted() const { return foodCounter <= 0; }

	int getX() const { return RefPoint.x; }
	int getY() const { return RefPoint.y; }
	int getW() const { return width; }
	int getH() const { return height; }
};
