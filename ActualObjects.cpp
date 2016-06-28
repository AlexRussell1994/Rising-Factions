/*
 * ActualObjects.cpp
 *
 *  Created on: 28/08/2015
 *      Author: Alex
 */

// includes
#include "GameDataHeader.h"
#include "ActualObjects.h"
#include "Debugging.h"

// the map
extern Map map;

#define t(a,b) map.blocktile(map.gettile(newx + a, newy + b)->getx(),map.gettile(newx + a, newy + b)->gety())

#define u(a,b) map.unblocktile(map.gettile(objectx + a, objecty + b)->getx(),map.gettile(objectx + a, objecty + b)->gety())

TownHall::TownHall(int newx,int newy, int newobjectID): Building(newx,newy,newobjectID)
{
  exchangerate = 0.0015;
  numtilesoccupied = 9;

  woodtobuild = 250;
  isbuilt = FALSE;

  objectwidth = 5;
  objectheight = 5;

  t(1,1),t(1,2),t(1,3),t(2,1),t(2,2),t(2,3),t(3,1),t(3,2),t(3,3);

  // do not ccreate one of these anywhere it will include tiles off of the map
} // end constructor

TownHall::~TownHall()
{
  u(1,1),u(1,2),u(1,3),u(2,1),u(2,2),u(2,3),u(3,1),u(3,2),u(3,3);
} // end destructor

double TownHall::exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired)
{
  if (isbuilt == TRUE)
    return 0;

  if (typeexchangedesired != WOOD)
    return 0;

  woodtobuild -= amountexchangedesired;
  if (woodtobuild <= 0) // if done
  {
    isbuilt = TRUE;
    return amountexchangedesired + woodtobuild;
  } // end if done

  return amountexchangedesired;
} // end (townhall) exchangeresources

Stockpile::Stockpile(int newx,int newy, int newobjectID): Building(newx,newy,newobjectID)
{
  objectwidth = 10;
  objectheight = 10;

  exchangerate = 0.003;

  woodtobuild = 100;
  isbuilt = FALSE;

  t(0,0),t(0,1),t(0,2),t(0,3),t(0,4),t(0,5),t(0,6),t(0,7),t(0,8),t(0,9),t(1,0),t(2,0),t(1,9),t(2,9),t(7,0),t(8,0),t(7,9),t(8,9),t(9,0),t(9,1),t(9,2),t(9,3),t(9,4),t(9,5),t(9,6),t(9,7),t(9,8),t(9,9);

  amountfood = 0;
  amountwood = 0;

} // end constructor

Stockpile::~Stockpile()
{
  u(0,0),u(0,1),u(0,2),u(0,3),u(0,4),u(0,5),u(0,6),u(0,7),u(0,8),u(0,9),u(1,0),u(2,0),u(1,9),u(2,9),u(7,0),u(8,0),u(7,9),u(8,9),u(9,0),u(9,1),u(9,2),u(9,3),u(9,4),u(9,5),u(9,6),u(9,7),u(9,8),u(9,9);
} // end destructor

double Stockpile::exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired)
{
  if (isbuilt == FALSE) // is not yet built
  {
    if (typeexchangedesired != WOOD)
      return 0;

    woodtobuild -= amountexchangedesired;
    if (woodtobuild <= 0) // if done
    {
      isbuilt = TRUE;
      exchangerate = 0.01;
      return amountexchangedesired + woodtobuild;
    } // end if done

    return amountexchangedesired;
  }
  else // is built
  {
    if (typeexchangedesired == FOOD) // if food
    {
      amountfood += amountexchangedesired;
      if (amountfood > 100)
      {
        int temp = amountfood - 100;
        amountfood = 100;
        return amountexchangedesired - temp;
      } else if (amountfood < 0)
      {
        int temp = amountfood;
        amountfood = 0;
        return amountexchangedesired - temp;
      } else
        return amountexchangedesired;
    } else if (typeexchangedesired == WOOD) // if wood
    {
      amountwood += amountexchangedesired;
      if (amountwood > 100)
      {
        int temp = amountwood - 100;
        amountwood = 100;
        return amountexchangedesired - temp;
      } else if (amountwood < 0)
      {
        int temp = amountwood;
        amountwood = 0;
        return amountexchangedesired - temp;
      } else
      return amountexchangedesired;
    } // end if food/wood

  } // end if is built yet

  return 0; // this should never be reached
} // end (stockpile) exchangeresources

