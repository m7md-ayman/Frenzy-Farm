#include "Game.h"
#include "../Config/GameConfig.h"
#include "../CMUgraphicsLib/auxil.h"
#include <string>
#include <random>   // for random movement
#include <cstdlib>
#include <ctime>

using namespace std;

// Helper function for random int (FEATURE 15)
static int randomInt(int min, int max)
{
	static random_device rd;
	static mt19937 gen(rd());
	uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

// Constructor
Game::Game() :
	timerSeconds(120),
	currentLevel(1),
	goalAnimals(10),
	animalCount(0),
	animalCapacity(10),		// start with room for 10 animals
	productCount(0),
	productCapacity(20)
{
	// Allocate arrays (FEATURE 10)
	animals = new Animal * [animalCapacity];
	products = new Product * [productCapacity];

	pWind = CreateWind(config.windWidth, config.windHeight, config.wx, config.wy);
	createToolbar();
	createBudgetbar();

	// Keep Mohamed's visual background in the playing area.
	pWind->DrawImage("images\\background.jpeg", 0, config.toolBarHeight * 2,
		config.windWidth, config.windHeight);

	// Keep Mohamed's wolf/chicken entities and add them to Abdel's dynamic list.
	srand((unsigned int)time(0));
	point wolfPoint;
	wolfPoint.x = rand() % (range_max_x - range_min_x) + range_min_x;
	wolfPoint.y = rand() % (range_max_y - range_min_y) + range_min_y;
	myWolf = new Wolf(this, wolfPoint, 50, 40);
	addAnimal(myWolf);

	point chickenPoint;
	chickenPoint.x = 300;
	chickenPoint.y = 250;
	myChicken = new Chicken(this, chickenPoint, 55, 60);
	addAnimal(myChicken);

	clearStatusBar();
}

// Destructor – free all dynamic memory (Unit 3 rule)
Game::~Game()
{
	for (int i = 0; i < animalCount; i++)
		delete animals[i];
	delete[] animals;

	for (int i = 0; i < productCount; i++)
		delete products[i];
	delete[] products;

	delete gameToolbar;
	delete gameBudgetbar;
	delete pWind;
}

// ---------- Mouse / keyboard input ----------
clicktype Game::getMouseClick(int& x, int& y) const
{
	return pWind->WaitMouseClick(x, y);
}

string Game::getSrting() const
{
	return "";
}

window* Game::CreateWind(int w, int h, int x, int y) const
{
	window* pW = new window(w, h, x, y);
	pW->SetBrush(config.bkGrndColor);
	pW->SetPen(config.bkGrndColor, 1);
	pW->DrawRectangle(0, 0, w, h);
	return pW;
}

void Game::createToolbar()
{
	point pt; pt.x = 0; pt.y = 0;
	gameToolbar = new Toolbar(this, pt, 0, config.toolBarHeight);
	gameToolbar->draw();
}

void Game::createBudgetbar()
{
	point pt; pt.x = 0; pt.y = config.toolBarHeight;
	gameBudgetbar = new Budgetbar(this, pt, 0, config.toolBarHeight);
	gameBudgetbar->draw();
}

void Game::clearBudget() const
{
	pWind->SetPen(config.bkGrndColor, 1);
	pWind->SetBrush(config.bkGrndColor);
	pWind->DrawRectangle(config.windWidth - 500, config.toolBarHeight,
		config.windWidth, 2 * config.toolBarHeight);
}

void Game::printBudget(string msg) const
{
	clearBudget();
	pWind->SetPen(config.penColor, 50);
	pWind->SetFont(24, BOLD, BY_NAME, "Arial");
	pWind->DrawString(config.windWidth - 200, config.toolBarHeight + 10, msg);
}

void Game::clearStatusBar() const
{
	pWind->SetPen(config.statusBarColor, 1);
	pWind->SetBrush(config.statusBarColor);
	pWind->DrawRectangle(0, config.windHeight - config.statusBarHeight,
		config.windWidth, config.windHeight);
}

// ==================== FEATURE 1: drawStatusBar ====================
void Game::drawStatusBar() const
{
	clearStatusBar();
	window* w = pWind;
	w->SetPen(WHITE, 1);
	w->SetFont(20, BOLD, SWISS, "Arial");

	string timeMsg = "Time: " + to_string(timerSeconds) + "s";
	string levelMsg = "Level: " + to_string(currentLevel);
	string goalMsg = "Goal: " + to_string(goalAnimals) + " animals";
	string countMsg = "Animals: " + to_string(animalCount);

	int colWidth = config.windWidth / 4;
	int textY = config.windHeight - (int)(0.85 * config.statusBarHeight);

	w->DrawString(0 * colWidth + 10, textY, timeMsg);
	w->DrawString(1 * colWidth + 10, textY, levelMsg);
	w->DrawString(2 * colWidth + 10, textY, goalMsg);
	w->DrawString(3 * colWidth + 10, textY, countMsg);
}
// =================================================================

void Game::printMessage(string msg) const
{
	clearStatusBar();
	pWind->SetPen(config.penColor, 50);
	pWind->SetFont(24, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, config.windHeight - (int)(0.85 * config.statusBarHeight), msg);
}

window* Game::getWind() const { return pWind; }

// ==================== FEATURE 10 (part): Add animal/product ====================
void Game::addAnimal(Animal* a)
{
	if (animalCount >= animalCapacity)
	{
		// Expand array (Unit 3 example)
		int newCap = animalCapacity * 2;
		Animal** newArr = new Animal * [newCap];
		for (int i = 0; i < animalCount; i++)
			newArr[i] = animals[i];
		delete[] animals;
		animals = newArr;
		animalCapacity = newCap;
	}
	animals[animalCount++] = a;
}

void Game::addProduct(Product* p)
{
	if (productCount >= productCapacity)
	{
		int newCap = productCapacity * 2;
		Product** newArr = new Product * [newCap];
		for (int i = 0; i < productCount; i++)
			newArr[i] = products[i];
		delete[] products;
		products = newArr;
		productCapacity = newCap;
	}
	products[productCount++] = p;
}
// =============================================================================

// ==================== FEATURE 10 & 12 & 15: The Game Loop ====================
void Game::go()
{
	// Change window title
	pWind->ChangeTitle("- - - - - - - - - - Farm Frenzy (CIE101-project) - - - - - - - - - -");

	// Enable double buffering to avoid flicker
	pWind->SetBuffering(true);

	bool isExit = false;
	bool gameOver = false;
	int frameDelay = 30;			// about 33 FPS

	// We'll use the existing ElapsedTime function (auxil.h) for 1-second ticks
	// It returns true every time the interval has passed

	while (!isExit && !gameOver)
	{
		// ---- 1. Process input (non-blocking) ----
		int x, y;
		clicktype c = pWind->GetMouseClick(x, y);	// returns NO_CLICK if empty
		if (c != NO_CLICK)
		{
			if (y >= 0 && y < config.toolBarHeight)
			{
				isExit = gameToolbar->handleClick(x, y);
			}
			else if (y >= config.toolBarHeight && y < 2 * config.toolBarHeight)
			{
				isExit = gameBudgetbar->handleClick(x, y);
			}
			else if (y >= 2 * config.toolBarHeight && y < config.windHeight - config.statusBarHeight)
			{
				// Playing area clicked – you can later add grass planting, wolf attacking, etc.
				// For now, do nothing.
			}
		}

		// ---- 2. Update timer (FEATURE 12) ----
		if (ElapsedTime(1000))		// true once per second
		{
			if (timerSeconds > 0)
				timerSeconds--;
			else
				gameOver = true;	// time's up
		}

		// ---- 3. Update animal positions (FEATURE 15) ----
		for (int i = 0; i < animalCount; i++)
		{
			animals[i]->moveStep();
		}

		// ---- 4. Check goal (FEATURE 1 update) ----
		// (The animalCount is already updated when animals are added/removed)
		// If you want to trigger level up when animalCount >= goalAnimals, add it here.

		// ---- 5. Redraw everything ----
		// Preserve Mohamed's visual theme (background image).
		pWind->DrawImage("images\\background.jpeg", 0, config.toolBarHeight * 2,
			config.windWidth, config.windHeight);

		// Redraw toolbar and budget bar (they may have updated text)
		gameToolbar->draw();
		gameBudgetbar->draw();

		// Draw all animals
		for (int i = 0; i < animalCount; i++)
			animals[i]->draw();

		// Draw all products (FEATURE 7)
		for (int i = 0; i < productCount; i++)
			products[i]->draw();

		// Draw status bar
		drawStatusBar();

		// Update the screen (double buffer swap)
		pWind->UpdateBuffer();

		// Small pause to cap frame rate
		Pause(frameDelay);
	}

	// Game Over message
	if (gameOver)
	{
		pWind->SetBuffering(false);		// turn off buffering for final message
		printMessage("Game Over! Click to exit.");
		pWind->SetBuffering(true);
		int dummyX, dummyY;
		pWind->WaitMouseClick(dummyX, dummyY);
	}
}

void Game::go() const
{
	// Backward-compatibility overload for older object files that
	// still reference the const-qualified signature.
	const_cast<Game*>(this)->go();
}