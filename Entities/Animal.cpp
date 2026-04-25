#include "Animal.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

Animal::Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
	curr_pos = r_point;
	curr_vel.x = 1;
	curr_vel.y = 1;

}

void Animal::draw() const
{
	//draw image of this object
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

Chick::Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
{}

void Chick::moveStep()
{
	//TO DO: add code for cleanup and game exit here
	/*
	//draw image of this object in the field
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
	*/
	cout << "Icon Chick Clicked" << endl;
}

Cow::Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
{}

void Cow::moveStep()
{
	//TO DO: add code for cleanup and game exit here
	cout << "Icon Cow Clicked" << endl;

}

Wolf::Wolf(Game* r_pGame, point r_point, int r_width, int r_height) : Animal(r_pGame, r_point, r_width, r_height, "")
{}

void Wolf::draw() const
{
	window* pWind = pGame->getWind();
	int x = RefPoint.x;
	int y = RefPoint.y;

	// body
	pWind->SetPen(GRAY, 2);
	pWind->SetBrush(GRAY);
	pWind->DrawRectangle(x, y + 18, x + 44, y + 38);

	// square head
	pWind->DrawRectangle(x + 28, y + 2, x + 52, y + 20);

	// left ear
	pWind->SetPen(DARKGRAY, 2);
	pWind->SetBrush(DARKGRAY);
	pWind->DrawRectangle(x + 29, y - 8, x + 35, y + 4);

	// right ear
	pWind->DrawRectangle(x + 44, y - 8, x + 50, y + 4);

	// left eye
	pWind->SetPen(RED, 2);
	pWind->SetBrush(RED);
	pWind->DrawRectangle(x + 31, y + 6, x + 36, y + 11);

	// right eye
	pWind->DrawRectangle(x + 43, y + 6, x + 48, y + 11);

	// four legs
	pWind->SetPen(DARKGRAY, 2);
	pWind->SetBrush(DARKGRAY);
	pWind->DrawRectangle(x + 4, y + 37, x + 11, y + 52);
	pWind->DrawRectangle(x + 14, y + 37, x + 21, y + 52);
	pWind->DrawRectangle(x + 26, y + 37, x + 33, y + 52);
	pWind->DrawRectangle(x + 35, y + 37, x + 42, y + 52);

	// tail
	pWind->DrawRectangle(x - 10, y + 19, x + 2, y + 26);
}

void Wolf::moveStep()
{
	window* pWind = pGame->getWind();

	// erase wolf from old position
	pWind->DrawImage("images\\background.jpeg", 0, config.toolBarHeight * 2,
		config.windWidth, config.windHeight);

	// move to a new random position
	int r1 = rand() % 20 - 10;
	int r2 = rand() % 20 - 10;
	RefPoint.x = RefPoint.x + r1;
	RefPoint.y = RefPoint.y + r2;

	// draw wolf at new position
	draw();
}