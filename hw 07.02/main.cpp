#include "Windows.h"
#include "windowsx.h"
#include "tchar.h"
#include "string"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool movingWindow = false;
int speed = 5;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    WCHAR className[] = L"FirstWindow";
    WNDCLASSEX wndClass;
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wndClass.hInstance = hInstance;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hIconSm = NULL;
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = className;
    wndClass.lpfnWndProc = WindowProc;

    if (!RegisterClassEx(&wndClass)) {
        return 0;
    }

    HWND hWnd = CreateWindowEx(
        0,
        className,
        L"My first window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        600, 600, 
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    SetTimer(hWnd, 1, 50, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

  LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
      switch (uMsg)
      {
      case WM_DESTROY:
          PostQuitMessage(0);
          break;
      case WM_KEYDOWN:
          if (wParam == VK_RETURN) {
              RECT rect;
              GetWindowRect(hwnd, &rect);
              int width = 300;
              int height = 300;
              MoveWindow(hwnd, 0, 0, width, height, TRUE);
              movingWindow = true;
          }
          else if (wParam == VK_ESCAPE) {
              movingWindow = false;
          }
          break;
    case WM_TIMER:
        if (movingWindow) {
            RECT rect;
            GetWindowRect(hwnd, &rect);
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);

            static int direction = 0;
            switch (direction) {
            case 0: 
                if (rect.right + speed <= screenWidth) {
                    rect.left += speed;
                    rect.right += speed;
                }
                else {
                    direction = 1; 
                }
                break;
            case 1:
                if (rect.bottom + speed <= screenHeight) {
                    rect.top += speed;
                    rect.bottom += speed;
                }
                else {
                    direction = 2;
                }
                break;
            case 2:
                if (rect.left - speed >= 0) {
                    rect.left -= speed;
                    rect.right -= speed;
                }
                else {
                    direction = 3; 
                }
                break;
            case 3:
                if (rect.top - speed >= 0) {
                    rect.top -= speed;
                    rect.bottom -= speed;
                }
                else {
                    direction = 0; 
                }
                break;
            }

            MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        }
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}