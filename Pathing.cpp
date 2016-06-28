/*
 *  The functions for unit pathing
 *
 *  Created on: 24/08/2015
 *      Author: Alex
 */

#include "GameDataHeader.h"

#define roundnumber(x) (x==((double)((int)x)))?TRUE:FALSE

Map map; // declare the global map object

waypoint* unit::pathto(double x, double y)
{

  waypoint* basicwaypoint = new waypoint(currentx, currenty, size); // create a waypoint chain describing

  if (roundnumber(x) and roundnumber(y)) // a waypoint chain cannot end on an exact point
  { // between tiles, as this may allow a unit through an inpassible "checker grid" point
    x += 0.01;
    if (x > map.getwidth())
      x -= 0.02;
  }

  waypoint* newnextwaypoint = new waypoint(x, y, size); // where you want to go to and from

  // DEBUG
  /*delete newnextwaypoint;
  return basicwaypoint;*/

  basicwaypoint->insertnextwaypoint(newnextwaypoint);

  // DEBUG
  return basicwaypoint;

  routenode* firstroutenode = new routenode(basicwaypoint); // begin a routenode sequence for this path

  routenode* currentroutenode = firstroutenode; // steps through all opssible routes
  int lengthofshortestroute = -1;
  waypoint* shortestroute = NULL;
  while (currentroutenode != NULL) // while there are routes to try
  {
    if (currentroutenode->getlength() != -1)
    { // if this route didn't failed
      if (lengthofshortestroute == -1 or lengthofshortestroute > currentroutenode->getlength())
      { // if this is the new shortest route
        lengthofshortestroute = currentroutenode->getlength();
        shortestroute = currentroutenode->releasewaypointchain();
      } // endif this is the new shortest route
    } // endif this route didn't fail
    currentroutenode = currentroutenode->getnextroutenode(); // move to the next route
  } // end while there are routes to try

  delete firstroutenode; // delete the routenode chain
  // routes will delete their own waypoint chains unless you change their waypoint pointer to NULL first
  // which is done as part of releasewaypointchain()

  return shortestroute;
  // returns the first waypoint of the chain of waypoints representing the shortest route
  // if no route can be found, this returns NULL

} // end pathto

routenode::routenode(waypoint* thisnodeswaypoint)
{

  firstwaypoint = thisnodeswaypoint; // set list of waypoints for this route to try
  nextroutenode = NULL; // important 0-setting
  alreadychecked = NONE;
  startingpartwaythrough = NULL;
  hascalculatedlength = FALSE;
  length = -1; // getlength() returns this if the path fails

} // end constructor

routenode::routenode(waypoint* thisnodeswaypoint, waypoint* thewaypointbeingchecked, SquareSide directionalreadytried)
{

  firstwaypoint = thisnodeswaypoint; // set list of waypoints for this route to try
  nextroutenode = NULL; // important 0-setting
  alreadychecked = directionalreadytried; // mid-calculation info
  startingpartwaythrough = thewaypointbeingchecked; // mid-calculation info
  hascalculatedlength = FALSE; // important 0-setting
  length = -1; // getlength() returns this if the path fails

} // end constructor


routenode::~routenode()
{

  delete firstwaypoint; // destroy its waypoint chain
  delete nextroutenode; // delete the entire routenode chain

} // end destructor

waypoint* routenode::releasewaypointchain()
{
  waypoint* tempwayopointholder = firstwaypoint;
  firstwaypoint = NULL;
  return tempwayopointholder;
} // end releasewaypointchain

int routenode::getlength()
{
  if (hascalculatedlength == TRUE) // only calculate length once
    return length;

  // local variable creation and initialization
  waypoint* currentwaypoint = firstwaypoint;
  tile* tilehitfrom;
  SquareSide sidehitfrom;
  tile* tilehitto;
  SquareSide sidehitto;
  SquareSide directionattempted; // to hold the return from circumvent()
  waypoint* tempduplicatedwaypointlistholder = NULL;
  waypoint* waypointcounterorigonal = NULL;
  waypoint* waypointcounterduplicate = NULL;
  bool routehasnotfailed = TRUE;
  int returnvalue; // checks return value of trace call

  if (startingpartwaythrough != NULL)
    currentwaypoint = startingpartwaythrough;


  for (;;) // forever
  {

    // move past unobstructed paths
    while ((returnvalue = currentwaypoint->trace(&tilehitfrom, sidehitfrom, &tilehitto, sidehitto)) != 0)
    { // if reached next waypoint without incident
      if (returnvalue == -1)
        return length; // abandon infinite loop paths

      currentwaypoint = currentwaypoint->getnextwaypoint(); // goto next waypoint

      if (currentwaypoint->getnextwaypoint() == NULL)
      { // if reached the final waypoint
        length = firstwaypoint->sumroutelength(); // route was successfully found, return the length of the route
        hascalculatedlength = TRUE;
        return length;
      } // endif reached the final waypoint
    } // end while

    // known because the above while did not contunue, path is obstructed

    if (alreadychecked == NONE)
    { // if does not have to go a specific direction

      // duplicate the waypoint list for the new routenode
      tempduplicatedwaypointlistholder = firstwaypoint->duplicatewaypointlist(currentwaypoint);

      // find the equivalent of currentwaypoint on the new list
      waypointcounterorigonal = firstwaypoint;
      waypointcounterduplicate = tempduplicatedwaypointlistholder;
      while (waypointcounterorigonal != currentwaypoint)
      {
        waypointcounterorigonal =  waypointcounterorigonal->getnextwaypoint();
        waypointcounterduplicate = waypointcounterduplicate->getnextwaypoint();
      }

      // attempt to circumvent
      routehasnotfailed = currentwaypoint->circumvent(directionattempted,*tilehitfrom, sidehitfrom, *tilehitto, sidehitto);

      // add the unckecked route to the routenode sequence
      addroutetolist(tempduplicatedwaypointlistholder, waypointcounterduplicate, directionattempted);

    } else
    { // if does have to go a specific direction

      // attempt to circumvent
      routehasnotfailed = currentwaypoint->circumvent(*tilehitfrom, sidehitfrom, *tilehitto, sidehitto, alreadychecked);

      // does not have to go a specific direction next time
      alreadychecked = NONE;

    } // endif has to go a specific direction

    // if route had failed, abandon it
    if (routehasnotfailed == FALSE)
      return length;

    // remove inefficient waypoints
    if (firstwaypoint->anglereduction() == FALSE)
      return length; // if route had failed, abandon it

    // begin checking the waypoint chain again from the start
    startingpartwaythrough = NULL;
    currentwaypoint = firstwaypoint;

  } // end forever

  return 0; // this should never be reached, it is here only to make a warning go away

} // end getlength

