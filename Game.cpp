#include "Game.h"

using namespace std;

vector<vector<Cell>> cells;
int n;
HMENU hMenu, hSizeMenu;
Gdiplus::Bitmap *Image = nullptr;
int idTimer = -1;
boolean needToRedraw = true;

const SIZE MIN_WINDOW_SIZE = SIZE{500, 500};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_COMMAND: {
            switch (wParam) {
                case GAME_MENU_NEW_CLASSIC: {
                    OnNewClassicGame(hWnd);
                    break;
                }
                case GAME_MENU_NEW_IMAGE: {
                    OnNewImageGame(hWnd);
                    break;
                }
                case GAME_MENU_SAVE: {
                    OnSave(hWnd);
                    break;
                }
                case GAME_MENU_LOAD: {
                    OnLoad(hWnd);
                    break;
                }
                case GAME_SIZE_THREE:{
                    ResizeGameField(hWnd, 3);
                    break;
                }
                case GAME_SIZE_FOUR:{
                    ResizeGameField(hWnd, 4);
                    break;
                }
                case GAME_SIZE_FIVE:{
                    ResizeGameField(hWnd, 5);
                    break;
                }
                case GAME_SIZE_SIX:{
                    ResizeGameField(hWnd, 6);
                    break;
                }
                case GAME_SIZE_SEVEN:{
                    ResizeGameField(hWnd, 7);
                    break;
                }
            }
            HWND buttonClicked = (HWND)lParam;
            CheckMove(hWnd, buttonClicked);
            break;
        }
        case WM_DRAWITEM:
        {
            CutImage(lParam);
            break;
        }
        case WM_TIMER: {
            if (needToRedraw){
                Resize(hWnd);
            }
        }
        case WM_ERASEBKGND:
            return (LRESULT)1;
        case WM_CREATE: {
            Initialize(hWnd);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_SIZE: {
            needToRedraw = true;
            break;
        }
        case WM_GETMINMAXINFO: {
            LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
            lpMMI->ptMinTrackSize.x = MIN_WINDOW_SIZE.cx;
            lpMMI->ptMinTrackSize.y = MIN_WINDOW_SIZE.cy;
            break;
        }
        default: {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }
    return 0;
}

Scale GetWindowSteps(HWND hWnd){
    Scale result = {0};
    RECT rect;
    GetWindowRect(hWnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top - 55;

    result.x = width / n;
    result.y = height / n;

    return result;
}

int getInversionsCount(){
    int count = 0;
    for (int i = 0; i < n * n - 1; i++){
        for (int j = i + 1; j < n * n; j++){
            if (cells[j/n][j%n].id && cells[i/n][i%n].id && cells[i/n][i%n].id > cells[j/n][j%n].id)
                count++;
        }
    }
    return count;
}

int findEmptyPosition(){
   for (int i = n - 1; i >= 0; i--)
        for (int j = n - 1; j >= 0; j--)
            if (cells[i][j].id == 0)
                return n - i;
    return 0;
}

boolean CheckSolution(){
    int invCount = getInversionsCount();

    if (n & 1)
        return !(invCount & 1);
    else {
        int pos = findEmptyPosition();
        if (pos & 1)
            return !(invCount & 1);
        else
            return invCount & 1;
    }
}

void RandomizeButtons(){
    int col1, col2, row1, row2;
    do {
        for (int i = 0; i < 100; i++) {
            col1 = rand() % n;
            col2 = rand() % n;
            row1 = rand() % n;
            row2 = rand() % n;
            std::swap(cells[row1][col1], cells[row2][col2]);
        }
    }while (!CheckSolution());
}

void DrawGameField(HWND hWnd, Scale steps){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            MoveWindow(cells[i][j].btn, j * steps.x, i * steps.y, steps.x, steps.y, 0);
        }
    }
    InvalidateRect(hWnd, nullptr, false);
}

void Resize(HWND hWnd){
    if (!cells.empty()){
        Scale steps = GetWindowSteps(hWnd);
        DrawGameField(hWnd, steps);
        InvalidateRect(hWnd, nullptr, true);
        needToRedraw = false;
    }
}

