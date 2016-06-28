/*
 *  These functions were so cumbersome they merited their own file.
 *  They would otherwise be part of Pathing.cpp
 *
 *  Created on: 24/08/2015
 *      Author: Alex
 */

#include "GameDataHeader.h"

extern Map map; // declare the global map object




bool waypoint::circumvent(SquareSide& attempted, tile& tilehitfrom, SquareSide sidehitfrom, tile& tilehitto, SquareSide sidehitto)
{

  waypoint* currentwaypoint = this;
  tile* currenttile = NULL;
  tile* potentialnexttile = NULL;
  SquareSide sidetohitlastsquare; // to avoid getting messed up by walls 1 tile thick
  SquareSide sideoflastsquare;

  // specify what direction you're going
  if (sidehitfrom == BOTTOM) {attempted = LEFT; sideoflastsquare = RIGHT;}
  if (sidehitfrom == LEFT) {attempted = TOP; sideoflastsquare = BOTTOM;}
  if (sidehitfrom == TOP) {attempted = RIGHT; sideoflastsquare = LEFT;}
  if (sidehitfrom == RIGHT) {attempted = BOTTOM; sideoflastsquare = TOP;}

  currenttile = &tilehitfrom; // set / reset currenttile

  // assign a value to sidetohitlastsquare
  if (map.gettile(currenttile->getx()-1,currenttile->gety())->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx()-1,currenttile->gety()-1)->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx(),currenttile->gety()-1)->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx()+1,currenttile->gety()-1)->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx()+1,currenttile->gety())->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx()+1,currenttile->gety()+1)->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx(),currenttile->gety()+1)->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx()-1,currenttile->gety()+1)->istileinpassible(size) == FALSE)
  { // if it is a single tile obstruction
    sidetohitlastsquare = sideoflastsquare;
  } else
  {

    currenttile = &tilehitto;

    if (sidehitto == BOTTOM) {goto clockwisefunctionsidetohitlastsquarefrombottom;}
    if (sidehitto == LEFT) {goto clockwisefunctionsidetohitlastsquarefromleft;}
    if (sidehitto == TOP) {goto clockwisefunctionsidetohitlastsquarefromtop;}
    if (sidehitto == RIGHT) {goto clockwisefunctionsidetohitlastsquarefromright;}

    for (;;)
    {

      clockwisefunctionsidetohitlastsquarefromtop:;

      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety());
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
      if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
      {
        sidetohitlastsquare = LEFT;
        goto clockwisefunctionsidetohitlastsquareend;
      }
      }
      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety()-1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
      if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
      {
        sidetohitlastsquare = LEFT;
        goto clockwisefunctionsidetohitlastsquareend;
      }
      }
      insertnextwaypoint(new waypoint(currenttile->getx(),currenttile->gety(),size));

      clockwisefunctionsidetohitlastsquarefromleft:;

      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()+1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
      if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
      {
        sidetohitlastsquare = BOTTOM;
        goto clockwisefunctionsidetohitlastsquareend;
      }
      }
      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety()+1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
      if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
      {
        sidetohitlastsquare = BOTTOM;
        goto clockwisefunctionsidetohitlastsquareend;
      }
      }
      insertnextwaypoint(new waypoint(currenttile->getx(),currenttile->gety()+1,size));

      clockwisefunctionsidetohitlastsquarefrombottom:;

      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety());
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
      if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
      {
        sidetohitlastsquare = RIGHT;
        goto clockwisefunctionsidetohitlastsquareend;
      }
      }
      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety()+1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
      if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
      {
        sidetohitlastsquare = RIGHT;
        goto clockwisefunctionsidetohitlastsquareend;
      }
      }
      insertnextwaypoint(new waypoint(currenttile->getx()+1,currenttile->gety()+1,size));

      clockwisefunctionsidetohitlastsquarefromright:;

      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()-1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
      if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
      {
        sidetohitlastsquare = TOP;
        goto clockwisefunctionsidetohitlastsquareend;
      }
      }
      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety()-1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
      if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
      {
        sidetohitlastsquare = TOP;
        goto clockwisefunctionsidetohitlastsquareend;
      }
      }
      insertnextwaypoint(new waypoint(currenttile->getx()+1,currenttile->gety(),size));

    } // end forever

    clockwisefunctionsidetohitlastsquareend:;

  } // endif is/isn't a single tile obstruction

  currenttile = &tilehitfrom; // set / reset currenttile

  while (currenttile != &tilehitto or sideoflastsquare != sidetohitlastsquare)
  { // while there are more tiles to step through

    if (sideoflastsquare == BOTTOM) {goto clockwisecircumventhitfrombottom;}
    if (sideoflastsquare == LEFT) {goto clockwisecircumventhitfromleft;}
    if (sideoflastsquare == TOP) {goto clockwisecircumventhitfromtop;}
    if (sideoflastsquare == RIGHT) {goto clockwisecircumventhitfromright;}

    for(;;) // forever
    {

      clockwisecircumventhitfrombottom:;

      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety());
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = RIGHT;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety()-1);
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = RIGHT;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()-1);
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = BOTTOM;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      insertnextwaypoint(new waypoint(currenttile->getx(),currenttile->gety(),size));
      currentwaypoint = currentwaypoint->getnextwaypoint();

      clockwisecircumventhitfromleft:;

      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()-1);
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = BOTTOM;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety()-1);
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = BOTTOM;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety());
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = LEFT;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      insertnextwaypoint(new waypoint(currenttile->getx()+1,currenttile->gety(),size));
      currentwaypoint = currentwaypoint->getnextwaypoint();

      clockwisecircumventhitfromtop:;

      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety());
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = LEFT;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety()+1);
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = LEFT;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()+1);
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = TOP;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      insertnextwaypoint(new waypoint(currenttile->getx()+1,currenttile->gety()+1,size));
      currentwaypoint = currentwaypoint->getnextwaypoint();

      clockwisecircumventhitfromright:;

      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()+1);
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = TOP;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety()+1);
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = TOP;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety());
      if (potentialnexttile == NULL)// if you hit the map edge
      {
        return FALSE;// the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = RIGHT;
          goto endclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      insertnextwaypoint(new waypoint(currenttile->getx(),currenttile->gety()+1,size));
      currentwaypoint = currentwaypoint->getnextwaypoint();

    } // end forever

    endclockwisecircumventnexttilechecks:;

  } // end while there are more tiles to step through

  return TRUE;
} // end circumvent (no direction)