void routenode::addroutetolist(waypoint* newfirstwaypoint, waypoint* fromhere, SquareSide directionalreadytried)
{

  routenode* temporaryroutenodepointer = new routenode(newfirstwaypoint, fromhere, directionalreadytried);

  routenode* currentroutenode = this;
  while (currentroutenode->getnextroutenode() != NULL)
    currentroutenode = currentroutenode->getnextroutenode(); // move to last routenode

  currentroutenode->setnextroutenode(temporaryroutenodepointer);
} // end addroutetolist

waypoint::waypoint(double newx,double newy,UnitSize newsize)
{

  x = newx;
  y = newy;
  size = newsize;

  nextwaypoint = NULL;
  lastwaypoint = NULL;

  tracenextx = -1;
  tracenexty = -1;

} // end constructor

waypoint::waypoint(double newx,double newy, UnitSize newsize, int newtracenextx, int newtracenexty)
{
  x = newx;
  y = newy;
  size = newsize;

  nextwaypoint = NULL;
  lastwaypoint = NULL;

  tracenextx = newtracenextx;
  tracenexty = newtracenexty;
}

waypoint::~waypoint()
{
  if (nextwaypoint != NULL) // delete the entire waypoint chain
    delete nextwaypoint; // to delete just one waypoint you muse set its nextwaypoint to NULL first
} // end destructor

void waypoint::insertnextwaypoint(waypoint* newnextwaypoint)
{
  if(newnextwaypoint != NULL)
  {
    newnextwaypoint->setnextwaypoint(nextwaypoint);
    if (nextwaypoint != NULL)
      nextwaypoint->setlastwaypoint(newnextwaypoint);
    nextwaypoint = newnextwaypoint;
    newnextwaypoint->setlastwaypoint(this);
  }
} // end insertnextwaypoint

waypoint* waypoint::removenextwaypoint()
{
  if (nextwaypoint == NULL)
    return NULL;

  waypoint* waypointholder = nextwaypoint;
  if (waypointholder->getnextwaypoint() != NULL) // if the waypoint being removed is not the end of the chain
  {
    waypointholder->getnextwaypoint()->setlastwaypoint(this);
    nextwaypoint = waypointholder->getnextwaypoint();
  } else
  {
    nextwaypoint = NULL;
  } // endif the waypoint being removed is not the end of the chain

  waypointholder->setnextwaypoint(NULL); // pointer safety, and not deleting the chain randomly
  waypointholder->setlastwaypoint(NULL);

  return waypointholder;
} // end removenextwaypoint

double waypoint::sumroutelength()
{
  if (nextwaypoint == NULL)
    return 0;
  else
    return sqrt(((nextwaypoint->getx() - x) * (nextwaypoint->getx() - x)) + ((nextwaypoint->gety() - y) * (nextwaypoint->gety() - y))) + nextwaypoint->sumroutelength();
} // end sumroutelength

waypoint* waypoint::duplicatewaypointlist(waypoint* waypointactivetrace)
{

  waypoint* tempwaypoint;
  if (waypointactivetrace == this)
    tempwaypoint = new waypoint(x,y,size,tracenextx,tracenexty);
  else
    tempwaypoint = new waypoint(x,y,size);

  if (nextwaypoint != NULL)
  {
    tempwaypoint->setnextwaypoint(nextwaypoint->duplicatewaypointlist(waypointactivetrace));
    tempwaypoint->getnextwaypoint()->setlastwaypoint(tempwaypoint);
  }

  return tempwaypoint;

} // end duplicatewaypointlist