void CreateImageGameField(HWND hWnd, Scale steps, int emptyI, int emptyJ){
    int id = 1;
    cells.resize(n);
    for(int i = 0; i < n; i++){
        cells[i].resize(n);
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cells[i][j].btn = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, steps.x * j, steps.y * i, steps.x, steps.y, hWnd, nullptr, nullptr, nullptr);
            if( (i != emptyI) || (j != emptyJ)){
                cells[i][j].id = id++;
                cells[i][j].isEmpty = false;
                cells[i][j].bitmap = Image->Clone(steps.x * j, steps.y * i, steps.x, steps.y, PixelFormat24bppRGB);
            } else {
                cells[i][j].isEmpty = true;
                cells[i][j].id = 0;
            }
        }
    }
    RandomizeButtons();
}

void CreateClassicGameField(HWND hWnd, Scale steps, int emptyI, int emptyJ){
    int id = 1;
    cells.resize(n);
    for(int i = 0; i < n; i++){
        cells[i].resize(n);
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cells[i][j].btn = CreateWindowW(L"Button", nullptr, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, steps.x * j, steps.y * i, steps.x, steps.y, hWnd, nullptr, nullptr, nullptr);
            if( (i != emptyI) || (j != emptyJ)){
                cells[i][j].id = id++;
                cells[i][j].isEmpty = false;
                wchar_t text[3] = L"";
                swprintf_s(text, L"%d", cells[i][j].id);
                SetWindowText(cells[i][j].btn, text);
            } else {
                cells[i][j].id = 0;
                cells[i][j].isEmpty = true;
                wchar_t text[3] = L"";
                SetWindowText(cells[i][j].btn, text);
            }
        }
    }
    RandomizeButtons();
}

void ClearGameField(){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            DestroyWindow(cells[i][j].btn);
        }
    }
}

boolean CheckWinSituation(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if ((cells[i][j].id != i * n + j + 1) && (cells[i][j].id != 0)){
                return false;
            }
        }
    }
    return true;
}

void CheckMove(HWND hWnd, HWND buttonClicked){
    boolean swapped = false;
    int i = 0;
    int j = 0;
    while (i < n && !swapped) {
        j = 0;
        while (j < n && !swapped) {
            if (buttonClicked == cells[i][j].btn){
                int emptyI;
                int emptyJ;

                for(int k = 0; k < n; k++){
                    for(int q = 0; q < n; q++){
                        if (cells[k][q].isEmpty){
                            emptyI = k;
                            emptyJ = q;
                        }
                    }
                }

                Scale steps = GetWindowSteps(hWnd);
                float delta = sqrt((i - emptyI) * (i - emptyI) + (j - emptyJ) * (j - emptyJ));
                if (delta <= 1.01){
                    swapped = true;
                    std::swap(cells[i][j], cells[emptyI][emptyJ]);
                    MoveWindow(cells[i][j].btn, j * steps.x, i * steps.y, steps.x, steps.y, 1);
                    MoveWindow(cells[emptyI][emptyJ].btn, emptyJ * steps.x, emptyI * steps.y, steps.x, steps.y, 1);

                    RECT rect;
                    GetClientRect(cells[i][j].btn, &rect);
                    const RECT *rectPointer = &rect;
                    InvalidateRect(hWnd, rectPointer, false);
                    GetClientRect(cells[emptyI][emptyJ].btn, &rect);
                    rectPointer = &rect;
                    InvalidateRect(hWnd, rectPointer, false);

                    if (CheckWinSituation()){
                        if (MessageBoxW(hWnd, L"Congratulations! You won", L"Start new game?", MB_YESNO | MB_ICONQUESTION) == IDYES){
                            ClearGameField();
                            Image == nullptr ? CreateClassicGameField(hWnd, steps, n - 1, n - 1) : CreateImageGameField(hWnd, steps, n - 1, n - 1);
                            DrawGameField(hWnd, steps);
                        }
                    }
                }
            }
            j++;
        }
        i++;
    }
}

