#pragma once
#include "../Core/Drawable.h"
#include <string>
#include <random>

class Animal : public Drawable
{
protected:
	string image_path;
	static int randomInt(int min, int max);	// helper for random numbers

public:
	point curr_pos;
	point curr_vel;
	Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void draw() const override;
	virtual void moveStep() = 0;
};

class Chick : public Animal
{
public:
	Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void moveStep() override;
};

class Cow : public Animal
{
public:
	Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void moveStep() override;
};

class Chicken : public Animal
{
public:
	Chicken(Game* r_pGame, point r_point, int r_width, int r_height);
	virtual void draw() const override;
	virtual void moveStep() override;
};

class Wolf : public Animal
{
public:
	Wolf(Game* r_pGame, point r_point, int r_width, int r_height);
	virtual void draw() const override;
	virtual void moveStep() override;
};

class Warehouse : public Drawable
{
public:
	Warehouse(Game* r_pGame, point r_point, int r_width, int r_height);

	void draw() const override;
	void onClick();
	bool isClicked(int x, int y) const;
};