bool waypoint::anglereduction()
{

  waypoint* firstwaypoint = this; // get the first waypoint in the waypoint chain
  while (firstwaypoint->getlastwaypoint() != NULL)
    firstwaypoint = firstwaypoint->getlastwaypoint();

  waypoint* currentwaypoint = firstwaypoint; // holds the waypoint we're working with
  waypoint* waypointcounter = currentwaypoint; // holds the waypoint we're comparing it to

  int fromdx, fromdy, todx, tody; // to gauge the directions of lines
  // from/to_d_x/y + currentwaypoint's x/y = last/next waypoint's x/y
  int fromquad, toquad, quadofbarrier; // to gauge where from
  // top left is quad 1 and numbers rise clockwise


  // remove any waypoint chains with dublicate waypoints
  do
  {
    for(waypointcounter = currentwaypoint->getnextwaypoint();waypointcounter != NULL;waypointcounter = waypointcounter->getnextwaypoint())
    {
      if(currentwaypoint->getx() == waypointcounter->getx() and currentwaypoint->gety() == waypointcounter->gety())
        return FALSE;
    }
  } while ((currentwaypoint = currentwaypoint->getnextwaypoint()) != NULL);

  bool removedawaypoint = TRUE; // are we done yet?
  while (removedawaypoint == TRUE)
  {
    removedawaypoint = FALSE;

    currentwaypoint = firstwaypoint->getnextwaypoint();
    while (currentwaypoint->getnextwaypoint() != NULL) // while there are more waypoints
    {

      // figure out where things are
      fromdx = currentwaypoint->getlastwaypoint()->getx() - currentwaypoint->getx();
      fromdy = currentwaypoint->getlastwaypoint()->gety() - currentwaypoint->gety();
      todx = currentwaypoint->getnextwaypoint()->getx() - currentwaypoint->getx();
      tody = currentwaypoint->getnextwaypoint()->gety() - currentwaypoint->gety();

      // set quadrants to -1 to signify not-yet-set
      fromquad = -1;
      toquad = -1;
      quadofbarrier = -1;

      // find what square barrier is in, assume 1 and only one
      if (map.gettile(currentwaypoint->getx()-1,currentwaypoint->gety()-1) != NULL)
      {
        if (map.gettile(currentwaypoint->getx()-1,currentwaypoint->gety()-1)->istileinpassible(currentwaypoint->getsize()) == TRUE) // if barrier is in quad 1
          quadofbarrier = 1; // barrier is in quad 1
      } else if (map.gettile(currentwaypoint->getx(),currentwaypoint->gety()-1) != NULL)
      {
        if (map.gettile(currentwaypoint->getx(),currentwaypoint->gety()-1)->istileinpassible(currentwaypoint->getsize()) == TRUE) // if barrier is in quad 2
          quadofbarrier = 2; // barrier is in quad 2
      } else if (map.gettile(currentwaypoint->getx(),currentwaypoint->gety()) != NULL)
      {
        if (map.gettile(currentwaypoint->getx(),currentwaypoint->gety())->istileinpassible(currentwaypoint->getsize()) == TRUE) // if barrier is in quad 3
          quadofbarrier = 3; // barrier is in quad 3
      } else if (map.gettile(currentwaypoint->getx()-1,currentwaypoint->gety()) != NULL)
        if (map.gettile(currentwaypoint->getx()-1,currentwaypoint->gety())->istileinpassible(currentwaypoint->getsize()) == TRUE) // if barrier is in quad 4
          quadofbarrier = 4; // barrier is in quad 4

      if (fromdy == 0) // from is on x-axis
      {
        if (tody == 0) // to is also on x-axis
        {
          goto anglereductionremovethiswaypoint; // destroy it
        }
        if (quadofbarrier == 1 or quadofbarrier == 2) // barrier is above
        {
           if (fromdx < 0)// from is below
             fromquad = 4;
           else
             fromquad = 3;
        } else // barrier is below
        {
          if (fromdx < 0)// from is above
            fromquad = 1;
          else
            fromquad = 2;
        }
      } else if (fromdx == 0) // from is on y-axis
      {
        if (todx == 0) // to is also on y-axis
          goto anglereductionremovethiswaypoint; // destroy it
        if (quadofbarrier == 1 or quadofbarrier == 4) // barrier is left
        {
          if (fromdy < 0)// from is right
            fromquad = 2;
          else
            fromquad = 3;
        } else // barrier is right
        {
          if (fromdy < 0) // from is left
            fromquad = 1;
          else
            fromquad = 4;
        }
      } else // from is not on either axis
      {
        if (tody == 0) // if to is on x-axis
        {
          if (quadofbarrier == 1 or quadofbarrier == 2) // barrier is above
          {
            if (todx < 0) // to is below
              toquad = 4;
            else
              toquad = 3;
          } else // barrier is below
          {
            if (todx < 0) // to is above
              toquad = 1;
            else
              toquad = 2;
          }
        } else if (todx == 0) // if to is on y-axis
        {
          if (quadofbarrier == 1 or quadofbarrier == 4) // barrier is left
          {
            if (tody < 0) // to is right
            {
              toquad = 2;
            } else
            {
              toquad = 3;
            }
          } else // barrier is right
          {
            if (tody < 0) // to is left
            {
              toquad = 1;
            } else
            {
              toquad = 4;
            }
          }
        } // end if to is on an axis

        if (fromquad == -1) // if from quad is not set
        {
          if (fromdx < 0 and fromdy < 0) // if from is in quad 1
            fromquad = 1; // from is in quad 1
          else if (fromdx > 0 and fromdy < 0) // if from is in quad 2
            fromquad = 2; // from is in quad 2
          else if (fromdx > 0 and fromdy > 0) // if from is in quad 3
            fromquad = 3; // from is in quad 3
          else if (fromdx < 0 and fromdy > 0) // if from is in quad 4
            fromquad = 4; // from is in quad 4
        } // endif from quad is not set
        if (toquad == -1) // if to quad is not set
        {
          if (todx < 0 and tody < 0) // if to is in quad 1
            toquad = 1; // to is in quad 1
          else if (todx > 0 and tody < 0) // if to is in quad 2
            toquad = 2; // to is in quad 2
          else if (todx > 0 and tody > 0) // if to is in quad 3
            toquad = 3; // to is in quad 3
          else if (todx < 0 and tody > 0) // if to is in quad 4
            toquad = 4; // to is in quad 4
        } // endif to quad is not set

        if(todx/fromdx == tody/fromdy) // if is a straight line
          goto anglereductionremovethiswaypoint; // destroy it

        if (fromquad == toquad) // if from and to are in same quad
          goto anglereductionremovethiswaypoint; // destroy it

        if (fromquad == quadofbarrier) // if from and barrier are in same quad
          goto anglereductionremovethiswaypoint; // destroy it

        if (toquad == quadofbarrier) // if to and barrier are in same quad
          goto anglereductionremovethiswaypoint; // destroy it

        if (((double)tody) < ((double)fromdy) + ((double)(fromdy/fromdx))*((double)(todx-fromdx))) // if to is above an extension of from
        {
          if ((fromquad == 4 or fromquad == 2) and quadofbarrier == 3) // from is in 4 or 2 and barrier is in 3
            goto anglereductionremovethiswaypoint; // destroy it
          else if ((fromquad == 1 or fromquad == 3) and quadofbarrier == 4) // from is in 1 or 3 and barrier is in 4
            goto anglereductionremovethiswaypoint; // destroy it
        } else // if to is below an extension of from
        {
          if ((fromquad == 4 or fromquad == 2) and quadofbarrier == 1) // from is in 4 or 2 and barrier is in 1
            goto anglereductionremovethiswaypoint; // destroy it
          else if ((fromquad == 1 or fromquad == 3) and quadofbarrier == 2) // from is in 1 or 3 and barrier is in 2
            goto anglereductionremovethiswaypoint; // destroy it
        } // endif to's position relative to an extension of from
      } // endif from is not on either axis
      // ok, this waypoint passes...this time.
      currentwaypoint = currentwaypoint->getnextwaypoint();
      continue; // don't do the rest of the while loop, that's for deleting a waypoint

      anglereductionremovethiswaypoint:; // waypoints removed here

      currentwaypoint = currentwaypoint->getlastwaypoint();
      delete (currentwaypoint->removenextwaypoint());

      currentwaypoint = currentwaypoint->getnextwaypoint();
      removedawaypoint = TRUE; // keep going
    } // end while there are more waypoints

  } // end while we are not done yet

  return TRUE;
} // end anglereduction


