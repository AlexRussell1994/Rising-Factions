/*
 * InterfaceHeader.h
 *
 *  Created on: 1/09/2015
 *      Author: Alex
 */

#ifndef INTERFACEHEADER_H_
#define INTERFACEHEADER_H_

#include "DrawToWindowheader.h"
#include "GameDataHeader.h"



enum ScreenID {MAINMENU, INGAME};
enum DisplayNodeTarget {NOTARGET, OTHER, PERSON, OBJECT};

class displaynode;

class Interface
{
private:

  void mainmenuout();
  void mainmenuin();
  void gameout();
  void gameprocessing();
  void gamein();

  HWND themainwindow; // for use in destroying the window and ending the program

  ScreenID currentscreen;

  // game screen variables
  double screenx, screeny;
  Person* personselected;
  bool menuup;
  displaynode* displaynodeLL;
  bool godmode;

  // pics
  Pic* mainmenubackround;
  Pic* newgamebutton;
  Pic* exitgamebutton;
  Pic* blackbox;
  Pic* gameborder;
  Pic* menubox;
  Pic* mainmenubutton;
  Pic* gameborderperson;
  Pic* woodicon;
  Pic* foodicon;
  Pic* dirttile;
  Pic* grasstile;
  Pic* shallowwatertile;
  Pic* deepwatertile;
  Pic* rocktile;
  Pic* townhallimg;
  Pic* stockpileimg;
  Pic* shelterimg;
  Pic* farmimg;
  Pic* treeimg;
  Pic* berrybushimg;
  Pic* personimg;
  Pic* godmodeimg;
  Pic* underconstructionimg;

  void gettilexy(double scrx,double scry,double& tilex,double& tiley);
  void getscrxy(double tilex,double tiley,double& scrx,double& scry);

  void drawdisplaynode(displaynode* thedisplaynode);

public:
  Interface();
  ~Interface();

  void initialize(HWND);

  void update();

}; // end Interface


#endif /* INTERFACEHEADER_H_ */

class displaynode
{
private:

  DisplayNodeTarget targettype;
  Person* personpointer;
  Object* objectpointer;

  displaynode* nextdisplaynode;
  int displayy;

public:
  displaynode(Object*,int,int,displaynode**); // used for drawing images associated with the object pprovided, ex. under construction images.
  displaynode(Person*,int,displaynode**);
  displaynode(Object*,int,displaynode**);
  ~displaynode();

  displaynode* getnextdisplaynode() {return nextdisplaynode;}
  void setnextdisplaynode(displaynode* newnextdisplaynode) {nextdisplaynode = newnextdisplaynode;}
  int getdisplayy() {return displayy;}
  DisplayNodeTarget gettype() {return targettype;}
  Object* getobject() {return objectpointer;}
  Person* getperson() {return personpointer;}

}; // end displaynode
