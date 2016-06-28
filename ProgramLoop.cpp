/*
 * This is ProgramLoop
 * It is called on a timer from Main.
 * It oordinates the program delegates any tasks not part of the an active game,
 * includeing maintaining the local program's clock, and calling updates to output.
 * It also calls GameLoops if a game is active.
 */

// includes
#include "UngroupedHeader.h"
#include "InterfaceHeader.h"

Interface theinterface;

SysInfo sysinfo;

void ProgramLoop()
{

  // update the program clock
  sysinfo.updateIteration();

  theinterface.update();

  return;

} // end ProgramLoop()