void ScaleImage(HWND hWnd, Gdiplus::Bitmap *bitmap){
    RECT rect;
    GetWindowRect(hWnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top - MENU_HEIGHT;

    float horizontalScalingFactor = (float) width / (float) bitmap->GetWidth();
    float verticalScalingFactor = (float) height / (float) bitmap->GetHeight();

    Image = new Gdiplus::Bitmap(width, height);
    Image->SetResolution(bitmap->GetHorizontalResolution(), bitmap->GetVerticalResolution());
    Gdiplus::Graphics g(Image);
    g.ScaleTransform(horizontalScalingFactor, verticalScalingFactor);
    g.DrawImage(bitmap, 0, 0);
}

void OnNewClassicGame(HWND hWnd){
    if (MessageBoxW(hWnd, L"Your progress will be lost?", L"Start new game", MB_YESNO | MB_ICONQUESTION) == IDNO)
        return;
    ClearGameField();
    Image = nullptr;
    n = 4;
    Scale steps = GetWindowSteps(hWnd);

    CreateClassicGameField(hWnd, steps, n - 1, n - 1);
    DrawGameField(hWnd, steps);
}

void OnNewImageGame(HWND hWnd){
    if (MessageBoxW(hWnd, L"Your progress will be lost?", L"Start new game", MB_YESNO | MB_ICONQUESTION) == IDNO)
        return;
    wchar_t buffer[256];
    GetCurrentDirectoryW(256, buffer);
    std::wstring name = OpenFile(hWnd);
    SetCurrentDirectoryW(buffer);
    Gdiplus::Bitmap bitmap(name.c_str());

    if(bitmap.GetLastStatus() != Gdiplus::Ok){
        MessageBoxW(nullptr, L"Can't create bitmap", L"Attention", MB_OK);
        return;
    }

    ScaleImage(hWnd, &bitmap);

    ClearGameField();
    n = 4;
    Scale steps = GetWindowSteps(hWnd);

    cells.resize(n);
    for(int i = 0; i < n; i++){
        cells[i].resize(n);
    }
    CreateImageGameField(hWnd, steps, n - 1, n - 1);
    DrawGameField(hWnd, steps);
}


void OnSave(HWND hWnd){
    if(Image != nullptr){
        CLSID pngClsid;
        CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);
        Image->Save(L"save.png", &pngClsid, nullptr);
    } else {
        wchar_t buffer[] = L"save.png";
        wchar_t *lpStr;
        lpStr = buffer;

        if(PathFileExistsW(lpStr))
            DeleteFileW(lpStr);
    }

    std::fstream fout("save.dat", std::ios::out | std::ios::binary);
    if (!fout)
    {
        MessageBoxW(hWnd, L"Can't open file for reading.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }
    if (MessageBoxW(hWnd, L"Last save will be deleted", L"Are you sure?", MB_YESNO | MB_ICONQUESTION) == IDNO)
        return;

    fout << n << ' ';

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (cells[i][j].isEmpty)
                fout << i << ' ' << j << ' ';


    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            fout << cells[i][j].id << ' ';

    fout.close();
    MessageBoxW(hWnd, L"Game saved.", L"Success!", MB_OK | MB_ICONASTERISK);
}

void RestoreImageGame(HWND hWnd, Scale steps){
    int x0, y0;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            y0 = cells[i][j].id / n;
            x0 = cells[i][j].id % n;
            cells[i][j].btn = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, steps.x * x0, steps.y * y0, steps.x, steps.y, hWnd, nullptr, nullptr, nullptr);
            if( !cells[i][j].isEmpty ) {
                cells[i][j].bitmap = Image->Clone(steps.x * x0, steps.y * y0, steps.x, steps.y, PixelFormat32bppRGB);
            }
        }
    }
}

void RestoreClassicGame(HWND hWnd, Scale steps){
    int x0, y0;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            y0 = (cells[i][j].id - 1) / n;
            x0 = (cells[i][j].id - 1) % n;
            cells[i][j].btn = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, steps.x * x0, steps.y * y0, steps.x, steps.y, hWnd, nullptr, nullptr, nullptr);
            if( !cells[i][j].isEmpty ){
                wchar_t text[3] = L"";
                swprintf_s(text, L"%d", cells[i][j].id);
                SetWindowText(cells[i][j].btn, text);
            } else {
                wchar_t text[3] = L"";
                SetWindowText(cells[i][j].btn, text);
            }
        }
    }
}

void OnLoad(HWND hWnd){
    boolean isImageOpen = false;
    wchar_t buffer[] = L"save.png";
    wchar_t *lpStr;
    lpStr = buffer;

    std::fstream fin("save.dat", std::ios::in | std::ios::binary);
    if (MessageBoxW(hWnd, L"Load another game?", L"Are you sure?", MB_YESNO | MB_ICONQUESTION) == IDNO)
        return;
    if (!fin){
        MessageBoxW(hWnd, L"No loadings available.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    if(PathFileExistsW(lpStr)){
        Gdiplus::Bitmap bitmap(L"save.png");
        isImageOpen = true;
        ScaleImage(hWnd, &bitmap);
    }

    ClearGameField();

    cells.clear();

    fin >> n;
    cells.resize(n);
    for(int i = 0; i < n; i++){
        cells[i].resize(n);
    }

    Scale steps  = GetWindowSteps(hWnd);

    int emptyI, emptyJ;
    fin >> emptyI;
    fin >> emptyJ;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            fin >> cells[i][j].id;
            cells[i][j].isEmpty = false;
        }
    }

    cells[emptyI][emptyJ].isEmpty= true;

    fin.close();

    isImageOpen ? RestoreImageGame(hWnd, steps) : RestoreClassicGame(hWnd, steps);
    DrawGameField(hWnd, steps);
}

