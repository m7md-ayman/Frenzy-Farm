#pragma once
#include "Core/Drawable.h"
#include <string>

class Product : public Drawable
{
protected:
	std::string imagePath;

public:
	Product(Game* pGame, point pos, int w, int h, std::string img);
	virtual void draw() const override;
	virtual bool isEgg() const { return false; }
	bool containsPoint(int x, int y) const;
	int getX() const { return RefPoint.x; }
	int getY() const { return RefPoint.y; }
	virtual ~Product() = default;
};

class Egg : public Product
{
public:
	Egg(Game* pGame, point pos);
	bool isEgg() const override { return true; }
};

class Milk : public Product
{
public:
	Milk(Game* pGame, point pos);
};