int waypoint::trace(tile** tilehitfrom, SquareSide& sidehitfrom, tile** tilehitto, SquareSide& sidehitto)
{

  if (nextwaypoint == NULL)
    return -1; // this should never happen

  // don't call trace twice with the same parameters
  if (tracenextx == nextwaypoint->getx() and tracenexty == nextwaypoint->gety())
    return -1; // abandon infinite loop paths

  if (subtrace(nextwaypoint, tilehitfrom, sidehitfrom, NULL) == TRUE)
    return 1;
  else
  {
    nextwaypoint->subtrace(this, tilehitto, sidehitto, tilehitfrom);
    return 0;
  }

} // end trace

bool waypoint::checkpassable()
{

  tile** tilehitfrom = NULL; // data to be discarded
  SquareSide sidehitfrom;

  return subtrace(nextwaypoint, tilehitfrom, sidehitfrom, NULL);

} // end checkpassable

bool waypoint::subtrace(waypoint* waypointto, tile** tilehitfrom, SquareSide& sidehitfrom, tile** tilecollectiontohit)
{

  double tracenextx = waypointto->getdoublex(); // I'm sorry for the confusing variable hiding,
  double tracenexty = waypointto->getdoubley(); // I honestly should have planned this better

  tile* tilepointer = NULL; // the tile we're checking

  bool collision = FALSE;
  tile* thetilehit = NULL;
  SquareSide directionhit = NONE;
  double directionx = 0;
  double directiony = 0;

  if (x == tracenextx and y == tracenexty) // if you're already there
    return TRUE;

  if (roundnumber(y)) // if start is on x-axis
  {

    if (tracenexty == y) // if finish is on the same x-axis
    {
      directionx = (tracenextx - x) / abs(tracenextx - x); // get direction of finish from start
      for (tilepointer = map.gettile(x,y);tilepointer != map.gettile(tracenextx,tracenexty);tilepointer = map.gettile(tilepointer->getx() + directionx,tilepointer->gety())) // for each line segment between here and there
      {
        collision = didcollideline(tilepointer,map.gettile(tilepointer->getx(),tilepointer->gety() - 1), waypointto, &thetilehit, directionhit); // check that you can walk that horizontal line
      }
      if (collision == TRUE) // if collided with something
      {
        if(tilecollectiontohit == NULL) // check if hit object that counts
        { // fill in collision data, and if hit an object that counts, return
          if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
          {
            *tilehitfrom = thetilehit;
            sidehitfrom = directionhit;
            return 0;
          } else
          {
            thetilehit = NULL;
            directionhit = NONE;
          }
        } else
        {
          *tilehitfrom = thetilehit;
          sidehitfrom = directionhit;
          return 0;
        } // end check if hit object that counts and if so return
      }
      return 1; // return 1
    }
  } // endif start is on x-axis

  if (roundnumber(x)) // if start is on y-axis
  {

    if (tracenextx == x) // if finish is on the same y-axis
    {
      directiony = (tracenexty - y) / abs(tracenexty - y); // get direction of finish from start
      for (tilepointer = map.gettile(x,y);tilepointer != map.gettile(tracenextx,tracenexty);tilepointer = map.gettile(tilepointer->getx(),tilepointer->gety() + directiony)) // for each line segment between here and there
      {
        collision = didcollideline(tilepointer,map.gettile(tilepointer->getx() - 1,tilepointer->gety()), waypointto, &thetilehit, directionhit); // check that you can walk that vertical line
      }
      if (collision == TRUE) // if collided with something
      {
        if(tilecollectiontohit == NULL) // check if hit object that counts
        { // fill in collision data, and if hit an object that counts, return
          if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
          {
            *tilehitfrom = thetilehit;
            sidehitfrom = directionhit;
            return 0;
          } else
          {
            thetilehit = NULL;
            directionhit = NONE;
          }
        } else
        {
          *tilehitfrom = thetilehit;
          sidehitfrom = directionhit;
          return 0;
        } // end check if hit object that counts and if so return
      }
      return 1; // return 1
    }
  } // endif start is on y-axis

  if (tracenextx == x) // if there is only vertical movement
  {
    directiony = (tracenexty - y) / abs(tracenexty - y); // get direction of finish from start
    for (tilepointer = map.gettile(x,y);tilepointer != map.gettile(tracenextx,tracenexty);tilepointer = map.gettile(tilepointer->getx(),tilepointer->gety() + directiony)) // for each line segment between here and there
    {
      collision = didcollide(tilepointer, waypointto, &thetilehit, directionhit); // check that you can walk that vertical line
    }
    if (collision == TRUE) // if collided with something
    {
      if(tilecollectiontohit == NULL) // check if hit object that counts
      { // fill in collision data, and if hit an object that counts, return
        if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
        {
          *tilehitfrom = thetilehit;
          sidehitfrom = directionhit;
          return 0;
        } else
        {
          thetilehit = NULL;
          directionhit = NONE;
        }
      } else
      {
        *tilehitfrom = thetilehit;
        sidehitfrom = directionhit;
        return 0;
      } // end check if hit object that counts and if so return
    }
    return 1; // return 1
  } // endif there is only vertical movement

  bool isonsametile = FALSE; // is it on the same tile
  for (int i = x; ((double)i) >= x - 1 ; i -= 1)
  {
    for (int j = y; ((double)j) >= y - 1 ; j -= 1)
    {
      for (int k = tracenextx; ((double)k) >= tracenextx - 1 ; k -= 1)
      {
        for (int l = tracenexty; ((double)l) >= tracenexty - 1 ; l -= 1)
        {
          if (map.gettile(i,j) == map.gettile(k,l))
          {
            collision = didcollide(map.gettile(i,j), waypointto, &thetilehit, directionhit); // is the one tile passable
            if (collision == TRUE) // if collided with something
            {
              if(tilecollectiontohit == NULL) // check if hit object that counts
              { // fill in collision data, and if hit an object that counts, return
                if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
                {
                  *tilehitfrom = thetilehit;
                  sidehitfrom = directionhit;
                  return 0;
                } else
                {
                  thetilehit = NULL;
                  directionhit = NONE;
                }
              } else
              {
                *tilehitfrom = thetilehit;
                sidehitfrom = directionhit;
                return 0;
              } // end check if hit object that counts and if so return
            }
            isonsametile = TRUE;
          }
        }
      }
    }
  }
  if (isonsametile == TRUE) // if is on same tile (and tile wasn't impassible)
    return 1; // return 1

  // recall tilepointer // the tile we're checking
  // recall directionx // 1 or -1 to indicate what direction x must change to get to the target
  // recall directiony // 1 or -1 to indicate what direction y must change as we follow the changing x
  double currentx = x; // the current x value
  double currenty = y; // the current y value
  directionx = (tracenextx - x) / abs(tracenextx - x); // get direction of finish from start
  directiony = (tracenexty - y) / (tracenextx - x); // the change is y-value per x-border crossed; to multiply by the change in x, sign inclusive

  int currenttiley; // get the y-value of the tile you currently reside in
  if (roundnumber(currenty))
    currenttiley = (directiony > 1) ? y : y - 1;
  else
    currenttiley = y;

  if (directionx > 0)
  {
    currenty = (((double)((int)currentx + 1)) - currentx) * directiony;
    currentx = ((int)currentx + 1);
  } else
  {
    currenty = (((double)((int)currentx)) - currentx) * directiony;
    currentx = ((int)currentx);
  }

  for (tilepointer = map.gettile((directionx > 0) ? currentx - 1 : currentx, currenttiley);
      tilepointer != map.gettile((directionx > 0) ? currentx - 1 : currentx, currenty - 1 * ((roundnumber(currenty)) and (directiony > 0)));
      tilepointer = map.gettile(tilepointer->getx(),tilepointer->gety() + ((directiony > 0) ? 1 : -1)))
  { // for each tile we entered, vertically, while moving to the first place we cross a y-axis
    didcollide(map.gettile(tilepointer->getx(),tilepointer->gety()), waypointto, &thetilehit, directionhit);
  } // end for each tile we entered, vertically, while moving to the first place we cross a y-axis
  if (collision == TRUE) // if collided with something
  {
    if(tilecollectiontohit == NULL) // check if hit object that counts
    { // fill in collision data, and if hit an object that counts, return
      if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
      {
        *tilehitfrom = thetilehit;
        sidehitfrom = directionhit;
        return 0;
      } else
      {
        thetilehit = NULL;
        directionhit = NONE;
      }
    } else
    {
      *tilehitfrom = thetilehit;
      sidehitfrom = directionhit;
      return 0;
    } // end check if hit object that counts and if so return
  }

  while ((directionx > 0) ? ((currentx + directionx) <= tracenextx) : ((currentx + directionx) >= tracenextx)) // while you will go a full horizontal unit without hitting the finish
  {

    if (roundnumber(currentx) and roundnumber(currenty)) // if you're on a corner
    {
      collision = didcollideline(((directionx*directiony) > 0) ? map.gettile(currentx - 1,currenty) : map.gettile(currentx,currenty - 1), ((directionx*directiony) > 0) ? map.gettile(currentx,currenty) : map.gettile(currentx - 1,currenty - 1), waypointto, &thetilehit, directionhit);
      if (TRUE == collision) // if it's an impassible corner, collide with something
      {
        if(tilecollectiontohit == NULL) // check if hit object that counts
        { // fill in collision data, and if hit an object that counts, return
          if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
          {
            *tilehitfrom = thetilehit;
            sidehitfrom = directionhit;
            return 0;
          } else
          {
            thetilehit = NULL;
            directionhit = NONE;
          }
        } else
        {
          *tilehitfrom = thetilehit;
          sidehitfrom = directionhit;
          return 0;
        } // end check if hit object that counts and if so return
      } else // if it's a corner but passible
      {

        collision = didcollide(map.gettile((currentx + (directionx * 0.5)), (currenty + ((directiony / abs(directiony)) * 0.5))), waypointto, &thetilehit, directionhit);
        if (collision == TRUE)
        { // if it would collide in the square after the diagonal that it can pass through
          if(tilecollectiontohit == NULL) // check if hit object that counts
          { // fill in collision data, and if hit an object that counts, return
            if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
            {
              *tilehitfrom = thetilehit;
              sidehitfrom = directionhit;
              return 0;
            } else
            {
              thetilehit = NULL;
              directionhit = NONE;
            }
          } else
          {
            *tilehitfrom = thetilehit;
            sidehitfrom = directionhit;
            return 0;
          } // end check if hit object that counts and if so return
        } // end if collided with the square after the diagonal that it can pass through
      } // endif is it a passable corner
    } else // it's not a corner
    {
      collision = didcollide(map.gettile(((directionx > 0) ? currentx : currentx - 1), currenty), waypointto, &thetilehit, directionhit); // check if the next horizontal square is a collision
      if (collision == TRUE) // if it's a collision
      {
        if(tilecollectiontohit == NULL) // check if hit object that counts
        { // fill in collision data, and if hit an object that counts, return
          if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
          {
            *tilehitfrom = thetilehit;
            sidehitfrom = directionhit;
            return 0;
          } else
          {
            thetilehit = NULL;
            directionhit = NONE;
          }
        } else
        {
          *tilehitfrom = thetilehit;
          sidehitfrom = directionhit;
          return 0;
        } // end check if hit object that counts and if so return
      } // endif collided
    } // endif are you on a corner
    currenttiley = currenty; // update currentx and currenty, also leave currenttiley at starting y
    currentx += directionx;
    currenty += directiony * directionx;
    while (currenttiley != ((int)((roundnumber(currenty) and directiony > 0) ? currenty - 1 : currenty)))
    { // for each vertical unit you change
      currenttiley += directiony / abs(directiony);

      collision = didcollide(map.gettile(((directionx > 0) ? currentx - 1 : currentx),currenttiley), waypointto, &thetilehit, directionhit); // check if it's a collision
      if (collision == TRUE) // if it's a collision
      {
        if(tilecollectiontohit == NULL) // check if hit object that counts
        { // fill in collision data, and if hit an object that counts, return
          if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
          {
            *tilehitfrom = thetilehit;
            sidehitfrom = directionhit;
            return 0;
          } else
          {
            thetilehit = NULL;
            directionhit = NONE;
          }
        } else
        {
          *tilehitfrom = thetilehit;
          sidehitfrom = directionhit;
          return 0;
        } // end check if hit object that counts and if so return
      } // endif collided
    } // end for each vertical unit you change
  } // end while you will go a full horizontal unit without hitting the finish

  if (currentx == tracenextx and currenty == tracenexty) // if you're already there
    return TRUE;

  if (roundnumber(currentx) and roundnumber(currenty)) // if you're on a corner
  {
    collision = didcollideline(((directionx*directiony) > 0) ? map.gettile(currentx - 1,currenty) : map.gettile(currentx,currenty - 1), ((directionx*directiony) > 0) ? map.gettile(currentx,currenty) : map.gettile(currentx - 1,currenty - 1), waypointto, &thetilehit, directionhit);
    if (TRUE == collision) // if it's an impassible corner, collide with something
    {
      if(tilecollectiontohit == NULL) // check if hit object that counts
      { // fill in collision data, and if hit an object that counts, return
        if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
        {
          *tilehitfrom = thetilehit;
          sidehitfrom = directionhit;
          return 0;
        } else
        {
          thetilehit = NULL;
          directionhit = NONE;
        }
      } else
      {
        *tilehitfrom = thetilehit;
        sidehitfrom = directionhit;
        return 0;
      } // end check if hit object that counts and if so return
    } else // if it's a corner but passible
    {

      collision = didcollide(map.gettile((currentx + (directionx * 0.5)), (currenty + ((directiony / abs(directiony)) * 0.5))), waypointto, &thetilehit, directionhit);
      if (collision == TRUE)
      { // if it would collide in the square after the diagonal that it can pass through
        if(tilecollectiontohit == NULL) // check if hit object that counts
        { // fill in collision data, and if hit an object that counts, return
          if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
          {
            *tilehitfrom = thetilehit;
            sidehitfrom = directionhit;
            return 0;
          } else
          {
            thetilehit = NULL;
            directionhit = NONE;
          }
        } else
        {
          *tilehitfrom = thetilehit;
          sidehitfrom = directionhit;
          return 0;
        } // end check if hit object that counts and if so return
      } // end if collided with the square after the diagonal that it can pass through
    } // endif is it a passable corner
  } else // it's not a corner
  {
    collision = didcollide(map.gettile(((directionx > 0) ? currentx : currentx - 1), currenty), waypointto, &thetilehit, directionhit); // check if the next horizontal square is a collision
    if (collision == TRUE) // if it's a collision
    {
      if(tilecollectiontohit == NULL) // check if hit object that counts
      { // fill in collision data, and if hit an object that counts, return
        if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
        {
          *tilehitfrom = thetilehit;
          sidehitfrom = directionhit;
          return 0;
        } else
        {
          thetilehit = NULL;
          directionhit = NONE;
        }
      } else
      {
        *tilehitfrom = thetilehit;
        sidehitfrom = directionhit;
        return 0;
      } // end check if hit object that counts and if so return
    } // endif collided
  } // endif are you on a corner

  currenttiley = currenty; // update currentx and currenty, also leave currenttiley at starting y
  currentx = tracenextx;
  currenty = tracenexty;

  while (currenttiley != ((int)((roundnumber(currenty) and directiony > 0) ? currenty - 1 : currenty)))
    { // for each vertical unit you change
      currenttiley += directiony / abs(directiony);

      collision = didcollide(map.gettile(((directionx > 0) ? currentx - 1 : currentx),currenttiley), waypointto, &thetilehit, directionhit); // check if it's a collision
      if (collision == TRUE) // if it's a collision
      {
        if(tilecollectiontohit == NULL) // check if hit object that counts
        { // fill in collision data, and if hit an object that counts, return
          if(sameobstacle(thetilehit,directionhit,*tilecollectiontohit) == TRUE)
          {
            *tilehitfrom = thetilehit;
            sidehitfrom = directionhit;
            return 0;
          } else
          {
            thetilehit = NULL;
            directionhit = NONE;
          }
        } else
        {
          *tilehitfrom = thetilehit;
          sidehitfrom = directionhit;
          return 0;
        } // end check if hit object that counts and if so return
      } // endif collided
    } // end for each vertical unit you change

  return 1; // success

} // end subtrace

