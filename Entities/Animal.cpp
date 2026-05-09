#include "Animal.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

// Static random generator (FEATURE 15)
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

// FEATURE 15: Random movement step
void Chick::moveStep()
{
	// Change direction occasionally (every ~10 frames)
	static int stepCounter = 0;
	stepCounter++;
	if (stepCounter % 10 == 0)
	{
		int speed = randomInt(1, 3);			// chicken speed
		curr_vel.x = randomInt(-speed, speed);
		curr_vel.y = randomInt(-speed, speed);
	}

	// Update position
	RefPoint.x += curr_vel.x;
	RefPoint.y += curr_vel.y;
	curr_pos = RefPoint;

	// Keep inside playing area (between toolbar*2 and statusbar)
	const int minX = 0, maxX = config.windWidth - width;
	const int minY = 2 * config.toolBarHeight, maxY = config.windHeight - config.statusBarHeight - height;

	if (RefPoint.x < minX) { RefPoint.x = minX; curr_vel.x = -curr_vel.x; }
	if (RefPoint.x > maxX) { RefPoint.x = maxX; curr_vel.x = -curr_vel.x; }
	if (RefPoint.y < minY) { RefPoint.y = minY; curr_vel.y = -curr_vel.y; }
	if (RefPoint.y > maxY) { RefPoint.y = maxY; curr_vel.y = -curr_vel.y; }
}

Cow::Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: Animal(r_pGame, r_point, r_width, r_height, img_path)
{
}

// Cow movement (similar but slower)
void Cow::moveStep()
{
	static int stepCounter = 0;
	stepCounter++;
	if (stepCounter % 15 == 0)	// cows change direction less often
	{
		int speed = randomInt(1, 2);		// slower
		curr_vel.x = randomInt(-speed, speed);
		curr_vel.y = randomInt(-speed, speed);
	}

	RefPoint.x += curr_vel.x;
	RefPoint.y += curr_vel.y;
	curr_pos = RefPoint;

	const int minX = 0, maxX = config.windWidth - width;
	const int minY = 2 * config.toolBarHeight, maxY = config.windHeight - config.statusBarHeight - height;

	if (RefPoint.x < minX) { RefPoint.x = minX; curr_vel.x = -curr_vel.x; }
	if (RefPoint.x > maxX) { RefPoint.x = maxX; curr_vel.x = -curr_vel.x; }
	if (RefPoint.y < minY) { RefPoint.y = minY; curr_vel.y = -curr_vel.y; }
	if (RefPoint.y > maxY) { RefPoint.y = maxY; curr_vel.y = -curr_vel.y; }
}

Wolf::Wolf(Game* r_pGame, point r_point, int r_width, int r_height)
	: Animal(r_pGame, r_point, r_width, r_height, "")
{
	config.wolfCounter++;
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
	pWind->DrawString(x + 15, y - 15, to_string(config.wolfCounter));
}

void Wolf::moveStep()
{
	int dx = randomInt(-5, 5);
	int dy = randomInt(-5, 5);
	RefPoint.x += dx;
	RefPoint.y += dy;

	const int minX = 0, maxX = config.windWidth - width;
	const int minY = 2 * config.toolBarHeight, maxY = config.windHeight - config.statusBarHeight - height;
	if (RefPoint.x < minX) RefPoint.x = minX;
	if (RefPoint.x > maxX) RefPoint.x = maxX;
	if (RefPoint.y < minY) RefPoint.y = minY;
	if (RefPoint.y > maxY) RefPoint.y = maxY;
}

Chicken::Chicken(Game* r_pGame, point r_point, int r_width, int r_height)
	: Animal(r_pGame, r_point, r_width, r_height, "")
{
	config.chickenCounter++;
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
	pWind->DrawString(x + 5, y - 15, to_string(config.chickenCounter));
}

void Chicken::moveStep()
{
	int dx = randomInt(-3, 3);
	int dy = randomInt(-3, 3);
	RefPoint.x += dx;
	RefPoint.y += dy;

	const int minX = 0, maxX = config.windWidth - width;
	const int minY = 2 * config.toolBarHeight, maxY = config.windHeight - config.statusBarHeight - height;
	if (RefPoint.x < minX) RefPoint.x = minX;
	if (RefPoint.x > maxX) RefPoint.x = maxX;
	if (RefPoint.y < minY) RefPoint.y = minY;
	if (RefPoint.y > maxY) RefPoint.y = maxY;
}