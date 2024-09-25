// G1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "G1.h"
#include "world.h"
#include <ctime>
#include "gui.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND Window;
bool Exit = false;

chrono::high_resolution_clock timer;
chrono::steady_clock::time_point lastTime = timer.now();
double deltaTime;

bool isPaused = false;
world worldInstance;
gui guiInstance(vec(WINDOW_WIDTH / 2., WINDOW_HEIGHT / 2.));

HBRUSH guiBrush = CreateSolidBrush(RGB(150, 50, 50));
//
HBRUSH black = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));

vector<RECT*> rects;

void draw()
{
    HDC hdc = GetDC(Window);
    HDC buffHdc = CreateCompatibleDC(hdc);

    HBITMAP bitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
    HGDIOBJ render = SelectObject(buffHdc, bitmap);

    BitBlt(buffHdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc, 0, 0, SRCCOPY);

    SetTextAlign(buffHdc, TA_CENTER | TA_BASELINE);
    SetBkMode(buffHdc, TRANSPARENT);

    // Draw world
    for (RECT* rect : rects)
    {
        FillRect(buffHdc, rect, white);
        delete rect;
    }
    rects.clear();

    for (shape* sq : worldInstance.shapes)
    {
        RECT* rect = new RECT;

        rect->left = sq->center.x - sq->halfWidth;
        rect->top = sq->center.y - sq->halfHeight;
        rect->right = sq->center.x + sq->halfWidth;
        rect->bottom = sq->center.y + sq->halfHeight;
        FillRect(buffHdc, rect, black);

        rects.push_back(rect);
    }

    //Draw GUI
    if (guiInstance.isShown)
    {
        RECT* guiWin = new RECT;
        guiWin->left = guiInstance.position.x - guiInstance.halfWidth;
        guiWin->right = guiInstance.position.x + guiInstance.halfWidth;
        guiWin->top = guiInstance.position.y - guiInstance.halfHeight;
        guiWin->bottom = guiInstance.position.y + guiInstance.halfHeight;

        Rectangle(buffHdc, guiWin->left, guiWin->top, guiWin->right, guiWin->bottom);
        rects.push_back(guiWin);

        for (uielement* element : guiInstance.elements)
        {
            RECT* rect = new RECT;

            rect->left = element->position.x - element->halfWidth + guiWin->left;
            rect->top = element->position.y - element->halfHeight + guiWin->top;
            rect->right = element->position.x + element->halfWidth + guiWin->left;
            rect->bottom = element->position.y + element->halfHeight + guiWin->top;
            FillRect(buffHdc, rect, guiBrush);

            rects.push_back(rect);

            switch (element->GetType())
            {
            case UI_BUTTON:
                const char* text = ((button*)element)->title.c_str();
                size_t size = strlen(text) + 1;
                wchar_t* wtext = new wchar_t[size];
                size_t outSize;
                mbstowcs_s(&outSize, wtext, size, text, size - 1);

                TextOut(buffHdc, element->position.x + guiWin->left, element->position.y + guiWin->top, wtext, outSize);

                delete[] wtext;
                break;
            }
        }
    }

    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, buffHdc, 0, 0, SRCCOPY);

    DeleteObject(render);
    DeleteObject(bitmap);
    DeleteDC(buffHdc);
    DeleteDC(hdc);
}

void main_loop()
{
    if (Exit) return;

    if (!isPaused)
        worldInstance.Process(deltaTime);

    draw();

    deltaTime = chrono::duration<double>((timer.now() - lastTime)).count();
    lastTime = timer.now();
}

void init_world()
{
    worldInstance.CreateRandomSquare();
}

void start()
{
    srand(time(0));
    
    button* _continue = new button(vec(guiInstance.halfWidth, 70), BUTTON_WIDTH, BUTTON_HEIGHT, "Continue");
    _continue->action = []() { guiInstance.isShown = false; isPaused = false; };

    button* restart = new button(vec(guiInstance.halfWidth, 170), BUTTON_WIDTH, BUTTON_HEIGHT, "Restart");
    restart->action = []() { worldInstance.Dispose(); init_world(); };

    button* exit = new button(vec(guiInstance.halfWidth, 270), BUTTON_WIDTH, BUTTON_HEIGHT, "Close");
    exit->action = []() { Exit = true; PostQuitMessage(0); };

    guiInstance.RegisterElement(_continue);
    guiInstance.RegisterElement(restart);
    guiInstance.RegisterElement(exit);

    init_world();
}

void key_input()
{
    if (GetAsyncKeyState(VK_ESCAPE) & 0x01)
    {
        isPaused = !isPaused;
        guiInstance.isShown = !guiInstance.isShown;
    }
}

void mouse_input(int x, int y)
{
    guiInstance.Input(vec(x, y));
}










// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    start();

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_G1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_G1));

    MSG msg;

    // Цикл основного сообщения:
    while (!Exit)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }
        main_loop();
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_G1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_G1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   Window = CreateWindowW(szWindowClass, szTitle, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
      CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!Window)
   {
      return FALSE;
   }

   ShowWindow(Window, nCmdShow);
   UpdateWindow(Window);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYUP:
        key_input();
        break;
    case WM_LBUTTONUP:
        mouse_input(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                Exit = true;
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        Exit = true;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }   

    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}