int Stockpile::getresourceamount(ResourceType oftype)
{
  if (isbuilt == FALSE)
   return 0;

  if (oftype == WOOD)
    return amountwood;
  else // is food
    return amountfood;
} // end (stockpile) getresourceamount

bool Stockpile::canaccept(ResourceType thetype)
{
  if (isbuilt == FALSE)
  {
    if (thetype == WOOD)
      return TRUE;
    else
      return FALSE;
  }

  if (thetype == WOOD)
    if (amountwood < 100)
      return TRUE;
  if (thetype == FOOD)
    if (amountfood < 100)
      return TRUE;
  return FALSE;
} // end (stockpile) canaccept

Shelter::Shelter(int newx,int newy, int newobjectID): Building(newx,newy,newobjectID)
{
  exchangerate = 0.002;

  woodtobuild = 40;
  isbuilt = FALSE;

  objectwidth = 5;
  objectheight = 5;

  t(0,1),t(0,2),t(0,3),t(1,0),t(2,0),t(3,0),t(4,1),t(4,2),t(4,3);

} // end constructor

Shelter::~Shelter()
{
  u(0,1),u(0,2),u(0,3),u(1,0),u(2,0),u(3,0),u(4,1),u(4,2),u(4,3);
} // end destructor

double Shelter::exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired)
{
  if (isbuilt == TRUE)
     return 0;

   if (typeexchangedesired != WOOD)
     return 0;

   woodtobuild -= amountexchangedesired;
   if (woodtobuild <= 0) // if done
   {
     isbuilt = TRUE;
     return amountexchangedesired + woodtobuild;
   } // end if done

   return amountexchangedesired;
} // end (shelter) exchangeresources

Farm::Farm(int newx,int newy, int newobjectID): Building(newx,newy,newobjectID)
{
  exchangerate = 0.0005;

  woodtobuild = 80;
  isbuilt = FALSE;

  objectwidth = 10;
  objectheight = 10;

  amountexchangedrecently = 0;
} // end constructor

Farm::~Farm()
{
  // nothing needs doing here yet
} // end destructor

void Farm::update(int timesincelast)
{
  if (isbuilt == TRUE) // if is built
  {
    timesincelast = (amountexchangedrecently < 10000) ? timesincelast : 9999;

    amountexchangedrecently = amountexchangedrecently * (1 - (timesincelast / 10000));

    exchangerate = (0.0005 < 5 / amountexchangedrecently) ? 0.0005 : 5 / amountexchangedrecently ;
  } // end if is built
} // end (farm) update

double Farm::exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired)
{
  if (isbuilt == FALSE)
  {
    if (typeexchangedesired != WOOD)
      return 0;

    woodtobuild -= amountexchangedesired;
    if (woodtobuild <= 0) // if done
    {
      isbuilt = TRUE;
      exchangerate = 0.0005;
      return amountexchangedesired + woodtobuild;
    } // end if done

    return amountexchangedesired;
  } else // is built
  {
    amountexchangedrecently += amountexchangedesired;
    return amountexchangedesired;
  } // end if is built yet

} // end (farm) exchangeresources

Tree::Tree(int newx,int newy, int newobjectID): Object(newx,newy,newobjectID)
{
  objectwidth = 3;
  objectheight = 3;

  amountwood = 50;
  exchangerate = 0.002;

  numtilesoccupied = 1;

  t(1,1);

} // end construcotr

Tree::~Tree()
{
  u(1,1);
} // end destructor