bool waypoint::didcollideline(tile* tilepointer1,tile* tilepointer2, waypoint* waypointto, tile** thetilehit, SquareSide& directionhit)
{

  if (*thetilehit != NULL) // if a collision olready occured, do nothing
    return TRUE;

  int firstcheck = 1, secondcheck = 1; // 1 no collision, 0 collision, -1 off map edge
  tile* tilehitfirst = NULL;
  tile* tilehitsecond = NULL;
  SquareSide directionhitfirst = NONE;
  SquareSide directionhitsecond = NONE;

  if (tilepointer1 == NULL)
    firstcheck = -1;
  else
    firstcheck = didcollide(tilepointer1,waypointto,&tilehitfirst,directionhitfirst);

  if (tilepointer2 == NULL)
    secondcheck = -1;
  else
    secondcheck = didcollide(tilepointer2,waypointto,&tilehitsecond,directionhitsecond);

  // both checks are -1 should never happen

  if (firstcheck == 1 or secondcheck == 1)
    return FALSE;

  if (firstcheck == -1)
  {
    *thetilehit = tilehitsecond;
    directionhit = directionhitsecond;
    return TRUE;
  } else
  {
    *thetilehit = tilehitfirst;
    directionhit = directionhitfirst;
    return TRUE;
  }

} // end didcollideline

