/*
 *   InputObjectHeader.h
 *
 *   This program establishes a class to simplify and organize input recieved through the windows API
 *
 *   Incorporating this class requires adding InputObjectHeader.h and InputObject.cpp to the project
 *   placing: extern InputObject inobj; in the file where the class will be used
 *   and calling:
 *     if (inobj.processwindowsmessage(hwnd, msg, wParam, lParam))
 *       return DefWindowProc(hwnd, msg, wParam, lParam);
 *   in the window procedure for each windows message it needs to process
 *
 *   Adding possible inputs messages for this class to reciece is a matter of
 *   1) adding the name of the input message to the InputMessage enumeration (not first or last in enum)
 *   2) adding to the processwindowsmessage function the conditions that should send this message
 *   and 3) performing whatever action you need to when it is returned by the getinput function
 *
 *  Created on: 21/08/2015
 *      Author: Alex
 */

#ifndef INPUTOBJECTHEADER_H_
#define INPUTOBJECTHEADER_H_

#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <string>
using namespace std;

enum InputMessage {MOUSECLICK_LEFT = 0,MOUSECLICK_RIGHT, KEY_G_CTRL, KEY_1, KEY_2, KEY_3, KEY_4, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_5, KEY_6, KEY_P, NOMOREMESSAGES};
// a list of every possible input retrieveable by getinput()
// NOMOREMESSAGES is for use by functions and arrays, and must contain both the largest value
// and the total number of (other) InputMessage values
// the first value must be set equal to zero, and other values must not be set

enum MouseButton {MOUSELEFT = 0, MOUSERIGHT, MOUSEMIDDLE};

class InputObject
{
private:
  bool haskeyboardfocus; // keeps track of if the program has keyboard focus
  bool messagestoprocess[NOMOREMESSAGES]; // keeps track of messages to be processed
  bool isprocessingastring; // for disabling other key functionality when typing
  int keycodetoend; // records what key ends typing, saved as a virtual key code, default is ESC
  bool keytoendbeenpressed; // remembers if the ending key for typing has been pressed
  string currentstring; // for keeping track of strings being entered
  int mousex, mousey; // the mouse's x and y
  RECT windowinfo; // used for checking the effective mouse location is on the screen
  int leftmousewentdownx, leftmousewentdowny; // for dragging boxes
  int rightmousewentdownx, rightmousewentdowny;
  int middlemousewentdownx, middlemousewentdowny;

public:
  InputObject();

  InputMessage getinput(); // returns the next InputMessage to be processed
  // or NOMOREMESSAGES if there are none
  bool isdown(int keycode); // the prarmeter is a virtual key code, as defined by windows
  // (changing all of them seemed like a lot of work for minimal gain)

  void getmousexy(int& xtoset, int& ytoset); // sets its parameters to the x and y of the mouse

  void getboxarea(MouseButton whichbutton,int& left, int& top, int& right, int& bottom); // for dragging boxes
  // getboxarea is inaccurate unless called promptly upon the release of the mouse button
  void getmousewentdownxy(MouseButton whichbutton, int& x, int& y);

  void beginprocessstring(); // disables keys for non-typing messages
  void beginprocessstring(int newkeycodetoend); // as above, now includes a key to indicate finished typing
  bool getstringsofar(string& returnstring); // reurns if finishing key hes been pressed, and updates currentstring
  void endprocessstring(string& returnstring); // ends string processing and updates currentstring

  bool processwindowsmessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); // returns true if it makes use of the message

}; // end class InputObject



#endif /* INPUTOBJECTHEADER_H_ */