double Tree::exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired)
{
  if (typeexchangedesired != WOOD)
    return 0;

  amountwood += amountexchangedesired;

  if (amountwood <= 0)
  {
    int temp = amountwood;
    amountwood = 0;
    return amountexchangedesired - temp;
  }

  return amountexchangedesired;
} // end (tree) exchangeresources

BerryBush::BerryBush(int newx,int newy, int newobjectID):Object(newx,newy,newobjectID)
{
  objectwidth = 5;
  objectheight = 5;

  amountfood = 100;
  exchangerate = 0.002;
} // end constructor

BerryBush::~BerryBush()
{
  // nothing to do here yet
} // end destrucotr

void BerryBush::update(int timesincelast)
{
  amountfood += timesincelast * 0.0005;
  if (amountfood > 100)
    amountfood = 100;
} // end (bush) update

double BerryBush::exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired)
{
  if (typeexchangedesired != FOOD)
    return 0;

  amountfood += amountexchangedesired;

  if (amountfood <= 0)
  {
    int temp = amountfood;
    amountfood = 0;
    return amountexchangedesired - temp;
  }

  return amountexchangedesired;
} // end (bush) exchangeresources

TapResources::TapResources(int newamount, ResourceType newtype, int newobjectID)
{
  amount = newamount;
  type = newtype;
  objectIDtoexchangewith = newobjectID;
  partialunittransfered = 0;
} // end constructor

TapResources::~TapResources()
{

} // end destructor

Instruction* TapResources::performtask(Person* personcalling, int timeperforming)
{
  Object* targetobject = map.getobject(objectIDtoexchangewith);

  if (personcalling->getresourcetype() != type)
    personcalling->setresourceamount(0,type);

  if (targetobject == NULL) // if the object to work with has been destroyed
  {
    Instruction* temp = nextinstruction; // remove this instruction
    nextinstruction = NULL;
    return temp;
  } // end if the object to work with has been destroyed

  bool returnvalue = targetobject->closeenough(personcalling->getx(),personcalling->gety());

  if (returnvalue == FALSE) // if you are not yet close enough to exchange resources
  {
    waypoint* waypointpointer = personcalling->pathto(targetobject->getexchangex(),targetobject->getexchangey());
    if (waypointpointer == NULL) // if you cannot got to the object
    { // abandon this instruction
      Instruction* temp = nextinstruction; // remove this instruction
      nextinstruction = NULL;
      return temp;
    } else // if you can get to the object
    {
      Instruction* newinstruction = new WalkTo(waypointpointer); // create an instruction to do so
      personcalling->insertnewfirstinstruction(newinstruction);
      return NULL;
    }

  } else // if you are close enough to exchange resources
  {

    // determine max exchange desired
    double amounttoexchange = 0;
    if (amount == 0) // if you have nothing to exchange
    {
      Instruction* temp = nextinstruction; // destroy this instruction
      nextinstruction = NULL;
      return temp;
    } // end if you have nothing to exchange
    amounttoexchange = amount * targetobject->getexchangerate();
    if (personcalling->getresourceamount(type) + amount < 0) // if you don't have that many resources
      amounttoexchange = personcalling->getresourceamount(type);
    double returnvalue = targetobject->exchangeresources(amount, type);
    amount -= returnvalue;
    partialunittransfered += returnvalue;
    personcalling->setresourceamount(personcalling->getresourceamount(type) - partialunittransfered, type);
    partialunittransfered -= ((double)((int)partialunittransfered));
    if (amounttoexchange != returnvalue) // if it can't accept that many
      {
        Instruction* temp = nextinstruction; // destroy this instruction
        nextinstruction = NULL;
        return temp;
      }
  } // end if are you close enough to exchange resources

  return NULL;
} // end (TapResources) performtask

WalkTo::WalkTo(waypoint* newwaypointlist)
{
  if (DEBUG == TRUE)
    cout << "WalkTo created.\n";
  waypointlist = newwaypointlist;
} // end constructor

