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
	virtual ~Product() = default;
};

class Egg : public Product
{
public:
	Egg(Game* pGame, point pos);
};

class Milk : public Product
{
public:
	Milk(Game* pGame, point pos);
};

