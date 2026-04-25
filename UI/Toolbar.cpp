#include "Toolbar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

ToolbarIcon::ToolbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    image_path = img_path;
}

void ToolbarIcon::draw() const
{
    window* pWind = pGame->getWind();
    pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

// --- RestartIcon ---
RestartIcon::RestartIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void RestartIcon::onClick()
{
    // TODO: add restart logic
    cout << "Restart clicked" << endl;
}

// --- PauseIcon ---
PauseIcon::PauseIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void PauseIcon::onClick()
{
    // TODO: add pause logic
    cout << "Pause clicked" << endl;
}

// --- ResumeIcon ---
ResumeIcon::ResumeIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void ResumeIcon::onClick()
{
    // TODO: add resume logic
    cout << "Resume clicked" << endl;
}

// --- SaveIcon ---
SaveIcon::SaveIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void SaveIcon::onClick()
{
    // TODO: add save logic
    cout << "Save clicked" << endl;
}

// --- LoadToolbarIcon ---
LoadToolbarIcon::LoadToolbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void LoadToolbarIcon::onClick()
{
    // TODO: add load logic
    cout << "Load clicked" << endl;
}

// --- ExitIcon ---
ExitIcon::ExitIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path) {
}

void ExitIcon::onClick()
{
    // TODO: add cleanup logic
}

// --- Toolbar ---
Toolbar::Toolbar(Game* r_pGame, point r_point, int r_width, int r_height)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    iconsImages[ICON_RESTART] = "images\\RESTART.jpeg";
    iconsImages[ICON_PAUSE] = "images\\PAUSE.jpeg";
    iconsImages[ICON_RESUME] = "images\\RESUME.jpeg";
    iconsImages[ICON_SAVE] = "images\\SAVE.jpeg";
    iconsImages[ICON_LOAD] = "images\\LOAD.jpeg";
    iconsImages[ICON_EXIT] = "images\\EXIT.jpeg";

    iconsList = new ToolbarIcon * [ICON_COUNT];

    point p;
    p.x = 0;
    p.y = 0;

    iconsList[ICON_RESTART] = new RestartIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_RESTART]);
    p.x += config.iconWidth;
    iconsList[ICON_PAUSE] = new PauseIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_PAUSE]);
    p.x += config.iconWidth;
    iconsList[ICON_RESUME] = new ResumeIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_RESUME]);
    p.x += config.iconWidth;
    iconsList[ICON_SAVE] = new SaveIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_SAVE]);
    p.x += config.iconWidth;
    iconsList[ICON_LOAD] = new LoadToolbarIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_LOAD]);
    p.x += config.iconWidth;
    iconsList[ICON_EXIT] = new ExitIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_EXIT]);
}

Toolbar::~Toolbar()
{
    for (int i = 0; i < ICON_COUNT; i++)
        delete iconsList[i];
    delete[] iconsList;
}

void Toolbar::draw() const
{
    for (int i = 0; i < ICON_COUNT; i++)
        iconsList[i]->draw();

    window* pWind = pGame->getWind();
    pWind->SetPen(BLACK, 3);
    pWind->DrawLine(0, config.toolBarHeight, pWind->GetWidth(), config.toolBarHeight);
}

bool Toolbar::handleClick(int x, int y)
{
    if (x >= ICON_COUNT * config.iconWidth)
        return false;

    int clickedIconIndex = x / config.iconWidth;
    iconsList[clickedIconIndex]->onClick();

    if (clickedIconIndex == ICON_EXIT) return true;

    return false;
}