void ResizeGameField(HWND hWnd, int size){
    if (n != size) {
        ClearGameField();
        n = size;
        Scale steps = GetWindowSteps(hWnd);
        Gdiplus::Bitmap *bitmap = Image->Clone();
        ScaleImage(hWnd, bitmap);
        Image == nullptr ? CreateClassicGameField(hWnd, steps, n - 1, n - 1) : CreateImageGameField(hWnd, steps, n - 1, n - 1);
        DrawGameField(hWnd, steps);
    }
}

void CutImage(LPARAM lParam){
    auto lpdrawstLogon = (LPDRAWITEMSTRUCT) lParam;
    PAINTSTRUCT ps;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if ((lpdrawstLogon->hwndItem == cells[i][j].btn) && !cells[i][j].isEmpty)
            {
                HDC memDC = CreateCompatibleDC(lpdrawstLogon->hDC);
                RECT rc = lpdrawstLogon->rcItem;

                HBITMAP bmp = CreateCompatibleBitmap(lpdrawstLogon->hDC, rc.right - rc.left,rc.bottom - rc.top);
                auto oldBmp = (HBITMAP) SelectObject(memDC, bmp);

                Gdiplus::Graphics(memDC).DrawImage(cells[i][j].bitmap, Gdiplus::Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top));

                BitBlt(lpdrawstLogon->hDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, memDC, 0, 0, SRCCOPY);

                SelectObject(memDC, oldBmp);
                DeleteObject(bmp);
                DeleteDC(memDC);
            }
        }
    }
}

std::wstring OpenFile(HWND hWnd){
    OPENFILENAME ofn;

    wchar_t path[256];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = path;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 256;
    ofn.lpstrFilter = L"Images\0*.jpg;*.jpeg;*.png;*.bmp\0";
    ofn.nFilterIndex = 1;

    if (GetOpenFileName(&ofn) == FALSE){
        MessageBoxW(nullptr, L"Error opening image", L"Attention", MB_OK);
    }

    return ofn.lpstrFile;
}

void AddMenus(HWND hWnd){
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "Game");
    AppendMenuA(hFileMenu, MF_STRING, GAME_MENU_NEW_CLASSIC, "New Classic");
    AppendMenuA(hFileMenu, MF_STRING, GAME_MENU_NEW_IMAGE, "New With Image");
    AppendMenuA(hFileMenu, MF_STRING, GAME_MENU_SAVE, "Save");
    AppendMenuA(hFileMenu, MF_STRING, GAME_MENU_LOAD, "Load");
    AppendMenuA(hFileMenu, MF_SEPARATOR, (UINT_PTR)nullptr, nullptr);
    AppendMenuA(hFileMenu, MF_STRING, GAME_MENU_EXIT, "Exit");
    hSizeMenu = CreateMenu();
    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hSizeMenu, "Size");
    AppendMenuA(hSizeMenu, MF_STRING, GAME_SIZE_THREE, "3 * 3");
    AppendMenuA(hSizeMenu, MF_STRING, GAME_SIZE_FOUR, "4 * 4");
    AppendMenuA(hSizeMenu, MF_STRING, GAME_SIZE_FIVE, "5 * 5");
    AppendMenuA(hSizeMenu, MF_STRING, GAME_SIZE_SIX, "6 * 6");
    AppendMenuA(hSizeMenu, MF_STRING, GAME_SIZE_SEVEN, "7 * 7");
    SetMenu(hWnd, hMenu);
}

void Initialize(HWND hWnd){
    srand(time(nullptr));
    SetTimer(hWnd, idTimer = 1, 1, nullptr);
    AddMenus(hWnd);

    n = 4;
    Scale steps = GetWindowSteps(hWnd);
    CreateClassicGameField(hWnd, steps, n - 1, n - 1);
    DrawGameField(hWnd, steps);
}