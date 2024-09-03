
#include "framework.h"
#include "RitualOfTheRose.h"

#define MAX_LOADSTRING 100


HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HWND window;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    Args.parse_args(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RITUALOFTHEROSE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RITUALOFTHEROSE));

    MSG msg;

    graphics.setup(&window, hInstance);
    game.setup();
    controller.setup(&window);

    HICON theIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    SendMessage(window, WM_SETICON, ICON_BIG, (LPARAM)theIcon);

    while (!game.quit())
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (CLOCK.hasEnoughTimePassed("FPS", 0)) {
            game.run();
            graphics.OnRender();
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance){
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RITUALOFTHEROSE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RITUALOFTHEROSE));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){
   hInst = hInstance;
   int titlebar_gap = 38;

   window = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_THICKFRAME,
      CW_USEDEFAULT, 0, explorer.resolution.first, explorer.resolution.second + titlebar_gap, nullptr, nullptr, hInstance, nullptr);
   graphics.hwnd = &window;

   if (!window)
   {
      return FALSE;
   }

   ShowWindow(window, nCmdShow);
   UpdateWindow(window);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    controller.acceptAllInput(message, wParam, lParam);
    switch (message){
    case WM_SETCURSOR: {
        HCURSOR hCursor = graphics.Cursors[graphics.CurrentCursor];
        SetCursor(hCursor);
        break;
    }
    case WM_COMMAND:{
            int wmId = LOWORD(wParam);
            switch (wmId){
                case IDM_EXIT: {
                    game.stateFlags["QUIT"] = "1";
                    DestroyWindow(hWnd);
                    break;
                }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
    case WM_DESTROY: {
        game.stateFlags["QUIT"] = "1";
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}