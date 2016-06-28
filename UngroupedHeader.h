/*
 * Basically all of the declarations
 */

#ifndef DECLARATIONS_H_
#define DECLARATIONS_H_

// includes
#include <windows.h> // windows.h, obviously needed
#include <iostream> // it's iostream
#include <ctime> // needed for clock_t, clock(), and CLOCKS_PER_SEC

// exactly what it says
using namespace std;

// core function declarations
void ProgramLoop();

// cross-file classes:

// SysInfo handles the program's timer and random number generator
/*
 * (The main timer allows for a Windows Message to keep the program updating, it is not to be confused
 *  with the program's timer, which updates itself each ProgramLoop iteration and that the rest of
 *  the program judges how much time has actually passed based on.)
 */
class SysInfo
{
  private:
  clock_t oldtime;
  int timesincelast;

  public:
  SysInfo();
  int getTimeSinceLastIteration(){return timesincelast;} // returns the amount of time in milliseconds that has passed since PorgramLoop's last iteration
  void updateIteration(); // called once per ProgramLoop to update the timer
}; // end SysInfo

// Output handles all output
class Output
{
  private:
  static Output * outputobject; // points to the output object (there should only be one)
  HWND hwnd; // the handle to the main window





  public:
  Output(HWND hwnd);
  ~Output();

  static Output* getOutputObject(){return outputobject;} // allow access to the output object
  static void setOutputObject(Output* obj){outputobject = obj;} // called once when output object is made



}; // end Output




#endif /* DECLARATIONS_H_ */






