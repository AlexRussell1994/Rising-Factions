/*
 * ActualObjects.h
 *
 *  Created on: 28/08/2015
 *      Author: Alex
 */

// includes
#include "GameDataHeader.h"

#ifndef ACTUALOBJECTS_H_
#define ACTUALOBJECTS_H_

class TownHall;
class Stockpile;
class Farm;
class Shelter;

class Tree;
class BerryBush;

class TapResources;
class WalkTo;
class Gather;
class Build;

class TownHall: public Building
{
private:

  double exchangerate;
  int numtilesoccupied;

  double woodtobuild;
  bool isbuilt;

public:
  TownHall(int newx,int newy, int newobjectID);
  ~TownHall();

  double exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired);

  double getexchangerate() {return exchangerate;}

  bool closeenough(int atx, int aty) {return (atx >= objectx and atx <= objectx + objectwidth and aty >= objecty and aty <= objecty + objectheight) ? TRUE : FALSE;}

  bool canaccept(ResourceType thetype) {return (thetype == WOOD) ? ((isbuilt == TRUE) ? FALSE : TRUE) : FALSE;}

  bool isfinished() {return isbuilt;}

  ObjectType gettype() {return TOWNHALL;}

  int getnumtilesoccupied() {return numtilesoccupied;}

  int getexchangex() {return objectx + 1;}
  int getexchangey() {return objecty + 1;}

}; // end TownHall

class Stockpile: public Building
{
private:

  double exchangerate;

  double woodtobuild;
  bool isbuilt;

  int amountfood;
  int amountwood;

public:
  Stockpile(int newx,int newy, int newobjectID);
  ~Stockpile();

  double exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired);

  bool closeenough(int atx, int aty) {return (atx >= objectx and atx <= objectx + objectwidth and aty >= objecty and aty <= objecty + objectheight) ? TRUE : FALSE;}

  double getexchangerate() {return exchangerate;}

  int getresourceamount(ResourceType oftype);

  bool canaccept(ResourceType thetype);

  bool isfinished() {return isbuilt;}

  ObjectType gettype() {return STOCKPILE;}

}; // end Stockpile

class Shelter: public Building
{
private:

  double exchangerate;

  double woodtobuild;
  bool isbuilt;

public:
  Shelter(int newx,int newy, int newobjectID);
  ~Shelter();

  double exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired);

  double getexchangerate() {return exchangerate;}

  bool canaccept(ResourceType thetype) {return (thetype == WOOD) ? ((isbuilt == TRUE) ? FALSE : TRUE) : FALSE;}

  bool isfinished() {return isbuilt;}

  ObjectType gettype() {return SHELTER;}

}; // end Shelter

class Farm: public Building
{
private:

  double exchangerate;

  double woodtobuild;
  bool isbuilt;

  double amountexchangedrecently;

public:
  Farm(int newx,int newy, int newobjectID);
  ~Farm();

  void update(int timesincelast);

  double exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired);

  bool closeenough(int atx, int aty) {return (atx >= objectx and atx <= objectx + objectwidth and aty >= objecty and aty <= objecty + objectheight) ? TRUE : FALSE;}

  double getexchangerate() {return exchangerate;}

  int getresourceamount(ResourceType oftype) {return (oftype == FOOD) ? ((isbuilt == TRUE) ? TRUE : FALSE) : FALSE;}

  bool canaccept(ResourceType thetype) {return (thetype == WOOD) ? ((isbuilt == TRUE) ? FALSE : TRUE) : FALSE;}

  bool isfinished() {return isbuilt;}

  ObjectType gettype() {return FARM;}

}; // end Farm


class Tree: public Object
{
private:

  double exchangerate;
  int numtilesoccupied;

  int amountwood;

public:
  Tree(int newx,int newy, int newobjectID);
  ~Tree();

  double exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired);

  bool closeenough(int atx, int aty) {return (atx >= objectx and atx <= objectx + objectwidth and aty >= objecty and aty <= objecty + objectheight) ? TRUE : FALSE;}

  int getexchangex() {return objectx + 1;}
  int getexchangey() {return objecty + 1;}

  double getexchangerate() {return exchangerate;}

  int getresourceamount(ResourceType oftype) {return (oftype == WOOD) ? amountwood : 0;}

  ObjectType gettype() {return TREE;}

  int getnumtilesoccupied() {return numtilesoccupied;}

}; // end Tree

class BerryBush: public Object
{
private:

  double exchangerate;

  double amountfood;

public:
  BerryBush(int newx,int newy, int newobjectID);
  ~BerryBush();

  void update(int timesincelast);

  double exchangeresources(double amountexchangedesired, ResourceType typeexchangedesired);

  bool closeenough(int atx, int aty) {return (atx >= objectx and atx <= objectx + objectwidth and aty >= objecty and aty <= objecty + objectheight) ? TRUE : FALSE;}

  double getexchangerate() {return exchangerate;}

  int getresourceamount(ResourceType oftype) {return (oftype == FOOD) ? amountfood : 0;}

  ObjectType gettype() {return BERRYBUSH;}

}; // end BerryBush


class TapResources: public Instruction
{
private:

  double amount; // the amount, positive or negative, that the person wants to provide
  ResourceType type;
  int objectIDtoexchangewith;

  double partialunittransfered;

public:
  TapResources(int newamount, ResourceType newtype, int newobjectID);
  ~TapResources();

  Instruction* performtask(Person* personcalling, int timeperforming);

}; // end TapResources

class WalkTo: public Instruction
{
private:

  waypoint* waypointlist;

public:
  WalkTo(waypoint* newwaypointlist);
  ~WalkTo();

  Instruction* performtask(Person* personcalling, int timeperforming);

}; // end WalkTo

class Gather: public Instruction
{
private:

  int objectIDgatherfrom;
  int objectIDgatherto;
  ResourceType typetogather;

public:
  Gather(int newobjectIDgatherfrom, ResourceType newtypetogather);
  ~Gather();

  Instruction* performtask(Person* personcalling, int timeperforming);

}; // end Gather

class Build: public Instruction
{
private:

  int objectIDbuildfrom;
  int objectIDbuildto;

public:
  Build(int newobjectIDbuildto);
  ~Build();

  Instruction* performtask(Person* personcalling, int timeperforming);

}; // end Build


#endif /* ACTUALOBJECTS_H_ */