WalkTo::~WalkTo()
{
  if (DEBUG == TRUE)
    cout << "WalkTo deleted.\n";
  if (waypointlist != NULL)
    delete waypointlist;

} // end destructor

Instruction* WalkTo::performtask(Person* personcalling, int timeperforming)
{
  if (DEBUG == TRUE)
    cout << "WalkTo performtask called.\n";

  personcalling->setnextwaypoint(waypointlist);

  if (waypointlist == NULL)
  {
    Instruction* temp = nextinstruction;
    nextinstruction = NULL;
    return temp;
  }

  waypoint* wayptr = NULL;
  Instruction* temp = NULL;

  int returnvalue = personcalling->move(timeperforming * (personcalling->getmovementspeed()));
  if (DEBUG == TRUE)
    cout << "Person::move returned:" << returnvalue << "\n";
  switch(returnvalue)
  {
    case 1: // you've reached your next waypoint
      if (waypointlist->getnextwaypoint() == NULL) // if that's the end of the WalkTo
      {
        Instruction* temp = nextinstruction;
        nextinstruction = NULL;
        return temp;
      } else // if that's not the end of WalkTo
      {
        waypoint* temp = waypointlist; // remove the first waypoint of the list
        waypointlist = temp->getnextwaypoint();
        waypointlist->setlastwaypoint(NULL);
        temp->setnextwaypoint(NULL);
        delete temp;
        return NULL;
      }
    break;

    case -1: // your path must be recalculated
      wayptr = waypointlist;
      while (wayptr->getnextwaypoint() != NULL)
        wayptr = wayptr->getnextwaypoint();
      waypointlist = personcalling->pathto(wayptr->getdoublex(),wayptr->getdoubley());
      if (waypointlist == NULL)
      {
        Instruction* temp = nextinstruction;
        nextinstruction = NULL;
        return temp;
      }
    break;
    case -2: // the target is now unreachable
      temp = nextinstruction;
      nextinstruction = NULL;
      return temp;
    break;
    default:
    break;
  }

  return NULL;
} // end (WalkTo) performatask

Gather::Gather(int newobjectIDgatherfrom, ResourceType newtypetogather)
{
  objectIDgatherfrom = newobjectIDgatherfrom;
  typetogather = newtypetogather;

  Object* objectfrom = map.getobject(objectIDgatherfrom);
  Object* objectpointer = NULL;
  int shortestdistancetoobject = -1;

  if (objectfrom == NULL) // if there is not such object
  {
    objectIDgatherfrom = -1; // guarantee destruction of Gather on first update call
  } else // if there is a such object
  { // find object to gather to
    objectpointer = map.getfirstobject();
    while (objectpointer != NULL) // for each object
    {
      if (objectpointer->gettype() == STOCKPILE and objectpointer->isfinished() == TRUE and objectpointer->canaccept(typetogather)) // if this is an appropriate object
      {
        if (shortestdistancetoobject == -1 or shortestdistancetoobject > sqrt((objectfrom->getexchangex() - objectpointer->getexchangex())*(objectfrom->getexchangex() - objectpointer->getexchangex())+(objectfrom->getexchangey() - objectpointer->getexchangey())*(objectfrom->getexchangey() - objectpointer->getexchangey())))
        { // if this object is the closest yet
          shortestdistancetoobject = sqrt((objectfrom->getexchangex() - objectpointer->getexchangex())*(objectfrom->getexchangex() - objectpointer->getexchangex())+(objectfrom->getexchangey() - objectpointer->getexchangey())*(objectfrom->getexchangey() - objectpointer->getexchangey()));
          objectIDgatherto = objectpointer->getID();
        } // end if this object is the closest yet
      } // end if this is an appropriate object
      objectpointer = objectpointer->getnextobject();
    } // end for each object
    if (shortestdistancetoobject == -1) // if no object  was found
    {
      objectIDgatherto = -1;
      objectIDgatherfrom = -1; // guarantee destruction of Gather on first update call
    } // end if no object  was found
  } // end find object to gather to

} // end constructor

