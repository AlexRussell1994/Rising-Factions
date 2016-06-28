/*
 *  Includes:
 *  The declarations for unit pathing
 *  And now the rest as well
 *
 *  Created on: 24/08/2015
 *      Author: Alex
 */

#ifndef GAMEDATAHEADER_H_
#define GAMEDATAHEADER_H_

#include <windows.h> // includes
#include <iostream>
#include <cmath>
#include "Debugging.h"
using namespace std;

const int gmapwidth = 1000, gmapheight = 1000; // the size of the map

enum SquareSide {NONE, LEFT, TOP, RIGHT, BOTTOM}; // for refering to a specific side of a tile
enum UnitSize {ONE = 1,MAXSIZE}; // an array of possible unit sizes
// ending on MAXSIZE (which is actually one larger than the largest unit)
enum ResourceType {FOOD, WOOD};
enum ObjectType {NONEOBJ, TOWNHALL, STOCKPILE, FARM, SHELTER, TREE, BERRYBUSH};
enum TileType {DIRT,GRASS,SHALLOWWATER,DEEPWATER,ROCK};

class waypoint; // refered to by another class before it is defined
class tile;
class unit;
class Object; // dude, your capitalization system sucks
class Building;
class Faction;
class Person;
class Instruction;

class Map
{
private:

  tile* tiles[gmapwidth][gmapheight]; // the base of the map
  int mapwidth;
  int mapheight;

  bool activemaploaded; // to prevent anything from being called on an unprepared map

  Object* objectLL;
  Faction* factionLL;

  int nextobjectID, nextfactionID, nextpersonID;

public:
  Map();
  ~Map();

  int getwidth() {return gmapwidth;} // global variables probably don't belong here.
  int getheight() {return gmapheight;} // change this if you revamp the program

  tile* gettile(int tilex,int tiley);

  void blocktile(int x,int y); // used to make tiles passable or inpassable
  void unblocktile(int x,int y); // no checks, so make sure you can call them before you do

  void updatemap(int timesincelastupdate); // updates all game data

  Faction* getfaction(int factionID);
  Object* getobject(int objectID);
  Object* getfirstobject() {return objectLL;}
  Faction* getfirstfaction() {return factionLL;}

  bool initialisegame(Object* newobjectLL, Faction* newfactionLL);
  // returns TRUE if successful or FALSE if not, but can be safely ignored
  // if it fails, it deletes the contents of its parameters itself
  // re-IDS the contents of its parameters
  // places them in the appropriate variables
  // and, if successful, sets activemaploaded to TRUE
  // people passed in through their respective factions will not keep their instructions
  // also tells the tiles their x and y values

  void endgame();
  // deletes everything active and puts the map back into standby (activemaploaded = FALSE)

  void addobject(Object* newobject); // how to add or remove objects from the game
  void removeobject(int objectID);

  int getnextpersonID() {return nextpersonID++;}
  int getnextobjectID() {return nextobjectID++;}
  int getnextfactionID() {return nextfactionID++;}

}; // end map

class tile
{
private:

  bool isblocked[MAXSIZE]; // remembers if this tile is passable for each unit size
  int x,y;
  TileType typeoftile;

public:
  tile();

  void setxy(int newx, int newy) {x = newx; y = newy;}

  bool istileinpassible(UnitSize size) {return isblocked[size];}

  int getx() {return x;}
  int gety() {return y;}

  TileType gettype() {return typeoftile;}
  void settype(TileType newtype) {typeoftile = newtype;}

  void makeinpassable() {isblocked[ONE] = TRUE;}
  void makepassable() {isblocked[ONE] = FALSE;}
  void updatepassable(); // updates what sizes can passs through this tile, based on what allows size ONE to pass

  void settiledata(tile* setto); // sets all data of the tile to match the new tile

}; // end tile

class unit // an x and y that can be manipulated generically and accessed by a class which it is a memeber of
{
private:

  double currentx, currenty; // the units current location
  UnitSize size; // the unit's size

public:

  waypoint* pathto(double x, double y);
  // creates a routenode LL and the first waypoint chain for that route
  // finds and returns the waypoint chain that gets a unit of the provided unit's size
  // from the proveded unit's location to the target x and y via an unobstructed path