bool waypoint::didcollide(tile* tilepointer, waypoint* waypointto, tile** thetilehit , SquareSide& directionhit)
{

  if (*thetilehit != NULL) // if a collision already occured, do nothing
    return TRUE;

  if (tilepointer->istileinpassible(size)) // if no collision
    return FALSE; // say so

  // there must have been a collision
  *thetilehit = tilepointer;

  double currentx = this->getdoublex();
  double currenty = this->getdoubley();
  double dx = waypointto->getdoublex() - currentx;
  double dy = waypointto->getdoubley() - currenty;

  if (dx == 0) // if only vertical movement
  {
    if (dy < 0) // if going up
      directionhit = BOTTOM; // hit bot
    else
      directionhit = TOP; // hit top
    return TRUE; // do not continue (or you will divide by zero)
  }

  double yatside = (dx < 0) ? (currenty + (dy/dx) * (tilepointer->getx()+1 - currentx)) : (currenty + (dy/dx) * (tilepointer->getx() - currentx));

  if (yatside >= tilepointer->gety() and yatside <= tilepointer->gety()+1) // yatside is within side-collision height
  {
    if (dx < 0) // if going left
      directionhit = RIGHT; // hit right
    else
      directionhit = LEFT; // hit left
  } else // yatside is not within side-collision height
  {
    if (dy < 0) // if going up
      directionhit = BOTTOM; // hit bot
    else
      directionhit = TOP; // hit top
  }

  return TRUE;

} // end didcollide

