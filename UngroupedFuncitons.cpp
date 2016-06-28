/*
 * This is a collection of general program funcions, classes, etc.
 */

// includes
#include "UngroupedHeader.h"

// SysInfo section:

// constructor, here to initialize oldtime
SysInfo::SysInfo()
{
  oldtime = clock();
  timesincelast = 0;
} // end constructor

// called once per ProgramLoop to update the timer
void SysInfo::updateIteration()
{
  clock_t newtime = clock();
  timesincelast = (((double)(newtime - oldtime))*1000/(double)(CLOCKS_PER_SEC));
  oldtime = newtime;
  return;
} // end updateIteration()

// end SysInfo section

// Output section:
Output::Output(HWND newhwnd)
{
  hwnd = newhwnd; // save the handle of the main window


} // end constructor

Output::~Output()
{

} // end destructor
// end Output section