  double* getxref() {return &currentx;}
  double* getyref() {return &currenty;}

  double getx() {return currentx;}
  double gety() {return currenty;}

  UnitSize getsize() {return size;}


}; // end unit

class waypoint // an x and y destination that a unit might attempt to walk to
// it is also its own node in a (2 direction) linked list, creating a multi-waypoint path
// and has a pointer to the routenode that it is a potential path for
{ // and it holds the size of the unit that the path is being found for
private:

  double x,y; // the location of this specific waypoint on the map
  waypoint* lastwaypoint; // the next and last waypoints in the linked list
  waypoint* nextwaypoint;
  UnitSize size;
  double tracenextx, tracenexty; // for checking that trace is not called twice with the same parameters

  bool subtrace(waypoint* waypointto, tile** tilehitfrom, SquareSide& sidehitfrom, tile** tilecollectiontohit);
  // called by trace
  // traces from this waypoint to the parameter waypoint and returns info on what it hits
  // also returns 1 for success or 0 for a collision
  // if tilecollectiontohit is not NULL, then subtrace can only collide with an a tile connected by
  // obstructed tiles to the tile it points to

  bool didcollideline(tile* tilepointer1,tile* tilepointer2, waypoint* waypointto, tile** thetilehit, SquareSide& directionhit);
  // returns TRUE if both potential tiles are impassible, and FALSE if either is passable
  // if TRUE, it also returns (via parameter) the tile hit and the side it was hit from
  // if a colission already occured, it retirns TRUE and leaves the collision data unchanged
  // it also takes into accoutn that a NULL parameter (map wall) could be passed to it
  // it counts as a colission for the "were both collisions" but is never returned as colission data

  bool didcollide(tile* tilepointer, waypoint* waypointto, tile** thetilehit , SquareSide& directionhit);
  // returns TRUE if the tile is impassible and FALSE if not
  // if TRUE, it also returns (via parameter) the tile hit and the side it was hit from
  // it figures out the side hit via the waypointto parameter (going from current waypoint there)
  // if a colission already occured, it retirns TRUE and leaves the collision data unchanged

  bool sameobstacle(tile* thetilehit,SquareSide directionhit,tile* tilecollectiontohit);
  // returns TRUE if the tiles are part of the same obstruction or FALSE if they are not

public:

  waypoint(double newx,double newy, UnitSize newsize);
  waypoint(double newx,double newy, UnitSize newsize, int newtracenextx, int newtracenexty);
  ~waypoint();

  void insertnextwaypoint(waypoint* newnextwaypoint); // adds nextwaypoint to the waypoint chain after this waypoint
  waypoint* removenextwaypoint(); // removes the next waypoint from the waypoint chain
  void setnextwaypoint(waypoint* newnextwaypoint) {nextwaypoint = newnextwaypoint;}
  void setlastwaypoint(waypoint* newlastwaypoint) {lastwaypoint = newlastwaypoint;}
  waypoint* getnextwaypoint() {return nextwaypoint;}
  waypoint* getlastwaypoint() {return lastwaypoint;}
  int getx() {return x;}
  int gety() {return y;}
  double getdoublex() {return x;}
  double getdoubley() {return y;}
  UnitSize getsize() {return size;}

  double sumroutelength(); // returns the length of the route from here to the end of the waypointchain

  waypoint* duplicatewaypointlist(waypoint* waypointactivetrace); // returns a duplicate of the remainder of the waypoint chain
  // should only be called on the first waypoint of a chain (unless duplicating only a latter section)
  // only the waypoint pointed to by the parameter will remember its last trace call

  bool path();
  // this is a recursive algorithm that ajusts the waypoint linked list it recieves to be a complete,
  // unobstructed path that a unit can follow from the waypoint it is provided to the next waypoint
  // held in the linked list. It does not remove or affect the linked list beyond reaching the next waypoint
  // it returns true if it succeeds or false if it fails (and the routenode should be discarded)

  int trace(tile** tilehitfrom, SquareSide& sidehitfrom, tile** tilehitto, SquareSide& sidehitto);
  // this function checks every point along a direct line from the waypoint to the next waypoint
  // if it hits impassible terrain it also traces back from the next waypoint toward this one
  // until it hits the same obstruction, and places the data of where each hit in its parameters
  // it returns 0 if it hits an obstruction and 1 if it reaches the next waypoint without
  // hitting obstructed terrain
  // it returns -1 if the path should be abondoned
  // this is for when the same trace is called by the same waypoint twice in a row
  // which would cause an infinite loop when navagating entirely within an obstruction
  bool checkpassable();
  // this is a version of trace which only returns success or failure

  bool circumvent(SquareSide& attempted, tile& tilehitfrom, SquareSide sidehitfrom, tile& tilehitto, SquareSide sidehitto);
  // this function follows the edge of an object, adding every convex point to the waypoint list
  // until it reaches the side a provided by a trace back to the same object
  // fills "attempted" with the direction that the first adjacent impassible tile was found in
  // and then goes clockwise if no direction is provided(above) or counterclockwise othwise(below)
  // returns TRUE if it finds a path or FALSE if it fails to find a path
  bool circumvent(tile& tilehitfrom, SquareSide sidehitfrom, tile& tilehitto, SquareSide sidehitto, SquareSide directionalreadytried);
  // goes counterclockwise
  // and is not meant to add new routenodes to the routenode chain

  bool anglereduction();
  // is only called on the first waypoint of a chain (and finds the first waypoint anyway)
  // removes all waypoints where the unit would turn in such a way as it would not need to hug the corner
  // i.e. relative to the starting vector, the new vector is on the opposite side of the
  // starting vector from the impassible terrain
  // this function removes these waypoint from the entire list (forward and backward) repeatedly
  // until it can pass through the entire list without finding any such waypoints
  // do not call it on single-waypoint-long-chains
  // returns TRUE if it succeeds or FALSE if the waypoint list should be abandoned

}; // end waypoint