bool waypoint::sameobstacle(tile* thetilehit,SquareSide directionhit,tile* tilecollectiontohit)
{
  int nextspottotry; // 0 is topleft and the increase clockwise
  tile* potentialnexttile;
  int currenttilex = thetilehit->getx(); // the location of the tile being checked, not the one being held
  int currenttiley = thetilehit->gety();

  switch(directionhit) // set starting nextspottotry
  {
    case LEFT:
      nextspottotry = 0;
      currenttilex--;
      currenttiley--;
    break;
    case TOP:
      nextspottotry = 2;
      currenttilex++;
      currenttiley--;
    break;
    case RIGHT:
      nextspottotry = 4;
      currenttilex++;
      currenttiley++;
    break;
    case BOTTOM:
      nextspottotry = 6;
      currenttilex--;
      currenttiley++;
    break;
    default: // this should never be used; "NONE" should never be a value of directionhit
    break;
  } // end switch set starting nextspottotry

  int firstcurrentx = currenttilex; // for knowing when to quit
  int firstcurrenty = currenttiley;
  int firstspottotry = nextspottotry;
  bool passedbefore = FALSE;

  for(;;) // forever
  {
    potentialnexttile = map.gettile(currenttilex,currenttiley);
    if (potentialnexttile != NULL) // if you're on the map still
    {

      if (potentialnexttile == tilecollectiontohit) // if you've reached the other tile
        return TRUE;
      if (firstcurrentx == currenttilex and firstcurrenty == currenttiley and firstspottotry == nextspottotry)
      {
        if (passedbefore == FALSE)
          passedbefore = TRUE;
        else // if you're back where you started
          return FALSE;
      }
    } // end if you're on the map

    if (potentialnexttile->istileinpassible(size) == TRUE) // if the tile being checked is not passable
    {

      // move to that tile
      switch (nextspottotry)
      {
        case 0:
        case 1:
        case 2:
          currenttiley--;
        break;
        case 4:
        case 5:
        case 6:
          currenttiley++;
        break;
      }
      switch (nextspottotry)
      {
        case 2:
        case 3:
        case 4:
          currenttilex++;
        break;
        case 6:
        case 7:
        case 0:
          currenttilex--;
        break;
      }

      // move four tiles around the checking order
      for (int i = 0; i < 4; i++)
      {
        switch (nextspottotry) // move to the next tile
        { // clockwise
          case 0:
          case 1:
            currenttilex++;
          break;
          case 2:
          case 3:
            currenttiley++;
          break;
          case 4:
          case 5:
            currenttilex--;
          break;
          case 6:
          case 7:
            currenttiley--;
          break;
        } // end switch move to the next tile
        nextspottotry++;
        if (nextspottotry > 7)
          nextspottotry -= 8;
      } // end move four tiles around the checking order

    } // end if is the tile being checked passable

    switch (nextspottotry) // move to the next tile
    { // clockwise
      case 0:
      case 1:
        currenttilex++;
      break;
      case 2:
      case 3:
        currenttiley++;
      break;
      case 4:
      case 5:
        currenttilex--;
      break;
      case 6:
      case 7:
        currenttiley--;
      break;
    } // end switch move to the next tile
    nextspottotry++;
    if (nextspottotry > 7)
      nextspottotry -= 8;

  } // end forever

  return 0; // this should never be reached, it is here only to make a warning go away
} // end sameobstacle

tile* Map::gettile(int tilex,int tiley)
{
  if (tilex < 0 or tiley < 0 or tilex >= mapwidth or tiley >= mapheight)
    return NULL;
  return tiles[tilex][tiley];
} // end gettile
