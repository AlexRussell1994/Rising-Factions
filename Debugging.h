/*
 * Debugging.h
 *
 *  Created on: 1/09/2015
 *      Author: Alex
 */

#ifndef DEBUGGING_H_
#define DEBUGGING_H_

#include <iostream>
using namespace std;

// all messages
const bool DEBUG = true;

// big stuff
const bool DISABLE_PAINT = false;
const bool DISABLE_INPUT = false;
const bool DISABLE_TIMER = false;

// little stuff
const bool DISABLE_MOUSE_CAPTURE = true; // seems to be causing the unresponsive border
const bool DISABLE_FACTIONPERSONUSE = false;
const bool DISABLE_WALKINGCOMPONENTS = true;

// specific messages
const bool DISPLAY_DISMESANDCALLS = false; // short for display dispatch messages and function calls (from window procedure) debugging notifications
const bool DISPLAY_DTW_PERIODIC_HWND_USED = false; // drawtowindow, what hwnd it got its DC from

#endif /* DEBUGGING_H_ */