bool waypoint::circumvent(tile& tilehitfrom, SquareSide sidehitfrom, tile& tilehitto, SquareSide sidehitto, SquareSide directionalreadytried)
{

  // ignore the direction you're told to go in; it can only be counterclockwise


  waypoint* currentwaypoint = this;
  tile* currenttile = NULL;
  tile* potentialnexttile = NULL;
  SquareSide sidetohitlastsquare; // to avoid getting messed up by walls 1 tile thick
  SquareSide sideoflastsquare;

  // specify what direction you're going
  if (sidehitfrom == BOTTOM) {sideoflastsquare = LEFT;}
  if (sidehitfrom == LEFT) {sideoflastsquare = TOP;}
  if (sidehitfrom == TOP) {sideoflastsquare = RIGHT;}
  if (sidehitfrom == RIGHT) {sideoflastsquare = BOTTOM;}

  currenttile = &tilehitfrom; // set / reset currenttile

  // assign a value to sidetohitlastsquare
  if (map.gettile(currenttile->getx()-1,currenttile->gety())->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx()-1,currenttile->gety()-1)->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx(),currenttile->gety()-1)->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx()+1,currenttile->gety()-1)->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx()+1,currenttile->gety())->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx()+1,currenttile->gety()+1)->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx(),currenttile->gety()+1)->istileinpassible(size) == FALSE and
  map.gettile(currenttile->getx()-1,currenttile->gety()+1)->istileinpassible(size) == FALSE)
  { // if it is a single tile obstruction
    sidetohitlastsquare = sideoflastsquare;
  } else
  {

    currenttile = &tilehitto;

    if (sidehitto == BOTTOM) {goto counterclockwisefunctionsidetohitlastsquarefrombottom;}
    if (sidehitto == LEFT) {goto counterclockwisefunctionsidetohitlastsquarefromleft;}
    if (sidehitto == TOP) {goto counterclockwisefunctionsidetohitlastsquarefromtop;}
    if (sidehitto == RIGHT) {goto counterclockwisefunctionsidetohitlastsquarefromright;}


    for (;;)
    {

      counterclockwisefunctionsidetohitlastsquarefromtop:;

      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety());
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          sidetohitlastsquare = RIGHT;
          goto counterclockwisefunctionsidetohitlastsquareend;
        }
      }
      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety()-1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          sidetohitlastsquare = RIGHT;
          goto counterclockwisefunctionsidetohitlastsquareend;
        }
      }
      insertnextwaypoint(new waypoint(currenttile->getx()+1,currenttile->gety(),size)); // top right

      counterclockwisefunctionsidetohitlastsquarefromright:;

      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()+1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          sidetohitlastsquare = BOTTOM;
          goto counterclockwisefunctionsidetohitlastsquareend;
        }
      }
      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety()+1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          sidetohitlastsquare = BOTTOM;
          goto counterclockwisefunctionsidetohitlastsquareend;
        }
      }
      insertnextwaypoint(new waypoint(currenttile->getx()+1,currenttile->gety()+1,size)); // bottom right

      counterclockwisefunctionsidetohitlastsquarefrombottom:;

      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety());
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          sidetohitlastsquare = LEFT;
          goto counterclockwisefunctionsidetohitlastsquareend;
        }
      }
      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety()+1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          sidetohitlastsquare = LEFT;
          goto counterclockwisefunctionsidetohitlastsquareend;
        }
      }
      insertnextwaypoint(new waypoint(currenttile->getx(),currenttile->gety()+1,size)); // bottom left

      counterclockwisefunctionsidetohitlastsquarefromleft:;

      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()-1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          sidetohitlastsquare = TOP;
          goto counterclockwisefunctionsidetohitlastsquareend;
        }
      }
      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety()-1);
      if (potentialnexttile == NULL){return FALSE;}else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          sidetohitlastsquare = TOP;
          goto counterclockwisefunctionsidetohitlastsquareend;
        }
      }
      insertnextwaypoint(new waypoint(currenttile->getx(),currenttile->gety(),size)); // top left

    } // end forever

    counterclockwisefunctionsidetohitlastsquareend:;

  } // endif is/isn't a single tile obstruction

  currenttile = &tilehitfrom; // set / reset currenttile

  while (currenttile != &tilehitto or sideoflastsquare != sidetohitlastsquare)
  { // while there are more tiles to step through

    if (sideoflastsquare == BOTTOM) {goto counterclockwisecircumventhitfrombottom;}
    if (sideoflastsquare == LEFT) {goto counterclockwisecircumventhitfromleft;}
    if (sideoflastsquare == TOP) {goto counterclockwisecircumventhitfromtop;}
    if (sideoflastsquare == RIGHT) {goto counterclockwisecircumventhitfromright;}

    for(;;) // forever
    {


      counterclockwisecircumventhitfrombottom:;

      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety());
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = LEFT;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety()-1);
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = LEFT;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()-1);
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = BOTTOM;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      insertnextwaypoint(new waypoint(currenttile->getx()+1,currenttile->gety(),size)); // top right
      currentwaypoint = currentwaypoint->getnextwaypoint();

      counterclockwisecircumventhitfromright:;

      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()-1);
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = BOTTOM;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety()-1);
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = BOTTOM;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety());
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = RIGHT;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      insertnextwaypoint(new waypoint(currenttile->getx(),currenttile->gety(),size)); // top left
      currentwaypoint = currentwaypoint->getnextwaypoint();

      counterclockwisecircumventhitfromtop:;

      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety());
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = RIGHT;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()-1,currenttile->gety()+1);
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = RIGHT;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()+1);
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = TOP;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      insertnextwaypoint(new waypoint(currenttile->getx(),currenttile->gety()+1,size)); // bottom left
      currentwaypoint = currentwaypoint->getnextwaypoint();

      counterclockwisecircumventhitfromleft:;

      potentialnexttile = map.gettile(currenttile->getx(),currenttile->gety()+1);
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = TOP;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety()+1);
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = TOP;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      potentialnexttile = map.gettile(currenttile->getx()+1,currenttile->gety());
      if (potentialnexttile == NULL) // if you hit the map edge
      {
        return FALSE; // the path fails
      } else // if you haven't hit the map edge
      {
        if (potentialnexttile->istileinpassible(size) == TRUE) // if it's inpassible
        {
          currenttile = potentialnexttile; // it is the next tile
          sidehitfrom = LEFT;
          goto endcounterclockwisecircumventnexttilechecks;
        } // endif it's impassible
      } // endif you haven't hit the map edge
      insertnextwaypoint(new waypoint(currenttile->getx()+1,currenttile->gety()+1,size)); // bottom right
      currentwaypoint = currentwaypoint->getnextwaypoint();

    } // end forever

    endcounterclockwisecircumventnexttilechecks:;

  } // end while there are more tiles to step through

  return TRUE;

} // end circumvent (direction provided)

