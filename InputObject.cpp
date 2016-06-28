/*
 *  See InputObjectHeader.h's Top-of-the-File Comment
 *
 *  Created on: 21/08/2015
 *      Author: Alex
 */

#include "InputObjectHeader.h"
#include "Debugging.h"

InputObject inobj;

// a constructor, just to initialize stuff to 0
InputObject::InputObject()
{
  haskeyboardfocus = (GetFocus == NULL) ? FALSE : TRUE;
  for(int i = 0; i <= NOMOREMESSAGES;i++)
  {
    messagestoprocess[i] = FALSE;
  }
  isprocessingastring = FALSE;
  currentstring = "";
  keycodetoend = VK_ESCAPE;
  keytoendbeenpressed = FALSE;
  mousex = 0;
  mousey = 0;
  leftmousewentdownx = 0;
  leftmousewentdowny = 0;
  rightmousewentdownx = 0;
  rightmousewentdowny = 0;
  middlemousewentdownx = 0;
  middlemousewentdowny = 0;

} // end constructor

InputMessage InputObject::getinput()
{
  for(int i = 0; i < NOMOREMESSAGES;i++)
  {
    if (messagestoprocess[i] == TRUE)
    {
      messagestoprocess[i] = FALSE;
      return (InputMessage)i;
    }
  }
  return NOMOREMESSAGES;
} // end getinput

// the prarmeter is a virtual key code, as defined by windows
// (changing all of them seemed like a lot of work for minimal gain)
bool InputObject::isdown(int keycode)
{
  if (haskeyboardfocus == FALSE)
    return FALSE;

  return (GetAsyncKeyState(keycode) & ~1);
} // end isdown


void InputObject::getmousexy(int& xtoset, int& ytoset)
{
  xtoset = mousex;
  ytoset = mousey;
} // end getmousexy

void InputObject::getboxarea(MouseButton whichbutton,int& left, int& top, int& right, int& bottom)
{
  switch (whichbutton)
  {
    case MOUSELEFT:
      left = (mousex < leftmousewentdownx) ? mousex : leftmousewentdownx;
      right = (mousex > leftmousewentdownx) ? mousex : leftmousewentdownx;
      top = (mousey < leftmousewentdowny) ? mousey : leftmousewentdowny;
      bottom = (mousey > leftmousewentdowny) ? mousey : leftmousewentdowny;
    break;
    case MOUSERIGHT:
      left = (mousex < rightmousewentdownx) ? mousex : rightmousewentdownx;
      right = (mousex > rightmousewentdownx) ? mousex : rightmousewentdownx;
      top = (mousey < rightmousewentdowny) ? mousey : rightmousewentdowny;
      bottom = (mousey > rightmousewentdowny) ? mousey : rightmousewentdowny;
    break;
    case MOUSEMIDDLE:
      left = (mousex < middlemousewentdownx) ? mousex : middlemousewentdownx;
      right = (mousex > middlemousewentdownx) ? mousex : middlemousewentdownx;
      top = (mousey < middlemousewentdowny) ? mousey : middlemousewentdowny;
      bottom = (mousey > middlemousewentdowny) ? mousey : middlemousewentdowny;
    break;
  }
} // end getboxarea

void InputObject::getmousewentdownxy(MouseButton whichbutton, int& x, int& y)
{
  switch (whichbutton)
   {
     case MOUSELEFT:
       x = leftmousewentdownx;
       y = leftmousewentdowny;
     break;
     case MOUSERIGHT:
       x = rightmousewentdownx;
       y = rightmousewentdowny;
     break;
     case MOUSEMIDDLE:
       x = middlemousewentdownx;
       y = middlemousewentdowny;
     break;
   }
} // end getmousewentdownxy

void InputObject::beginprocessstring()
{
  isprocessingastring = TRUE;
  currentstring = "";
  keycodetoend = VK_ESCAPE;
} // end beginprocessstring()

void InputObject::beginprocessstring(int newkeycodetoend)
{
  isprocessingastring = TRUE;
  currentstring = "";
  keycodetoend = newkeycodetoend;
} // end beginprocessstring(int)

bool InputObject::getstringsofar(string& returnstring)
{
  returnstring = currentstring;
  if (keytoendbeenpressed == TRUE)
  {
    isprocessingastring = FALSE;
    return TRUE;
  }
  return FALSE;
} // end getstringsofar

