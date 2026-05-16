#include "Product.h"
#include "Core/Game.h"

Product::Product(Game* pGame, point pos, int w, int h, string img)
	: Drawable(pGame, pos, w, h), imagePath(img)
{
}

void Product::draw() const
{
	pGame->getWind()->DrawImage(imagePath, RefPoint.x, RefPoint.y, width, height);
}

bool Product::containsPoint(int x, int y) const
{
	return x >= RefPoint.x && x <= RefPoint.x + width &&
		y >= RefPoint.y && y <= RefPoint.y + height;
}

Egg::Egg(Game* pGame, point pos)
	: Product(pGame, pos, 20, 20, "images\\egg.jpg") 
{
}

Milk::Milk(Game* pGame, point pos)
	: Product(pGame, pos, 20, 20, "images\\milk.jpg")
{
}