Gather::~Gather()
{

} // end destructor

Instruction* Gather::performtask(Person* personcalling, int timeperforming)
{

  if (objectIDgatherfrom == -1) // if there is no object to gather from
  {
    Instruction* temp = nextinstruction; // remove this instruction
    nextinstruction = NULL;
    return temp;
  } // end if the object to work with never existed

  Object* targetobject = map.getobject(objectIDgatherfrom);

  if (targetobject == NULL) // if the object to work with has been destroyed
  {
    Instruction* temp = nextinstruction; // remove this instruction
    nextinstruction = NULL;
    return temp;
  } // end if the object to work with has been destroyed

  if (personcalling->getresourceamount(typetogather) < personcalling->getmaxresourceamount()) // you need resources
  {
    if (targetobject->getresourceamount(typetogather) == 0) // the target has none
    {
       Instruction* temp = nextinstruction; // remove this instruction
       nextinstruction = NULL;
       return temp;
     }

    Instruction* newinstruction = NULL; // exchange with it
    int amount = personcalling->getresourceamount(typetogather) - personcalling->getmaxresourceamount();
    newinstruction = new TapResources(amount,typetogather,targetobject->getID());
    personcalling->insertnewfirstinstruction(newinstruction);

    return NULL;

  } else // you do not need resources
  {
    targetobject = map.getobject(objectIDgatherto);
    if (targetobject != NULL)
    {
      if (targetobject->canaccept(typetogather) == FALSE)
        targetobject = NULL;
    }

    if (targetobject == NULL) // the target can accept none
    {
      Object* objptr = map.getfirstobject(); // find another target
      int shortestdistance = -1;
      while (objptr != NULL) // for each object
      {

        if (objptr-> canaccept(typetogather))
        {
          if (shortestdistance == -1 or shortestdistance > sqrt((objptr->getexchangex()-personcalling->getx())*(objptr->getexchangex()-personcalling->getx())+(objptr->getexchangey()-personcalling->gety())*(objptr->getexchangey()-personcalling->gety())))
            {
              shortestdistance = sqrt((objptr->getexchangex()-personcalling->getx())*(objptr->getexchangex()-personcalling->getx())+(objptr->getexchangey()-personcalling->gety())*(objptr->getexchangey()-personcalling->gety()));
              targetobject = objptr;
            }
        }

        objptr = objptr->getnextobject();
      } // end for each object

      if (shortestdistance == -1) // if there is no object to gather from
      {
        Instruction* temp = nextinstruction; // remove this instruction
        nextinstruction = NULL;
        return temp;
      } // end if there is no object to gather from

      objectIDgatherto = targetobject->getID();
    }

    Instruction* newinstruction = NULL; // exchange with it
    int amount = personcalling->getresourceamount(typetogather);
    newinstruction = new TapResources(amount,typetogather,targetobject->getID());
    personcalling->insertnewfirstinstruction(newinstruction);

  } // end if you need resources

  return NULL;
} // end (Gather) performtask

