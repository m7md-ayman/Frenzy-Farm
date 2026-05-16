#pragma once
#include "../Core/Drawable.h"
#include <string>
#include <random>

enum AnimalType
{
	ANIMAL_CHICK,
	ANIMAL_COW,
	ANIMAL_CHICKEN,
	ANIMAL_WOLF
};

class Animal : public Drawable
{
protected:
	string image_path;
	static int randomInt(int min, int max);
	int age;
	int productionCounter;
	bool onGrass;

public:
	point curr_pos;
	point curr_vel;
	Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual ~Animal();
	virtual void draw() const override;
	virtual void moveStep() = 0;
	virtual AnimalType getType() const = 0;
	virtual int getProductionMaxFrames() const { return 0; }
	virtual void drawProductionCounter() const;

	void setOnGrass(bool v) { onGrass = v; }
	void addProductionProgress(int amount);
	void resetProductionCounter() { productionCounter = 0; }
	int getProductionCounter() const { return productionCounter; }

	void smoothRandomWalk(int minSpeed, int maxSpeed, int headingChangePeriod);

	int getX() const { return RefPoint.x; }
	int getY() const { return RefPoint.y; }
	int getW() const { return width; }
	int getH() const { return height; }
};

class Chick : public Animal
{
public:
	Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void moveStep() override;
	virtual AnimalType getType() const override { return ANIMAL_CHICK; }
	int getAge() const { return age; }
};

class Cow : public Animal
{
public:
	Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void moveStep() override;
	virtual void draw() const override;
	virtual AnimalType getType() const override { return ANIMAL_COW; }
	int getProductionMaxFrames() const override;
};

class Chicken : public Animal
{
	int chickenNumber;
public:
	Chicken(Game* r_pGame, point r_point, int r_width, int r_height);
	virtual void draw() const override;
	virtual void moveStep() override;
	virtual AnimalType getType() const override { return ANIMAL_CHICKEN; }
	int getProductionMaxFrames() const override;
};

class Wolf : public Animal
{
	int moveSpeed;
	int clickCount;
	int wolfNumber;

public:
	Wolf(Game* r_pGame, point r_point, int r_width, int r_height, int speed = 3);
	virtual void draw() const override;
	virtual void moveStep() override;
	virtual AnimalType getType() const override { return ANIMAL_WOLF; }

	void registerClick() { clickCount++; }
	int getClickCount() const { return clickCount; }
	void resetClickCount() { clickCount = 0; }
};

class Warehouse : public Drawable
{
public:
	Warehouse(Game* r_pGame, point r_point, int r_width, int r_height);

	void draw() const override;
	void onClick();
	bool isClicked(int x, int y) const;
};