#ifndef FIFTEEN_GAME_H
#define FIFTEEN_GAME_H

#include <windows.h>
#include <ctime>
#include <string>
#include <iostream>
#include <vector>
#include <gdiplus.h>
#include <cmath>
#include <fstream>
#include "Shlwapi.h"

#define GAME_MENU_NEW_CLASSIC 1
#define GAME_MENU_NEW_IMAGE 2
#define GAME_MENU_SAVE 3
#define GAME_MENU_LOAD 4
#define GAME_MENU_EXIT 5
#define GAME_SIZE_THREE 33
#define GAME_SIZE_FOUR 44
#define GAME_SIZE_FIVE 55
#define GAME_SIZE_SIX 66
#define GAME_SIZE_SEVEN 77

#define MENU_HEIGHT 55;

struct Cell{
    int id;
    HWND btn;
    boolean isEmpty;
    Gdiplus::Bitmap *bitmap;
};

struct Scale{
    int x;
    int y;
};

Scale GetWindowSteps(HWND);
boolean CheckSolution();
void RandomizeButtons();
void DrawGameField(HWND, Scale steps);
void Resize(HWND);
void CreateImageGameField(HWND, Scale, int, int);
void CreateClassicGameField(HWND, Scale, int, int);
void ClearGameField();
boolean CheckWinSituation();
void CheckMove(HWND, HWND);
void ScaleImage(HWND, Gdiplus::Bitmap*);
void OnNewClassicGame(HWND);
void OnNewImageGame(HWND);
void OnSave(HWND);
void OnLoad(HWND);
void RestoreImageGame(HWND, Scale);
void RestoreClassicGame(HWND, Scale);
void ResizeGameField(HWND, int);
void CutImage(LPARAM);
void DrawBitmap(HDC, RECT, Gdiplus::Image);
std::wstring OpenFile(HWND);
void AddMenus(HWND);
void Initialize(HWND);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

#endif