class routenode // a linked list of waypoint chains
{ // that holds all potential ways of getting to the destination
private:

  waypoint* firstwaypoint; // the starting position of the route

  routenode* nextroutenode; // the next routenode in the routenode sequence

  bool hascalculatedlength; // only calculate the route once
  int length; // holds the length of the route once calculated

  SquareSide alreadychecked; // holds the direction already checked for the next split
  waypoint* startingpartwaythrough; // holds the next section of the path to check


public:
  routenode(waypoint* thisnodeswaypoint);
  routenode(waypoint* thisnodeswaypoint, waypoint* thewaypointbeingchecked, SquareSide directionalreadytried);
  ~routenode();

  routenode* getnextroutenode() {return nextroutenode;}
  void setnextroutenode(routenode* newnextroutenode) {nextroutenode = newnextroutenode;}
  waypoint* releasewaypointchain(); // returns the waypoint chain and sets firstwaypoint to NULL

  int getlength();
  // firgures out the route to the final waypoint in the chain, changing the chain to reflect the new route
  // it must be something that can be called multiple times but only calculates once
  // and it returns -1 if the route fails

  void addroutetolist(waypoint* newfirstwaypoint, waypoint* fromhere, SquareSide directionalreadytried);
  // adds the way to the target described to the list of ways to the target to be tried


}; // end routenode


class Object
{
protected:

  Object* nextobject;

  int objectID;
  ObjectType objecttype;

  int objectx, objecty;
  int objectwidth, objectheight;

public:
  Object(int newx,int newy, int newobjectID); // location is the upper left corner of the smallest possible bounding box
  virtual ~Object();

  void updateall(int timesincelast);
  virtual void update(int timesincelast){}

  virtual double exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired) {return 0;}
  virtual bool closeenough(int atx, int aty) {return TRUE;}
  virtual int getexchangex() {return objectx + (objectwidth / 2);}
  virtual int getexchangey() {return objecty + (objectheight / 2);}
  int getx() {return objectx;}
  int gety() {return objecty;}
  int getwidth() {return objectwidth;}
  int getheight() {return objectheight;}
  virtual double getexchangerate() {return 1000;}
  virtual int getresourceamount(ResourceType oftype) {return 0;}
  virtual bool canaccept(ResourceType thetype) {return FALSE;}
  virtual bool isfinished() {return TRUE;}
  virtual ObjectType gettype() {return objecttype;}

  void setID(int newobjectID) {objectID = newobjectID;}
  int getID() {return objectID;}

  Object* getnextobject() {return nextobject;}
  void setnextobject(Object* newobject) {nextobject = newobject;}

}; // end Object

