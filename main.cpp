/*
 *  Rising Factions
 *
 *      Created on: 30/07/2015
 *          Author: Alex Russell
 *
 *  This is intended to eventulally become a real-time empire simulation game.
 *
 */

// includes
#include "UngroupedHeader.h"
#include "DrawToWindowHeader.h"
#include "InputObjectHeader.h"
#include "GameDataHeader.h"
#include "InterfaceHeader.h"
#include "Debugging.h"

using namespace std;

extern DrawToWindow dtw; // declare the drawtowindow object in this scope
extern InputObject inobj;
extern Interface theinterface;

int windowWidth = 800, windowHeight = 600; // the dimensions of the screen
int returnvaluecheckingint; // Am I really doing this? I think I am.

// a name for our the main window's window class
const char windowClassName[] = "myWindowClass";

// an identifier for our main timer
const unsigned int timerID = 1;
const int timerFrequency = 200; // the timer's frequency of being called in milliseconds


// The Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
      case WM_SETFOCUS:
      case WM_KILLFOCUS:
      case WM_KEYDOWN:
      case WM_KEYUP:
      case WM_MOUSEMOVE:
      case WM_CAPTURECHANGED:
      case WM_LBUTTONDOWN:
      case WM_LBUTTONDBLCLK:
      case WM_LBUTTONUP:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONDBLCLK:
      case WM_RBUTTONUP:
      case WM_MBUTTONDOWN:
      case WM_MBUTTONDBLCLK:
      case WM_MBUTTONUP:
        if (DISABLE_INPUT == FALSE)
        {
          if (/*DEBUG == TRUE and*/ DISPLAY_DISMESANDCALLS == TRUE)
            cout << "calling processwindowsmessage on inobj. Message: " << msg << "\n";

          if (inobj.processwindowsmessage(hwnd, msg, wParam, lParam))
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
      break;




        case WM_PAINT:
          if (DISABLE_PAINT == FALSE)
          {
            if (DEBUG == TRUE and DISPLAY_DISMESANDCALLS == TRUE)
              cout << "calling paint on dtw\n";

            return dtw.Paint();
          }
          else
            return 0;

        break;
        case WM_TIMER:
          if (DISABLE_TIMER == FALSE)
          {
            if (DEBUG == TRUE and DISPLAY_DISMESANDCALLS == TRUE)
              cout << "WM_TIMER message\n";
              switch(wParam)
              {
                case(timerID):
                    if (DEBUG == TRUE and DISPLAY_DISMESANDCALLS == TRUE)
                      cout << "calling programloop\n";

                    ProgramLoop();
                break;
              }
          }
        break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            KillTimer(hwnd,timerID);
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    // Registering the Window Class

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc; // The window procedure for this window class.
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // Cursor that will be displayed over our window.
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // Background Brush to set the color of our window.
    wc.lpszMenuName  = NULL; // Name of a menu resource to use for the windows with this class.
    wc.lpszClassName = windowClassName; // Name to identify the class with.
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)) // Register the window class
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Creating the Window
    hwnd = CreateWindowEx(0, windowClassName,"Rising Factions", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth + 16, windowHeight + 38, NULL, NULL, hInstance, NULL);

    // check return value to see if window creation failed
    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // display the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    if (DEBUG == TRUE)
      cout << "dtw object initialization beginning\n";

    // initialize the global objects
    dtw.Initialize(hwnd);
    theinterface.initialize(hwnd);

    if (DEBUG == TRUE)
      cout << "dtw object initialization passed\n";

    //creating the main timer
    returnvaluecheckingint = SetTimer(hwnd, timerID, timerFrequency, (TIMERPROC) NULL);
    if (DEBUG == TRUE)
      cout << "settimer returned: " << returnvaluecheckingint << "\n";
    if(returnvaluecheckingint == 0)
    {
        MessageBox(NULL, "Timer Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // The Message Loop
    while(TRUE)
    {
        returnvaluecheckingint = PeekMessage(&Msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);

        if (returnvaluecheckingint != 0)
        {
          if (DEBUG == TRUE and DISPLAY_DISMESANDCALLS == TRUE)
            cout << "Dispatching Timer Message: " << Msg.message << "\n";

          TranslateMessage(&Msg);
          DispatchMessage(&Msg);
        }

        while(GetMessage(&Msg, NULL, 0, 0) > 0)
        {
            if (DEBUG == TRUE and DISPLAY_DISMESANDCALLS == TRUE)
            {
              if (Msg.message == 15)
                cout << "Dispatching Paint Message: " << Msg.message << "\n";
              else
                cout << "Dispatching Non-Paint Message: " << Msg.message << "\n";
            }

            TranslateMessage(&Msg);
            DispatchMessage(&Msg);

            if (Msg.message == WM_PAINT)
              break;
        }

        if (Msg.message == WM_QUIT)
          break;
    }


    return Msg.wParam;
} // end WinMain


