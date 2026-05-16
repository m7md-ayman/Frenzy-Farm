#include "Animal.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

int Animal::randomInt(int min, int max)
{
	static random_device rd;
	static mt19937 gen(rd());
	uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

Animal::Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
	curr_pos = r_point;
	curr_vel.x = 1;
	curr_vel.y = 1;
	age = 0;
	productionCounter = 0;
	onGrass = false;
}

Animal::~Animal() {}

void Animal::drawProductionCounter() const
{
	int maxF = getProductionMaxFrames();
	if (maxF <= 0) return;

	window* w = pGame->getWind();
	w->SetPen(BLACK, 1);
	w->SetFont(12, BOLD, SWISS, "Arial");
	int pct = (productionCounter * 100) / maxF;
	if (pct > 100) pct = 100;
	w->DrawString(RefPoint.x, RefPoint.y - 12, to_string(pct) + "%");
}

void Animal::addProductionProgress(int amount)
{
	productionCounter += amount;
	int maxF = getProductionMaxFrames();
	if (maxF > 0 && productionCounter > maxF)
		productionCounter = maxF;
}

void Animal::smoothRandomWalk(int minSpeed, int maxSpeed, int headingChangePeriod)
{
	if (headingChangePeriod < 1)
		headingChangePeriod = 1;

	if (age % headingChangePeriod == 0 || (curr_vel.x == 0 && curr_vel.y == 0))
	{
		int speed = randomInt(minSpeed, maxSpeed);
		int attempts = 0;
		do
		{
			curr_vel.x = randomInt(-speed, speed);
			curr_vel.y = randomInt(-speed, speed);
			attempts++;
		} while (curr_vel.x == 0 && curr_vel.y == 0 && attempts < 12);

		if (curr_vel.x == 0 && curr_vel.y == 0)
			curr_vel.x = speed;
	}

	RefPoint.x += curr_vel.x;
	RefPoint.y += curr_vel.y;
	curr_pos = RefPoint;

	const int minX = 0;
	const int maxX = config.windWidth - width;
	const int minY = 2 * config.toolBarHeight;
	const int maxY = config.windHeight - config.statusBarHeight - height;

	if (RefPoint.x < minX)
	{
		RefPoint.x = minX;
		curr_vel.x = abs(curr_vel.x);
		if (curr_vel.x == 0) curr_vel.x = minSpeed;
	}
	if (RefPoint.x > maxX)
	{
		RefPoint.x = maxX;
		curr_vel.x = -abs(curr_vel.x);
		if (curr_vel.x == 0) curr_vel.x = -minSpeed;
	}
	if (RefPoint.y < minY)
	{
		RefPoint.y = minY;
		curr_vel.y = abs(curr_vel.y);
		if (curr_vel.y == 0) curr_vel.y = minSpeed;
	}
	if (RefPoint.y > maxY)
	{
		RefPoint.y = maxY;
		curr_vel.y = -abs(curr_vel.y);
		if (curr_vel.y == 0) curr_vel.y = -minSpeed;
	}
}