class Building: public Object
{
protected:

public:
  Building(int newx,int newy, int newobjectID);
  virtual ~Building(){}

  virtual void update(int timesincelast){} // pass it on

}; // end Building

class Faction
{
private:

  int los[gmapwidth][gmapheight]; // 0 is not revealed, 1 has been revealed, 2 is currently revealed

  Person* personLL;

  Faction* nextfaction;

  int factionID;

public:
  Faction(int newfactionID);
  ~Faction();

  Faction* getnextfaction() {return nextfaction;}

  void updateall(int timesincelast); // updates all of the factions from here on in the factionLL
  void update(int timesincelast); // updates this faction an its constituent people (fine! "people," not "persons")

  Person* getperson(int personID);

  void addtolos(int sourcex, int sourcey, int sightrange);

  void setID(int newfactionID) {factionID = newfactionID;}
  int getfactionID() {return factionID;}

  void addperson(Person* newperson);
  void removeperson(int personID);
  Person* getfirstperson() {return ((DISABLE_FACTIONPERSONUSE == TRUE) ? NULL : personLL);} // DEBUG

}; // end Faction

class Person
{
private:

  int inventorycontents; // inventory contents
  int maxinventory;
  ResourceType inventorytype;

  unit personsunit; // movement facilitating object

  Instruction* instructionLL; // what to do
  waypoint* nextwaypoint; // handled by instructions
  double personspeed; // units moved per millisecond

  int sightrange;

  Person* nextperson; // the person linked list

  int personID;
  int owningfactionID;

public:
  Person(int newx, int newy);
  ~Person();

  void updateall(int timesincelast); // updates the rest of the people in the personLL
  void update(int timesincelast); // updates this person

  Person* getnextperson() {return nextperson;}
  void setnextperson(Person* newperson) {nextperson = newperson;}
  // no checks; be careful using it

  int move(int movement);
  // causes the person to move toward nextwaypoint at personspeed
  // returns 0 if it does so and must move further
  // returns 1 if it reaches its destination
  // returns -1 if the movement failed and pathing must be recalculated
  // returns -2 if the movement failed and the ultimate target is unreachable

  void setnextwaypoint(waypoint* newnextwaypoint) {nextwaypoint = newnextwaypoint;}

  void addinstructiontochain(Instruction* newinstruction); // how a person recieves new instructions
  void insertnewfirstinstruction(Instruction* newinstruction);
  void replaceinstructionchain(Instruction* newinstruction);

  void setIDs(int newpersonID, int newowningfactionID);
  int getpersonID() {return personID;}

  double getx() {return personsunit.getx();}
  double gety() {return personsunit.gety();}
  int getsightrange() {return sightrange;}
  double getmovementspeed() {return personspeed;}

  int getresourceamount(ResourceType type);
  int getmaxresourceamount() {return maxinventory;}
  ResourceType getresourcetype() {return inventorytype;}
  void setresourceamount(int newamount, ResourceType type){inventorycontents = newamount;inventorytype = type;}

  waypoint* pathto(double x, double y) {return personsunit.pathto(x,y);}

}; // end Person

class Instruction
{
  // Instruction is an own-node linked list of tasks that a person goes about perfroming

protected:

  Instruction* nextinstruction;

public:
  Instruction();
  virtual ~Instruction();

  Instruction* getnextinstruction() {return nextinstruction;}
  void setnextinstruction(Instruction* newinstruction) {nextinstruction = newinstruction;}
  // this does not check if it has a next instruction already; do not call it if it does

  virtual Instruction* performtask(Person* personcalling, int timeperforming){Instruction* next = nextinstruction;nextinstruction = NULL;return next;}
  // this is how an instruction is carried out
  // if this function does not return NULL, it returns the location of the next instruction along the linked list
  // this indicates that this insrtuction itseld has been removed from the linked list and should be deleted
  // deleting an instruction without removing it from the list (as this does when it returns non-NULL)
  // delets the entire remainder of the linked list after the instruction deleted

}; // end Instruction

#endif /* GAMEDATAHEADER_H_ */
