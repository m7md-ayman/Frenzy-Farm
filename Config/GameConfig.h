#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"

__declspec(selectany)

struct
{
	int	windWidth = 1200, windHeight = 600,
		wx = 5, wy = 5,
		toolBarHeight = 50,
		toolbarItemWidth = 40,
		statusBarHeight = 50;

	int playingAreaHeight = windHeight - toolBarHeight - statusBarHeight;

	color	penColor = BLUE,
		bkGrndColor = POWDERBLUE,
		statusBarColor = BLACK;
	int penWidth = 3;

	int	iconWidth = 70;
	int wolfCounter = 0;
	int chickenCounter = 0;
	int eggCount = 0;
	int milkCount = 0;

	// Production timers
	int chickenProductionFrames = 150;
	int cowProductionFrames = 350;
	int passiveProductionPerFrame = 1;
	int grassProductionBonus = 4;

	int chickGrowthFrames = 300;

	int wolfEatDistance = 40;

	// Level goals
	int level1GoalBudget = 2500;
	int level2GoalBudget = 4000;
	int level3GoalBudget = 6000;
	int level1Time = 120;
	int level2Time = 100;
	int level3Time = 80;

	int startBudget = 2000;
	int chickCost = 100;
	int cowCost = 100;
	int waterCost = 50;
	int eggSellPrice = 150;
	int milkSellPrice = 100;

	// Grass / food
	int foodAreaWidth = 80;
	int foodAreaHeight = 60;
	int foodAreaInitialCounter = 50;
	int foodPerBite = 1;

	// Wolf spawn
	int wolfSpawnIntervalSec = 45;
	int wolfBaseSpeed = 8;
	int wolfClicksToDestroy = 5;

	int warehouseCapacity = 99;
} config;
