#include "Game.h"
#include "../Config/GameConfig.h"
#include "../CMUgraphicsLib/auxil.h"
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

static int randomInt(int min, int max)
{
	static random_device rd;
	static mt19937 gen(rd());
	uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

Game::Game() :
	timerSeconds(config.level1Time),
	currentLevel(1),
	goalBudget(config.level1GoalBudget),
	animalCount(0),
	animalCapacity(10),
	productCount(0),
	productCapacity(20),
	foodCount(0),
	foodCapacity(10),
	isPaused(false),
	frameCount(0),
	secondsSinceWolfSpawn(0),
	myWolf(nullptr),
	myChicken(nullptr),
	myWarehouse(nullptr)
{
	budget = config.startBudget;

	animals = new Animal * [animalCapacity];
	products = new Product * [productCapacity];
	foodAreas = new FoodArea * [foodCapacity];

	pWind = CreateWind(config.windWidth, config.windHeight, config.wx, config.wy);
	createToolbar();
	createBudgetbar();

	pWind->DrawImage("images\\background.jpeg", 0, config.toolBarHeight * 2,
		config.windWidth, config.windHeight);

	point warehousePoint;
	warehousePoint.x = 500;
	warehousePoint.y = 380;
	myWarehouse = new Warehouse(this, warehousePoint, 140, 120);

	srand((unsigned int)time(0));

	// Spawn wolf
	point wolfPoint;
	wolfPoint.x = randomInt(50, config.windWidth - 100);
	wolfPoint.y = randomInt(config.toolBarHeight * 2 + 50, config.windHeight - config.statusBarHeight - 100);
	myWolf = new Wolf(this, wolfPoint, 50, 40, config.wolfBaseSpeed + currentLevel);
	addAnimal(myWolf);

	// Spawn chicken
	point chickenPoint;
	chickenPoint.x = 300;
	chickenPoint.y = 250;
	myChicken = new Chicken(this, chickenPoint, 55, 60);
	addAnimal(myChicken);

	clearStatusBar();
}

Game::~Game()
{
	for (int i = 0; i < animalCount; i++)
		delete animals[i];
	delete[] animals;

	for (int i = 0; i < productCount; i++)
		delete products[i];
	delete[] products;

	for (int i = 0; i < foodCount; i++)
		delete foodAreas[i];
	delete[] foodAreas;

	delete gameToolbar;
	delete gameBudgetbar;
	delete pWind;
	delete myWarehouse;
}

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

void Game::drawStatusBar() const
{
	clearStatusBar();
	window* w = pWind;
	w->SetPen(WHITE, 1);
	w->SetFont(20, BOLD, SWISS, "Arial");

	string timeMsg = "Time: " + to_string(timerSeconds) + "s";
	string levelMsg = "Level: " + to_string(currentLevel);
	string goalMsg = "Goal: $" + to_string(budget) + "/$" + to_string(goalBudget);
	string countMsg = "Animals: " + to_string(animalCount);
	string pauseMsg = isPaused ? " [PAUSED]" : "";

	int colWidth = config.windWidth / 5;
	int textY = config.windHeight - (int)(0.85 * config.statusBarHeight);

	w->DrawString(0 * colWidth + 10, textY, timeMsg);
	w->DrawString(1 * colWidth + 10, textY, levelMsg);
	w->DrawString(2 * colWidth + 10, textY, goalMsg);
	w->DrawString(3 * colWidth + 10, textY, countMsg);
	w->DrawString(4 * colWidth + 10, textY, pauseMsg);
}

void Game::printMessage(string msg) const
{
	clearStatusBar();
	pWind->SetPen(config.penColor, 50);
	pWind->SetFont(24, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, config.windHeight - (int)(0.85 * config.statusBarHeight), msg);
}

window* Game::getWind() const { return pWind; }


void Game::addAnimal(Animal* a)
{
	if (animalCount >= animalCapacity)
	{
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

void Game::removeAnimal(int index)
{
	if (index < 0 || index >= animalCount) return;

	Animal* removing = animals[index];
	if (myWolf && removing == static_cast<Animal*>(myWolf))
		myWolf = nullptr;
	if (myChicken && removing == static_cast<Animal*>(myChicken))
		myChicken = nullptr;

	delete removing;
	for (int i = index; i < animalCount - 1; i++)
		animals[i] = animals[i + 1];
	animalCount--;
}

void Game::removeProduct(int index)
{
	if (index < 0 || index >= productCount) return;

	delete products[index];
	for (int i = index; i < productCount - 1; i++)
		products[i] = products[i + 1];
	productCount--;
}

void Game::addFoodArea(FoodArea* f)
{
	if (foodCount >= foodCapacity)
	{
		int newCap = foodCapacity * 2;
		FoodArea** newArr = new FoodArea * [newCap];
		for (int i = 0; i < foodCount; i++)
			newArr[i] = foodAreas[i];
		delete[] foodAreas;
		foodAreas = newArr;
		foodCapacity = newCap;
	}
	foodAreas[foodCount++] = f;
}

void Game::removeFoodArea(int index)
{
	if (index < 0 || index >= foodCount) return;
	delete foodAreas[index];
	for (int i = index; i < foodCount - 1; i++)
		foodAreas[i] = foodAreas[i + 1];
	foodCount--;
}

void Game::removeDepletedFoodAreas()
{
	for (int i = foodCount - 1; i >= 0; i--)
	{
		if (foodAreas[i]->isDepleted())
			removeFoodArea(i);
	}
}

void Game::drawFieldBoundaries() const
{
	int top = config.toolBarHeight * 2;
	int bottom = config.windHeight - config.statusBarHeight;
	pWind->SetPen(DARKGREEN, 3);
	pWind->DrawRectangle(2, top + 2, config.windWidth - 2, bottom - 2, FRAME);
}

//PAUSE / RESUME

void Game::pauseGame()
{
	isPaused = true;
}

void Game::resumeGame()
{
	isPaused = false;
}

//RESTART

void Game::restartGame()
{
	myWolf = nullptr;
	myChicken = nullptr;

	for (int i = 0; i < animalCount; i++)
		delete animals[i];
	animalCount = 0;

	for (int i = 0; i < productCount; i++)
		delete products[i];
	productCount = 0;

	budget = config.startBudget;
	currentLevel = 1;
	timerSeconds = config.level1Time;
	goalBudget = config.level1GoalBudget;
	isPaused = false;
	frameCount = 0;
	secondsSinceWolfSpawn = 0;
	config.wolfCounter = 0;
	config.chickenCounter = 0;
	config.eggCount = 0;
	config.milkCount = 0;

	for (int i = 0; i < foodCount; i++)
		delete foodAreas[i];
	foodCount = 0;

	point wolfPoint;
	wolfPoint.x = randomInt(50, config.windWidth - 100);
	wolfPoint.y = randomInt(config.toolBarHeight * 2 + 50, config.windHeight - config.statusBarHeight - 100);
	myWolf = new Wolf(this, wolfPoint, 50, 40, config.wolfBaseSpeed + currentLevel);
	addAnimal(myWolf);

	point chickenPoint;
	chickenPoint.x = 300;
	chickenPoint.y = 250;
	myChicken = new Chicken(this, chickenPoint, 55, 60);
	addAnimal(myChicken);
}

//SAVE GAME

void Game::saveGame(string filename)
{
	ofstream fout(filename);
	if (!fout.is_open())
	{
		printMessage("Error: Could not save game!");
		return;
	}

	fout << budget << endl;
	fout << timerSeconds << endl;
	fout << currentLevel << endl;
	fout << goalBudget << endl;
	fout << config.eggCount << endl;
	fout << config.milkCount << endl;

	fout << animalCount << endl;
	for (int i = 0; i < animalCount; i++)
	{
		fout << animals[i]->getType() << " "
			<< animals[i]->getX() << " "
			<< animals[i]->getY() << endl;
	}

	fout << foodCount << endl;
	for (int i = 0; i < foodCount; i++)
	{
		fout << foodAreas[i]->getX() << " "
			<< foodAreas[i]->getY() << " "
			<< foodAreas[i]->getFoodCounter() << endl;
	}

	fout << productCount << endl;
	for (int i = 0; i < productCount; i++)
	{
		int ptype = products[i]->isEgg() ? 1 : 2;
		fout << ptype << " " << products[i]->getX() << " " << products[i]->getY() << endl;
	}

	fout.close();
	printMessage("Game saved successfully!");
}

//LOAD GAME

void Game::loadGame(string filename)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		printMessage("Error: No save file found!");
		return;
	}

	myWolf = nullptr;
	myChicken = nullptr;

	for (int i = 0; i < animalCount; i++)
		delete animals[i];
	animalCount = 0;
	for (int i = 0; i < productCount; i++)
		delete products[i];
	productCount = 0;

	config.wolfCounter = 0;
	config.chickenCounter = 0;

	fin >> budget;
	fin >> timerSeconds;
	fin >> currentLevel;
	fin >> goalBudget;
	fin >> config.eggCount;
	fin >> config.milkCount;

	int savedAnimalCount;
	fin >> savedAnimalCount;

	for (int i = 0; i < savedAnimalCount; i++)
	{
		int type, ax, ay;
		fin >> type >> ax >> ay;
		point p; p.x = ax; p.y = ay;

		Animal* a = nullptr;
		switch (type)
		{
		case ANIMAL_CHICK:
			a = new Chick(this, p, 50, 50, "images\\chick.jpg");
			break;
		case ANIMAL_COW:
			a = new Cow(this, p, 50, 50, "images\\cow.jpg");
			break;
		case ANIMAL_CHICKEN:
			a = new Chicken(this, p, 55, 60);
			if (!myChicken) myChicken = (Chicken*)a;
			break;
		case ANIMAL_WOLF:
		{
			int wolfSpeed = config.wolfBaseSpeed + currentLevel;
			a = new Wolf(this, p, 50, 40, wolfSpeed);
			if (!myWolf) myWolf = (Wolf*)a;
			break;
		}
		}
		if (a) addAnimal(a);
	}

	int savedFoodCount = 0;
	if (fin >> savedFoodCount)
	{
		for (int i = 0; i < savedFoodCount; i++)
		{
			int fx, fy, fcnt;
			if (!(fin >> fx >> fy >> fcnt))
				break;
			point fp; fp.x = fx; fp.y = fy;
			addFoodArea(new FoodArea(this, fp, config.foodAreaWidth, config.foodAreaHeight, fcnt));
		}
	}

	int savedProductCount = 0;
	if (fin >> savedProductCount)
	{
		for (int i = 0; i < savedProductCount; i++)
		{
			int ptype, px, py;
			if (!(fin >> ptype >> px >> py))
				break;
			point pp; pp.x = px; pp.y = py;
			if (ptype == 1)
				addProduct(new Egg(this, pp));
			else
				addProduct(new Milk(this, pp));
		}
	}

	fin.close();
	isPaused = false;
	frameCount = 0;
	printMessage("Game loaded successfully!");
}

//WOLF EATS ANIMALS

void Game::checkWolfCollisions()
{
	for (int w = 0; w < animalCount; w++)
	{
		if (animals[w]->getType() != ANIMAL_WOLF) continue;

		int wx = animals[w]->getX();
		int wy = animals[w]->getY();

		for (int j = 0; j < animalCount; j++)
		{
			if (j == w) continue;
			if (animals[j]->getType() == ANIMAL_WOLF) continue;

			int ax = animals[j]->getX();
			int ay = animals[j]->getY();

			int dx = wx - ax;
			int dy = wy - ay;
			int distSq = dx * dx + dy * dy;
			int eatDist = config.wolfEatDistance;

			if (distSq < eatDist * eatDist)
			{
				removeAnimal(j);
				if (j < w) w--;
				j--;
			}
		}
	}
}

void Game::spawnProductAtAnimal(Animal* a, bool isEgg)
{
	(void)a;
	if (isEgg)
	{
		if (config.eggCount < config.warehouseCapacity)
		{
			config.eggCount++;
			printMessage("Egg sent to warehouse!");
		}
	}
	else
	{
		if (config.milkCount < config.warehouseCapacity)
		{
			config.milkCount++;
			printMessage("Milk sent to warehouse!");
		}
	}
}

//LEVEL PROGRESSION

int Game::getGoalBudgetForLevel(int level) const
{
	switch (level)
	{
	case 1: return config.level1GoalBudget;
	case 2: return config.level2GoalBudget;
	case 3: return config.level3GoalBudget;
	default: return config.level3GoalBudget;
	}
}

int Game::getTimeForLevel(int level) const
{
	switch (level)
	{
	case 1: return config.level1Time;
	case 2: return config.level2Time;
	case 3: return config.level3Time;
	default: return config.level3Time;
	}
}

void Game::checkLevelUp()
{
	if (budget < goalBudget)
		return;

	if (currentLevel >= 3)
		return;

	currentLevel++;
	goalBudget = getGoalBudgetForLevel(currentLevel);
	timerSeconds = getTimeForLevel(currentLevel);
	secondsSinceWolfSpawn = 0;

	point wolfPoint;
	wolfPoint.x = randomInt(50, config.windWidth - 100);
	wolfPoint.y = randomInt(config.toolBarHeight * 2 + 50, config.windHeight - config.statusBarHeight - 100);
	int wolfSpeed = config.wolfBaseSpeed + currentLevel;
	addAnimal(new Wolf(this, wolfPoint, 50, 40, wolfSpeed));

	printMessage("LEVEL UP! Level " + to_string(currentLevel) + " - Reach $" + to_string(goalBudget));
}


void Game::checkChickGrowth()
{
	for (int i = 0; i < animalCount; i++)
	{
		if (animals[i]->getType() == ANIMAL_CHICK)
		{
			Chick* chick = (Chick*)animals[i];
			if (chick->getAge() >= config.chickGrowthFrames)
			{
				point p;
				p.x = chick->getX();
				p.y = chick->getY();

				delete animals[i];

				Chicken* newChicken = new Chicken(this, p, 55, 60);
				animals[i] = newChicken; 
			}
		}
	}
}

void Game::checkFoodCollisions()
{
	for (int i = 0; i < animalCount; i++)
	{
		if (animals[i]->getType() != ANIMAL_WOLF)
			animals[i]->setOnGrass(false);
	}

	for (int f = 0; f < foodCount; f++)
	{
		if (foodAreas[f]->isDepleted())
			continue;

		for (int i = 0; i < animalCount; i++)
		{
			Animal* a = animals[i];
			AnimalType t = a->getType();
			if (t != ANIMAL_CHICKEN && t != ANIMAL_COW)
				continue;

			if (foodAreas[f]->overlaps(a->getX(), a->getY(), a->getW(), a->getH()))
			{
				a->setOnGrass(true);
				foodAreas[f]->consumeFood(config.foodPerBite);
				a->addProductionProgress(config.grassProductionBonus);
			}
		}
	}
	removeDepletedFoodAreas();
}

static void tryCompleteProduction(Game* game, Animal* a)
{
	int maxF = a->getProductionMaxFrames();
	if (maxF <= 0 || a->getProductionCounter() < maxF)
		return;

	AnimalType t = a->getType();
	if (t == ANIMAL_CHICKEN)
		game->spawnProductAtAnimal(a, true);
	else if (t == ANIMAL_COW)
		game->spawnProductAtAnimal(a, false);

	a->resetProductionCounter();
}

void Game::updateWolfSpawns()
{
	secondsSinceWolfSpawn++;
	int interval = config.wolfSpawnIntervalSec - (currentLevel * 5);
	if (interval < 15)
		interval = 15;

	if (secondsSinceWolfSpawn >= interval)
	{
		secondsSinceWolfSpawn = 0;
		point wolfPoint;
		wolfPoint.x = randomInt(50, config.windWidth - 100);
		wolfPoint.y = randomInt(config.toolBarHeight * 2 + 50, config.windHeight - config.statusBarHeight - 100);
		int wolfSpeed = config.wolfBaseSpeed + currentLevel;
		addAnimal(new Wolf(this, wolfPoint, 50, 40, wolfSpeed));
	}
}

void Game::handleProductClick(int x, int y)
{
	for (int i = 0; i < productCount; i++)
	{
		if (!products[i]->containsPoint(x, y))
			continue;

		if (products[i]->isEgg())
		{
			if (config.eggCount < config.warehouseCapacity)
				config.eggCount++;
		}
		else
		{
			if (config.milkCount < config.warehouseCapacity)
				config.milkCount++;
		}
		removeProduct(i);
		printMessage("Product collected!");
		return;
	}
}

void Game::handleWolfClick(int x, int y)
{
	for (int i = 0; i < animalCount; i++)
	{
		if (animals[i]->getType() != ANIMAL_WOLF)
			continue;

		int ax = animals[i]->getX();
		int ay = animals[i]->getY();
		int aw = animals[i]->getW();
		int ah = animals[i]->getH();
		if (x < ax || x > ax + aw || y < ay || y > ay + ah)
			continue;

		Wolf* w = (Wolf*)animals[i];
		w->registerClick();
		int hits = w->getClickCount();
		if (hits >= config.wolfClicksToDestroy)
		{
			removeAnimal(i);
			printMessage("Wolf defeated!");
		}
		else
		{
			printMessage("Wolf hit " + to_string(hits) + "/" + to_string(config.wolfClicksToDestroy));
		}
		return;
	}
}

void Game::handlePlayingAreaClick(int x, int y)
{
	if (budget < config.waterCost)
		return;

	budget -= config.waterCost;
	point p;
	p.x = x - config.foodAreaWidth / 2;
	p.y = y - config.foodAreaHeight / 2;
	addFoodArea(new FoodArea(this, p, config.foodAreaWidth, config.foodAreaHeight, config.foodAreaInitialCounter));
}

//WIN / GAME OVER SCREENS

void Game::showWinScreen()
{
	pWind->SetBuffering(false);

	pWind->SetPen(DARKGREEN, 1);
	pWind->SetBrush(DARKGREEN);
	pWind->DrawRectangle(config.windWidth / 4, config.windHeight / 4,
		3 * config.windWidth / 4, 3 * config.windHeight / 4);

	pWind->SetPen(GOLD, 4);
	pWind->DrawRectangle(config.windWidth / 4 + 5, config.windHeight / 4 + 5,
		3 * config.windWidth / 4 - 5, 3 * config.windHeight / 4 - 5, FRAME);

	pWind->SetPen(WHITE, 1);
	pWind->SetFont(40, BOLD, BY_NAME, "Arial");
	pWind->DrawString(config.windWidth / 2 - 120, config.windHeight / 2 - 60, "YOU WIN!");

	pWind->SetFont(20, BOLD, BY_NAME, "Arial");
	pWind->DrawString(config.windWidth / 2 - 140, config.windHeight / 2,
		"All levels completed! Budget: $" + to_string(budget));

	pWind->SetFont(16, PLAIN, BY_NAME, "Arial");
	pWind->DrawString(config.windWidth / 2 - 80, config.windHeight / 2 + 40, "Click to exit.");

	int dx, dy;
	pWind->WaitMouseClick(dx, dy);
}

void Game::showGameOverScreen()
{
	pWind->SetBuffering(false);

	pWind->SetPen(DARKRED, 1);
	pWind->SetBrush(DARKRED);
	pWind->DrawRectangle(config.windWidth / 4, config.windHeight / 4,
		3 * config.windWidth / 4, 3 * config.windHeight / 4);

	pWind->SetPen(RED, 4);
	pWind->DrawRectangle(config.windWidth / 4 + 5, config.windHeight / 4 + 5,
		3 * config.windWidth / 4 - 5, 3 * config.windHeight / 4 - 5, FRAME);

	pWind->SetPen(WHITE, 1);
	pWind->SetFont(40, BOLD, BY_NAME, "Arial");
	pWind->DrawString(config.windWidth / 2 - 140, config.windHeight / 2 - 60, "GAME OVER");

	pWind->SetFont(20, BOLD, BY_NAME, "Arial");
	pWind->DrawString(config.windWidth / 2 - 160, config.windHeight / 2,
		"Time's up! Level: " + to_string(currentLevel) + "  Budget: $" + to_string(budget));

	pWind->SetFont(16, PLAIN, BY_NAME, "Arial");
	pWind->DrawString(config.windWidth / 2 - 80, config.windHeight / 2 + 40, "Click to exit.");

	int dx, dy;
	pWind->WaitMouseClick(dx, dy);
}


void Game::go()
{
	// Change window title
	pWind->ChangeTitle("- - - - - - - - - - Farm Frenzy (CIE101-project) - - - - - - - - - -");

	// Enable double buffering to avoid flicker
	pWind->SetBuffering(true);

	bool isExit = false;
	bool gameOver = false;
	bool gameWon = false;
	int frameDelay = 20;

	while (!isExit && !gameOver && !gameWon)
	{
		int x = 0, y = 0;
		clicktype c = NO_CLICK;
		while (true)
		{
			int tx, ty;
			clicktype t = pWind->GetMouseClick(tx, ty);
			if (t == NO_CLICK)
				break;
			x = tx;
			y = ty;
			c = t;
		}

		if (c == LEFT_CLICK)
		{
			if (y >= 0 && y < config.toolBarHeight)
			{
				isExit = gameToolbar->handleClick(x, y);
			}
			else if (y >= config.toolBarHeight && y < 2 * config.toolBarHeight)
			{
				isExit = gameBudgetbar->handleClick(x, y);
			}
			else if (myWarehouse && myWarehouse->isClicked(x, y))
			{
				myWarehouse->onClick();
			}
			else if (y >= config.toolBarHeight * 2 && y < config.windHeight - config.statusBarHeight)
			{
				handleWolfClick(x, y);
				handleProductClick(x, y);
			}
		}

		if (!isPaused)
		{
			if (ElapsedTime(1000))
			{
				if (timerSeconds > 0)
					timerSeconds--;
				else
					gameOver = true;
				updateWolfSpawns();
			}

			frameCount++;

			for (int i = 0; i < animalCount; i++)
				animals[i]->moveStep();

			checkFoodCollisions();

			for (int i = 0; i < animalCount; i++)
			{
				AnimalType t = animals[i]->getType();
				if (t == ANIMAL_CHICKEN || t == ANIMAL_COW)
					animals[i]->addProductionProgress(config.passiveProductionPerFrame);
			}

			for (int i = 0; i < animalCount; i++)
				tryCompleteProduction(this, animals[i]);

			checkWolfCollisions();
			checkChickGrowth();
			checkLevelUp();

			if (currentLevel >= 3 && budget >= goalBudget)
				gameWon = true;
		}
	
		pWind->DrawImage("images\\background.jpeg", 0, config.toolBarHeight * 2,
			config.windWidth, config.windHeight);

		gameToolbar->draw();
		gameBudgetbar->draw();

		drawFieldBoundaries();

		for (int i = 0; i < foodCount; i++)
			foodAreas[i]->draw();

		if (myWarehouse) myWarehouse->draw();

		for (int i = 0; i < animalCount; i++)
			animals[i]->draw();

		for (int i = 0; i < productCount; i++)
			products[i]->draw();

		drawStatusBar();
		pWind->UpdateBuffer();

		Pause(frameDelay);
	}

	// End screens
	if (gameWon)
	{
		showWinScreen();
	}
	else if (gameOver)
	{
		showGameOverScreen();
	}
}

void Game::go() const
{
	const_cast<Game*>(this)->go();
}