void InputObject::endprocessstring(string& returnstring)
{
  returnstring = currentstring;
  isprocessingastring = FALSE;
} // end endprocessstring

bool InputObject::processwindowsmessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  {
    case WM_SETFOCUS: // record the keyboard focus
      haskeyboardfocus = TRUE;
    break;
    case WM_KILLFOCUS:
      haskeyboardfocus = FALSE;
    break;

    case WM_KEYDOWN: // process keypresses
      if (isprocessingastring == TRUE and keytoendbeenpressed == FALSE) // if we are processing a string
      {
        if (wParam == keycodetoend) // if this is the key to end string processing
        {
          keytoendbeenpressed = TRUE;
        } else // if this is not the key to end string processing
        {
          if (wParam >= '0' and wParam <= '9') // if it is a number
          {
            currentstring += wParam; // add it to the current string
          } else if (wParam >= 'A' and wParam <= 'Z') // if it is a letter
          {
            if ((GetAsyncKeyState(VK_SHIFT)  & ~1) != 0) // if shift is down
            {
              currentstring += wParam; // add it to the current string (capital)
            } else // if shift is not down
            {
              currentstring += wParam + 32; // add it to the current string (non-capital)
            } // endif is shift down
          } // endif is it a number or a letter
        } // endif is this the key to end string processing
      } else // if not processing a string
      {
        if (wParam == 'G')
        {
          if (isdown(VK_CONTROL))
          {
            messagestoprocess[KEY_G_CTRL] = TRUE;
          }
        } else if (wParam == 'Q')
        {
          messagestoprocess[KEY_Q] = TRUE;
        } else if (wParam == 'W')
        {
          messagestoprocess[KEY_W] = TRUE;
        } else if (wParam == 'E')
        {
          messagestoprocess[KEY_E] = TRUE;
        } else if (wParam == 'R')
        {
          messagestoprocess[KEY_R] = TRUE;
        } else if (wParam == 'T')
        {
          messagestoprocess[KEY_T] = TRUE;
        } else if (wParam == '1')
        {
          messagestoprocess[KEY_1] = TRUE;
        } else if (wParam == '2')
        {
          messagestoprocess[KEY_2] = TRUE;
        } else if (wParam == '3')
        {
          messagestoprocess[KEY_3] = TRUE;
        } else if (wParam == '4')
        {
          messagestoprocess[KEY_4] = TRUE;
        } else if (wParam == '5')
        {
          messagestoprocess[KEY_5] = TRUE;
        } else if (wParam == '6')
        {
          messagestoprocess[KEY_6] = TRUE;
        } else if (wParam == 'P')
        {
          if (DEBUG == TRUE)
            cout << "Keypressed: P\n";
          messagestoprocess[KEY_P] = TRUE;
        }
      } // endif are we processing a string
    break;

    case WM_MOUSEMOVE:
      mousex = GET_X_LPARAM(lParam); // get the mouse location
      mousey = GET_Y_LPARAM(lParam);
      if (mousex < 0) {mousex = 0;} // treat the mouse like it doesn't pass the edges of the window
      if (mousey < 0) {mousey = 0;}
      GetWindowRect(hwnd,&windowinfo);
      if (mousex >= windowinfo.right) {mousex = windowinfo.right - 1;}
      if (mousey >= windowinfo.bottom) {mousey = windowinfo.bottom - 1;}
    break;

    case WM_LBUTTONDOWN: // process mouse clicks
    case WM_LBUTTONDBLCLK:
      if (DISABLE_MOUSE_CAPTURE == FALSE) {if (GetCapture() != hwnd) {SetCapture(hwnd);}}
      messagestoprocess[MOUSECLICK_LEFT] = TRUE;
      leftmousewentdownx = mousex;
      leftmousewentdowny = mousey;
    break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
      if (DISABLE_MOUSE_CAPTURE == FALSE) {if (GetCapture() != hwnd) {SetCapture(hwnd);}}
      messagestoprocess[MOUSECLICK_RIGHT] = TRUE;
      rightmousewentdownx = mousex;
      rightmousewentdowny = mousey;
    break;
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
      if (DISABLE_MOUSE_CAPTURE == FALSE) {if (GetCapture() != hwnd) {SetCapture(hwnd);}}
      middlemousewentdownx = mousex;
      middlemousewentdowny = mousey;
    break;
  }

  return FALSE; // do not pass this on to DefWindowProc
} // end processwindowsmessage