Build::Build(int newobjectIDbuildto)
{
  objectIDbuildto = newobjectIDbuildto;

  Object* objectto = map.getobject(objectIDbuildto);
  Object* objectpointer = NULL;
  int shortestdistancetoobject = -1;

  if (objectto == NULL) // if there is not such object
  {
    objectIDbuildto = -1; // guarantee destruction of Build on first update call
  } else // if there is a such object
  { // find object to build from
    objectpointer = map.getfirstobject();
    while (objectpointer != NULL) // for each object
    {
      if (((objectpointer->gettype() == STOCKPILE and objectpointer->isfinished() == TRUE) or objectpointer->gettype() == TREE) and objectpointer->getresourceamount(WOOD) > 0) // if this is an appropriate object
      {
        if (shortestdistancetoobject == -1 or shortestdistancetoobject > sqrt((objectto->getexchangex() - objectpointer->getexchangex())*(objectto->getexchangex() - objectpointer->getexchangex())+(objectto->getexchangey() - objectpointer->getexchangey())*(objectto->getexchangey() - objectpointer->getexchangey())))
        { // if this object is the closest yet
          shortestdistancetoobject = sqrt((objectto->getexchangex() - objectpointer->getexchangex())*(objectto->getexchangex() - objectpointer->getexchangex())+(objectto->getexchangey() - objectpointer->getexchangey())*(objectto->getexchangey() - objectpointer->getexchangey()));
          objectIDbuildfrom = objectpointer->getID();
        } // end if this object is the closest yet
      } // end if this is an appropriate object
      objectpointer = objectpointer->getnextobject();
    } // end for each object
    if (shortestdistancetoobject == -1) // if no object  was found
    {
      objectIDbuildfrom = -1;
      objectIDbuildto = -1; // guarantee destruction of Build on first update call
    } // end if no object  was found
  } // end find object to gather to

} // end constructor

Build::~Build()
{

} // end destructor

Instruction* Build::performtask(Person* personcalling, int timeperforming)
{
  if (objectIDbuildto == -1) // if there is no object to build to
  {
    Instruction* temp = nextinstruction; // remove this instruction
    nextinstruction = NULL;
    return temp;
  } // end if the object to work with never existed

  Object* targetobject = map.getobject(objectIDbuildto);

  if (targetobject == NULL) // if the object to work with has been destroyed
  {
    Instruction* temp = nextinstruction; // remove this instruction
    nextinstruction = NULL;
    return temp;
  } // end if the object to work with has been destroyed

  if (personcalling->getresourceamount(WOOD) != 0) // you have resources
  {
    if (targetobject->isfinished() == TRUE) // the target no longer needs building
    {
       Instruction* temp = nextinstruction; // remove this instruction
       nextinstruction = NULL;
       return temp;
     }

    Instruction* newinstruction = NULL; // exchange with it
    int amount = personcalling->getresourceamount(WOOD);
    newinstruction = new TapResources(amount,WOOD,targetobject->getID());
    personcalling->insertnewfirstinstruction(newinstruction);

    return NULL;

  } else // you need resources
  {
    targetobject = map.getobject(objectIDbuildfrom);
    if (targetobject != NULL)
    {
      if (targetobject->getresourceamount(WOOD) <= 0)
        targetobject = NULL;
    }

    if (targetobject == NULL) // the target can provide no wood
    {
      Object* objptr = map.getfirstobject(); // find another target
      int shortestdistance = -1;
      while (objptr != NULL) // for each object
      {

        if (objptr->getresourceamount(WOOD) >= 0)
        {
          if (shortestdistance == -1 or shortestdistance > sqrt((objptr->getexchangex()-personcalling->getx())*(objptr->getexchangex()-personcalling->getx())+(objptr->getexchangey()-personcalling->gety())*(objptr->getexchangey()-personcalling->gety())))
            {
              shortestdistance = sqrt((objptr->getexchangex()-personcalling->getx())*(objptr->getexchangex()-personcalling->getx())+(objptr->getexchangey()-personcalling->gety())*(objptr->getexchangey()-personcalling->gety()));
              targetobject = objptr;
            }
        }

        objptr = objptr->getnextobject();
      } // end for each object

      if (shortestdistance == -1) // if there is no object to gather from
      {
        Instruction* temp = nextinstruction; // remove this instruction
        nextinstruction = NULL;
        return temp;
      } // end if there is no object to gather from

      objectIDbuildfrom = targetobject->getID();
    }

    Instruction* newinstruction = NULL; // exchange with it
    int amount = personcalling->getresourceamount(WOOD) - personcalling->getmaxresourceamount();
    newinstruction = new TapResources(amount,WOOD,targetobject->getID());
    personcalling->insertnewfirstinstruction(newinstruction);

  } // end if you need resources

  return NULL;

} // end (Build) performtask