void Animal::draw() const
{
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

Chick::Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: Animal(r_pGame, r_point, r_width, r_height, img_path)
{
}

void Chick::moveStep()
{
	age++;
	smoothRandomWalk(3, 6, 12);
}

Cow::Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: Animal(r_pGame, r_point, r_width, r_height, img_path)
{
}

void Cow::moveStep()
{
	age++;
	smoothRandomWalk(2, 5, 14);
}

void Cow::draw() const
{
	Animal::draw();
	drawProductionCounter();
}

int Cow::getProductionMaxFrames() const
{
	return config.cowProductionFrames;
}

Wolf::Wolf(Game* r_pGame, point r_point, int r_width, int r_height, int speed)
	: Animal(r_pGame, r_point, r_width, r_height, ""), moveSpeed(speed), clickCount(0)
{
	config.wolfCounter++;
	wolfNumber = config.wolfCounter;
}

void Wolf::draw() const
{
	window* pWind = pGame->getWind();
	int x = RefPoint.x;
	int y = RefPoint.y;

	pWind->SetPen(GRAY, 2);
	pWind->SetBrush(GRAY);
	pWind->DrawRectangle(x, y + 18, x + 44, y + 38);
	pWind->DrawRectangle(x + 28, y + 2, x + 52, y + 20);

	pWind->SetPen(DARKGRAY, 2);
	pWind->SetBrush(DARKGRAY);
	pWind->DrawRectangle(x + 29, y - 8, x + 35, y + 4);
	pWind->DrawRectangle(x + 44, y - 8, x + 50, y + 4);
	pWind->DrawRectangle(x + 4, y + 37, x + 11, y + 52);
	pWind->DrawRectangle(x + 14, y + 37, x + 21, y + 52);
	pWind->DrawRectangle(x + 26, y + 37, x + 33, y + 52);
	pWind->DrawRectangle(x + 35, y + 37, x + 42, y + 52);
	pWind->DrawRectangle(x - 10, y + 19, x + 2, y + 26);

	pWind->SetPen(RED, 2);
	pWind->SetBrush(RED);
	pWind->DrawRectangle(x + 31, y + 6, x + 36, y + 11);
	pWind->DrawRectangle(x + 43, y + 6, x + 48, y + 11);

	pWind->SetPen(WHITE, 1);
	pWind->DrawString(x + 15, y - 15, to_string(wolfNumber));
}

void Wolf::moveStep()
{
	age++;
	int wolfMin = moveSpeed;
	int wolfMax = moveSpeed + 3;
	if (wolfMax < wolfMin + 1)
		wolfMax = wolfMin + 1;
	smoothRandomWalk(wolfMin, wolfMax, 18);
}

Chicken::Chicken(Game* r_pGame, point r_point, int r_width, int r_height)
	: Animal(r_pGame, r_point, r_width, r_height, "")
{
	config.chickenCounter++;
	chickenNumber = config.chickenCounter;
}

void Chicken::draw() const
{
	window* pWind = pGame->getWind();
	int x = RefPoint.x;
	int y = RefPoint.y;

	pWind->SetPen(ORANGE, 2);
	pWind->SetBrush(ORANGE);
	pWind->DrawRectangle(x + 5, y + 20, x + 40, y + 45);
	pWind->DrawRectangle(x + 25, y + 5, x + 45, y + 22);

	pWind->SetPen(RED, 2);
	pWind->SetBrush(RED);
	pWind->DrawRectangle(x + 28, y - 5, x + 34, y + 7);
	pWind->DrawRectangle(x + 34, y - 2, x + 40, y + 7);

	pWind->SetPen(YELLOW, 2);
	pWind->SetBrush(YELLOW);
	pWind->DrawRectangle(x + 44, y + 12, x + 52, y + 17);
	pWind->DrawRectangle(x + 12, y + 44, x + 18, y + 56);
	pWind->DrawRectangle(x + 25, y + 44, x + 31, y + 56);
	pWind->DrawRectangle(x + 8, y + 55, x + 20, y + 59);
	pWind->DrawRectangle(x + 22, y + 55, x + 34, y + 59);

	pWind->SetPen(BLACK, 2);
	pWind->SetBrush(BLACK);
	pWind->DrawRectangle(x + 37, y + 8, x + 42, y + 13);

	pWind->SetPen(BROWN, 2);
	pWind->SetBrush(BROWN);
	pWind->DrawRectangle(x + 8, y + 24, x + 28, y + 38);

	pWind->SetPen(WHITE, 1);
	pWind->DrawString(x + 20, y + height + 5, to_string(chickenNumber));
	drawProductionCounter();
}

int Chicken::getProductionMaxFrames() const
{
	return config.chickenProductionFrames;
}

void Chicken::moveStep()
{
	age++;
	smoothRandomWalk(3, 6, 10);
}

Warehouse::Warehouse(Game* r_pGame, point r_point, int r_width, int r_height)
	: Drawable(r_pGame, r_point, r_width, r_height)
{
}

void Warehouse::draw() const
{
	window* pWind = pGame->getWind();

	int x = RefPoint.x;
	int y = RefPoint.y;

	color woodLight = WHEAT;
	color woodMid = SANDYBROWN;
	color woodDeep = BURLYWOOD;

	pWind->SetPen(woodMid, 2);
	pWind->SetBrush(woodLight);
	pWind->DrawRectangle(x, y, x + 140, y + 20);

	pWind->SetPen(woodMid, 2);
	pWind->SetBrush(woodLight);
	pWind->DrawRectangle(x, y + 100, x + 140, y + 120);

	pWind->SetPen(woodDeep, 2);
	pWind->SetBrush(woodMid);
	pWind->DrawRectangle(x, y + 20, x + 15, y + 100);
	pWind->DrawRectangle(x + 125, y + 20, x + 140, y + 100);
	pWind->SetBrush(TAN);
	pWind->DrawRectangle(x + 45, y + 20, x + 58, y + 100);
	pWind->DrawRectangle(x + 82, y + 20, x + 95, y + 100);
}

void Warehouse::onClick()
{
	window* w = new window(300, 250, 400, 200);

	bool running = true;

	while (running)
	{
		w->SetPen(WHITE, 1);
		w->SetBrush(WHITE);
		w->DrawRectangle(0, 0, 300, 250);

		w->SetPen(BLACK, 1);
		w->SetFont(16, BOLD, SWISS, "Arial");
		w->DrawString(90, 10, "Warehouse");

		w->SetPen(BLACK, 2);
		w->DrawLine(0, 40, 300, 40);

		w->SetPen(BLACK, 1);
		w->SetFont(13, BOLD, SWISS, "Arial");
		w->DrawString(20, 60, "Egg  : " + to_string(config.eggCount));

		w->SetPen(BLACK, 1);
		w->SetBrush(config.eggCount > 0 ? LIGHTGREEN : LIGHTGRAY);
		w->DrawRectangle(180, 55, 260, 85);
		w->SetFont(12, BOLD, SWISS, "Arial");
		if (config.eggCount > 0)
			w->DrawString(195, 62, "Sell +" + to_string(config.eggSellPrice));
		else
			w->DrawString(188, 62, "Sell (empty)");

		w->SetPen(BLACK, 1);
		w->SetBrush(LIGHTGRAY);
		w->DrawRectangle(90, 190, 210, 220);
		w->SetFont(12, BOLD, SWISS, "Arial");
		w->DrawString(115, 197, "Close");

		int cx, cy;
		w->WaitMouseClick(cx, cy);

		if (cx >= 180 && cx <= 260 && cy >= 55 && cy <= 85)
		{
			if (config.eggCount > 0)
			{
				config.eggCount--;
				pGame->budget += config.eggSellPrice;
				pGame->clearBudget();
				pGame->printBudget("BUDGET = $" + to_string(pGame->budget));
			}
		}
		else if (cx >= 90 && cx <= 210 && cy >= 190 && cy <= 220)
		{
			running = false;
		}

	}

	delete w;
}

bool Warehouse::isClicked(int x, int y) const
{
	return (x >= RefPoint.x &&
		x <= RefPoint.x + width &&
		y >= RefPoint.y &&
		y <= RefPoint.y + height);
}