/*
 * Interface.cpp
 *
 *  Created on: 1/09/2015
 *      Author: Alex
 */

// includes
#include "InterfaceHeader.h"
#include "InputObjectHeader.h"
#include "GameDataHeader.h"
#include "Debugging.h"
#include "ActualObjects.h"
#include "UngroupedHeader.h"
extern DrawToWindow dtw; // the drawtowindow object
extern InputObject inobj; // the input object
extern Map map; // the map (ie. entire game's data) object
extern SysInfo sysinfo; // the timer

// debugging
Person* DEBUG_personpointer = NULL;

Interface::Interface()
{
  themainwindow = NULL; // this will be overwritten later

  currentscreen = MAINMENU;
  mainmenubackround = Pic::loadpic("MainMenuBackround.bmp");
  newgamebutton = Pic::loadpic("NewGameButton.bmp");
  exitgamebutton = Pic::loadpic("ExitGameButton.bmp");
  blackbox = Pic::loadpic("BlackBox.bmp");
  gameborder = Pic::loadpic("GameBorder.bmp","GameBorder_Transparency.bmp");
  menubox = Pic::loadpic("MenuBox.bmp");
  mainmenubutton = Pic::loadpic("MainMenuButton.bmp");
  gameborderperson = Pic::loadpic("GameBorderPerson.bmp","GameBorderPerson_Transparency.bmp");
  woodicon = Pic::loadpic("WoodIcon.bmp","WoodIcon_Transparency.bmp");
  foodicon = Pic::loadpic("FoodIcon.bmp","FoodIcon_Transparency.bmp");
  dirttile = Pic::loadpic("DirtTile.bmp","Tile_Transparency.bmp");
  grasstile = Pic::loadpic("GrassTile.bmp","Tile_Transparency.bmp");
  shallowwatertile = Pic::loadpic("ShallowWaterTile.bmp","Tile_Transparency.bmp");
  deepwatertile = Pic::loadpic("DeepWaterTile.bmp","Tile_Transparency.bmp");
  rocktile = Pic::loadpic("RockTile.bmp","Tile_Transparency.bmp");
  townhallimg = Pic::loadpic("TownHall.bmp","TownHall_Transparency.bmp");
  stockpileimg = Pic::loadpic("Stockpile.bmp","Stockpile_Transparency.bmp");
  shelterimg = Pic::loadpic("Shelter.bmp","Shelter_Transparency.bmp");
  farmimg = Pic::loadpic("Farm.bmp","Farm_Transparency.bmp");
  treeimg = Pic::loadpic("Tree.bmp","Tree_Transparency.bmp");
  berrybushimg = Pic::loadpic("BerryBush.bmp","BerryBush_Transparency.bmp");
  personimg = Pic::loadpic("Person.bmp","Person_Transparency.bmp");
  godmodeimg = Pic::loadpic("Godmode.bmp");
  underconstructionimg = Pic::loadpic("UnderConstruction.bmp","UnderConstruction_Transparency.bmp");

  if (mainmenubackround == NULL)
    MessageBox(NULL, "Picture Loading Failed.", "Error", MB_ICONEXCLAMATION | MB_OK);

  menuup = FALSE; // 0-setting
  personselected = NULL;
  screenx = 0;
  screeny = 0;
  displaynodeLL = NULL;
  godmode = FALSE;
} // end constructor

Interface::~Interface()
{
  Pic::deletepic(mainmenubackround);
  Pic::deletepic(newgamebutton);
  Pic::deletepic(exitgamebutton);
  Pic::deletepic(blackbox);
  Pic::deletepic(gameborder);
  Pic::deletepic(menubox);
  Pic::deletepic(mainmenubutton);
  Pic::deletepic(gameborderperson);
  Pic::deletepic(woodicon);
  Pic::deletepic(foodicon);
  Pic::deletepic(dirttile);
  Pic::deletepic(grasstile);
  Pic::deletepic(shallowwatertile);
  Pic::deletepic(deepwatertile);
  Pic::deletepic(rocktile);
  Pic::deletepic(townhallimg);
  Pic::deletepic(stockpileimg);
  Pic::deletepic(shelterimg);
  Pic::deletepic(farmimg);
  Pic::deletepic(treeimg);
  Pic::deletepic(berrybushimg);
  Pic::deletepic(personimg);
  Pic::deletepic(godmodeimg);
  Pic::deletepic(underconstructionimg);

  if (displaynodeLL != NULL)
    delete displaynodeLL;

  // DEBUG code
  if (DEBUG_personpointer != NULL)
  {
    delete DEBUG_personpointer;
    DEBUG_personpointer = NULL;
  }

} // end destructor

void Interface::initialize(HWND newhwnd)
{
  themainwindow = newhwnd;
} // end initialize

void Interface::update()
{
  switch (currentscreen)
  {
    case MAINMENU:
      mainmenuout();
      mainmenuin();
    break;
    case INGAME:
      gameout();
      gameprocessing();
      gamein();
    break;
    default:
    break;
  }
  dtw.Update(); // draw what you've buffered to the screen
} // end (interface) update

void Interface::mainmenuout()
{
  mainmenubackround->drawpic(dtw.GetBufferHDC(),0,0);
  newgamebutton->drawpic(dtw.GetBufferHDC(),250,275);
  exitgamebutton->drawpic(dtw.GetBufferHDC(),250,350);
} // end mainmenuout

void Interface::mainmenuin()
{
  InputMessage inputmessage = inobj.getinput();

  while (inputmessage != NOMOREMESSAGES) // while there are messages to process
  {
    switch (inputmessage)
    {
      case MOUSECLICK_LEFT:
        int x = 0,y = 0;
        inobj.getmousewentdownxy(MOUSELEFT, x, y);
        if (x > 250 and x < 550 and y > 275 and y < 325) // hit new game
        {
          currentscreen = INGAME;

          Object* objectlist = NULL;
          Faction* factionlist = new Faction(map.getnextfactionID());

          map.initialisegame(objectlist,factionlist);

        } else if (x > 250 and x < 550 and y > 350 and y < 400) // hit exit game
        {
          if (DEBUG == TRUE)
            cout << "Sending DestroyWindow Message\n";

          DestroyWindow(themainwindow); // end the program
        }
      break;
    } // end switch

    inputmessage = inobj.getinput();
  } // end while there are messages to process

} // end mainmenuin

void Interface::gameout()
{
  blackbox->drawpic(dtw.GetBufferHDC(),0,0);

  double toplefttilex, toplefttiley, garbagevalues;
  double botrighttilex, botrighttiley;
  double scrx, scry;
  tile* thetile = NULL;
  gettilexy(0,0,garbagevalues,toplefttiley);
  gettilexy(0,600,toplefttilex,garbagevalues);
  gettilexy(800,0,botrighttilex,garbagevalues);
  gettilexy(800,600,garbagevalues,botrighttiley);

  for (double drawtilex = toplefttilex - 1;drawtilex <= botrighttilex + 1; drawtilex++)
  {
    for (double drawtiley = toplefttiley - 1;drawtiley <= botrighttiley + 1; drawtiley++)
    {
      getscrxy(drawtilex,drawtiley,scrx,scry);
      thetile = map.gettile(drawtilex,drawtiley);

      if (thetile != NULL)
      {
        TileType type = thetile->gettype();

        switch (type)
        {
          case DIRT:
           dirttile->drawpic(dtw.GetBufferHDC(),scrx,scry-4);
          break;
          case GRASS:
            grasstile->drawpic(dtw.GetBufferHDC(),scrx,scry-4);
          break;
          case SHALLOWWATER:
            shallowwatertile->drawpic(dtw.GetBufferHDC(),scrx,scry-4);
          break;
          case DEEPWATER:
            deepwatertile->drawpic(dtw.GetBufferHDC(),scrx,scry-4);
          break;
          case ROCK:
            rocktile->drawpic(dtw.GetBufferHDC(),scrx,scry-4);
          break;
        } // end switch (type)
      }
    }
  } // end for each tile potentially in view

  // display objects section
  double thingx, thingy;

  for (Object* objptr = map.getfirstobject(); objptr != NULL; objptr = objptr->getnextobject())
  {
    getscrxy(objptr->getexchangex(),objptr->getexchangey(),thingx,thingy);
    if (thingx > -100 and thingx < 900 and thingy > -200 and thingy < 800)
    {
      new displaynode(objptr,thingy,&displaynodeLL);
      if ((objptr->gettype() == TOWNHALL or objptr->gettype() == STOCKPILE or objptr->gettype() == SHELTER or objptr->gettype() == FARM) and objptr->isfinished() == FALSE)
        new displaynode(objptr,0,thingy+1,&displaynodeLL);
    }
  }
  for (Faction* facptr = map.getfirstfaction(); facptr != NULL; facptr = facptr->getnextfaction())
  {
    for (Person* perptr = facptr->getfirstperson(); perptr != NULL; perptr = perptr->getnextperson())
    {
      getscrxy(perptr->getx(),perptr->gety(),thingx,thingy);
      if (thingx > -100 and thingx < 900 and thingy > -200 and thingy < 800)
        new displaynode(perptr,thingy,&displaynodeLL);
    }
  }

  for (displaynode* dsnptr = displaynodeLL; dsnptr != NULL; dsnptr = dsnptr->getnextdisplaynode())
    drawdisplaynode(dsnptr);

  if (displaynodeLL != NULL)
  delete displaynodeLL;
  displaynodeLL = NULL;

  gameborder->drawpic(dtw.GetBufferHDC(),0,0);

  if (personselected != NULL)
  {
      gameborderperson->drawpic(dtw.GetBufferHDC(),10,510);
    if (personselected->getresourcetype() == WOOD)
      woodicon->drawpic(dtw.GetBufferHDC(),110,530);
    else
      foodicon->drawpic(dtw.GetBufferHDC(),110,530);
  }

  if (godmode == TRUE)
    godmodeimg->drawpic(dtw.GetBufferHDC(),740,0);

  if (menuup == TRUE) // if menu is up
  {
    menubox->drawpic(dtw.GetBufferHDC(),200,225);
    mainmenubutton->drawpic(dtw.GetBufferHDC(),250,275);
    exitgamebutton->drawpic(dtw.GetBufferHDC(),250,350);
  } // end if menu is up

} // end gameout

void Interface::gameprocessing()
{
  int timesincelastupdate = sysinfo.getTimeSinceLastIteration();

  map.updatemap(timesincelastupdate);
} // end gameprocessing

void Interface::gamein()
{
  InputMessage inputmessage = inobj.getinput();
  int x = 0,y = 0,mousex,mousey;
  double tilex,tiley,scrx,scry;
  Object* objptr = NULL;
  Person* perptr = NULL;
  Faction* facptr = NULL;
  waypoint* wayptr = NULL;

  while (inputmessage != NOMOREMESSAGES) // while there are messages to process
  {
    switch (inputmessage)
    {
      case MOUSECLICK_LEFT:
        inobj.getmousewentdownxy(MOUSELEFT, x, y);

        if (menuup == TRUE) // if the menu is up
        {
          if (x > 250 and x < 550 and y > 275 and y < 325) // hit main menu
          {
            map.endgame();
            menuup = FALSE;
            godmode = FALSE;
            personselected = NULL;
            screenx = 0;
            screeny = 0;
            currentscreen = MAINMENU;
          } else if (x > 250 and x < 550 and y > 350 and y < 400) // hit exit game
          {
            if (DEBUG == TRUE)
              cout << "Sending DestroyWindow Message\n";

            DestroyWindow(themainwindow); // end the program
          }  else if (x >= 0 and x <= 800 and y >= 0 and y <= 600 and !(x > 200 and x < 600 and y > 225 and y < 450))
          { // did not hit in menu area, but still in game area
            menuup = FALSE;
          }
        } else // if menu is not up
        {
          if (x >= 0 and x <= 80 and y >= 0 and y <= 30) // hit menu area
          {
            menuup = TRUE;
          } else if (x >= 0 and x <= 110 and y >= 500 and y <= 600) // hit portrait area (very rough bounding box)
          {
            // do nothing
          } else // process general area click
          {

            // select person clicked, or noone if noone was clicked
            personselected = NULL;
            for (Faction* facptr = map.getfirstfaction();facptr != NULL;facptr = facptr->getnextfaction())
            {
              for (Person* perptr = facptr->getfirstperson();perptr != NULL;perptr = perptr->getnextperson())
              {
                tilex = perptr->getx();
                tiley = perptr->gety();
                getscrxy(tilex,tiley,scrx,scry);
                inobj.getmousexy(mousex, mousey);
                if (mousex > scrx - 12 and mousex < scrx + 12 and mousey < scry + 8 and mousey > scry - 32) // if click is on person
                {
                  personselected = perptr;
                } // end if click is on person
              }
            } // end for each person

          } // end process general area click
        } // end if menu is up
      break;
      case MOUSECLICK_RIGHT:

        // figure out where, and, if within map, waypoint there
        if (personselected != NULL)
        {
          inobj.getmousexy(mousex, mousey);
          gettilexy(mousex,mousey,tilex,tiley);

          bool hitsomething = FALSE;
          ObjectType objtype;

          for (objptr = map.getfirstobject();objptr != NULL;objptr = objptr->getnextobject())
          {
            if (tilex >= objptr->getx() and tilex <= objptr->getx()+objptr->getwidth() and tiley >= objptr->gety() and tiley <= objptr->gety()+objptr->getheight())
            { // if click was on object
              objtype = objptr->gettype();
              if (objtype == TOWNHALL or objtype == STOCKPILE or objtype == SHELTER or objtype == FARM)
              { // if you hit an unfinished building, build it
                if (objptr->isfinished() == FALSE)
                {
                  hitsomething = TRUE;
                  Instruction* newinstruction = new Build(objptr->getID());
                  if (newinstruction != NULL)
                    personselected->replaceinstructionchain(newinstruction);
                }
              }
              if (objtype == TREE or objtype == BERRYBUSH or (objtype == FARM and objptr->isfinished() == true))
              { // if you hit a resource, gather it
                hitsomething = TRUE;
                Instruction* newinstruction = new Gather(objptr->getID(),((objtype == TREE)?WOOD:FOOD));
                if (newinstruction != NULL)
                  personselected->replaceinstructionchain(newinstruction);
              }
            } // end if click was on object
          } // end for each object

          if (hitsomething == FALSE) // if you did not hit a resource or unfinished building
          {
            if (tilex >= 0 and tilex < gmapwidth and tiley >= 0 and tiley < gmapheight)
            {
              wayptr = personselected->pathto(tilex,tiley);
              //wayptr = NULL; DEBUG
              Instruction* newinstruction = new WalkTo(wayptr);
              if (newinstruction != NULL)
                personselected->replaceinstructionchain(newinstruction);
            }
          }
        }

      break;
      case KEY_G_CTRL:
        if (godmode == FALSE)
          godmode = TRUE;
        else
          godmode = FALSE;
      break;
      case KEY_1:
        inobj.getmousexy(mousex, mousey);
        gettilexy(mousex,mousey,tilex,tiley);
        map.addobject(new TownHall(tilex,tiley,map.getnextobjectID()));
      break;
      case KEY_2:
        inobj.getmousexy(mousex, mousey);
        gettilexy(mousex,mousey,tilex,tiley);
        map.addobject(new Stockpile(tilex,tiley,map.getnextobjectID()));
      break;
      case KEY_3:
        inobj.getmousexy(mousex, mousey);
        gettilexy(mousex,mousey,tilex,tiley);
        map.addobject(new Farm(tilex,tiley,map.getnextobjectID()));
      break;
      case KEY_4:
        inobj.getmousexy(mousex, mousey);
        gettilexy(mousex,mousey,tilex,tiley);
        map.addobject(new Shelter(tilex,tiley,map.getnextobjectID()));
      break;
      case KEY_Q:
        if (godmode == TRUE)
        {
          inobj.getmousexy(mousex, mousey);
          gettilexy(mousex,mousey,tilex,tiley);
          tile* tilepointer = map.gettile(tilex,tiley);
          if (tilepointer != NULL)
            tilepointer->settype(DIRT);
        }
      break;
      case KEY_W:
        if (godmode == TRUE)
        {
          inobj.getmousexy(mousex, mousey);
          gettilexy(mousex,mousey,tilex,tiley);
          tile* tilepointer = map.gettile(tilex,tiley);
          if (tilepointer != NULL)
            tilepointer->settype(GRASS);
        }
      break;
      break;
      case KEY_E:
        if (godmode == TRUE)
        {
          inobj.getmousexy(mousex, mousey);
          gettilexy(mousex,mousey,tilex,tiley);
          tile* tilepointer = map.gettile(tilex,tiley);
          if (tilepointer != NULL)
            tilepointer->settype(ROCK);
        }
      break;
      break;
      case KEY_R:
        if (godmode == TRUE)
        {
          inobj.getmousexy(mousex, mousey);
          gettilexy(mousex,mousey,tilex,tiley);
          tile* tilepointer = map.gettile(tilex,tiley);
          if (tilepointer != NULL)
            tilepointer->settype(SHALLOWWATER);
        }
      break;
      break;
      case KEY_T:
        if (godmode == TRUE)
        {
          inobj.getmousexy(mousex, mousey);
          gettilexy(mousex,mousey,tilex,tiley);
          tile* tilepointer = map.gettile(tilex,tiley);
          if (tilepointer != NULL)
            tilepointer->settype(DEEPWATER);
        }
      break;
      break;
      case KEY_5:
        inobj.getmousexy(mousex, mousey);
        gettilexy(mousex,mousey,tilex,tiley);
        if (godmode == TRUE)
        {
          map.addobject(new Tree(tilex,tiley,map.getnextobjectID()));
        }
      break;
      case KEY_6:
        inobj.getmousexy(mousex, mousey);
        gettilexy(mousex,mousey,tilex,tiley);
        if (godmode == TRUE)
        {
          map.addobject(new BerryBush(tilex,tiley,map.getnextobjectID()));
        }
      break;
      case KEY_P:
        inobj.getmousexy(mousex, mousey);
        gettilexy(mousex,mousey,tilex,tiley);
        if (DEBUG == TRUE)
          cout << "Beginning person creation\n";
        perptr = new Person(tilex,tiley);

        if (DEBUG_personpointer != NULL)
          delete DEBUG_personpointer;
        DEBUG_personpointer = /*perptr*/NULL;

        if (DEBUG == TRUE)
          cout << "Person creation returned pointer value: " << perptr << "\n";


        facptr = map.getfirstfaction();
        if (DEBUG == TRUE)
          cout << "Get first faction returned pointer value: " << facptr << "\n";


        if (perptr != NULL)
          if (facptr != NULL)
            facptr->addperson(perptr);



      break;
      case NOMOREMESSAGES:
      break;
    } // end switch

    inputmessage = inobj.getinput();
  } // end while there are messages to process

  // move the screen
  double scrollspeed = 10.0;
  inobj.getmousexy(x,y);
  if (x <= 5) {screenx -= scrollspeed;screeny -= scrollspeed;}
  if (x >= 795) {screenx += scrollspeed;screeny += scrollspeed;}
  if (y <= 5) {screenx += scrollspeed;screeny -= scrollspeed;}
  if (y >= 595) {screenx -= scrollspeed;screeny += scrollspeed;}
  if (screenx < -144) {screenx = -144;}
  if (screenx > gmapwidth) {screenx = gmapwidth;}
  if (screeny < -144) {screeny = -144;}
  if (screeny > gmapheight) {screeny = gmapheight;}
} // end gamein


void Interface::gettilexy(double scrx,double scry,double& tilex,double& tiley)
{
  double tilewidth = 12;
  double tileheight = 8;
  double mapheight = 600;

  double dx = (scrx + (mapheight/2.0))/(tilewidth);
  double dy = (scry - (mapheight/2.0))/(tileheight);

  tilex = screenx + dx - dy;
  tiley = screeny + dx + dy;

} // end gettilexy

void Interface::getscrxy(double tilex,double tiley,double& scrx,double& scry)
{
  double tilewidth = 12;
  double tileheight = 8;
  double mapheight = 600;

  scrx = (-1.0)*(mapheight/2.0) + (tilewidth/2.0) * ((tilex - screenx) + (tiley - screeny));
  scry = (mapheight/2.0) + (tileheight/2.0) * ((tiley - screeny) - (tilex - screenx));
} // end getscrxy

void Interface::drawdisplaynode(displaynode* dsnptr)
{
  double scrx,scry;

  if (dsnptr->gettype() == OBJECT)
  {
    getscrxy(dsnptr->getobject()->getx(),dsnptr->getobject()->gety(),scrx,scry);
    switch(dsnptr->getobject()->gettype())
    {
      case TOWNHALL:
        townhallimg->drawpic(dtw.GetBufferHDC(),scrx,scry - 48);
      break;
      case STOCKPILE:
        stockpileimg->drawpic(dtw.GetBufferHDC(),scrx,scry - 60);
      break;
      case SHELTER:
        shelterimg->drawpic(dtw.GetBufferHDC(),scrx,scry - 40);
      break;
      case FARM:
        farmimg->drawpic(dtw.GetBufferHDC(),scrx,scry - 40);
      break;
      case TREE:
        treeimg->drawpic(dtw.GetBufferHDC(),scrx,scry - 68);
      break;
      case BERRYBUSH:
        berrybushimg->drawpic(dtw.GetBufferHDC(),scrx,scry - 20);
      break;
    }

  } else if (dsnptr->gettype() == PERSON)
  {
    getscrxy(dsnptr->getperson()->getx(),dsnptr->getperson()->gety(),scrx,scry);
    personimg->drawpic(dtw.GetBufferHDC(),scrx - 12,scry - 32);
  } else // building symbol
  {
    getscrxy(dsnptr->getobject()->getx(),dsnptr->getobject()->gety(),scrx,scry);
    switch(dsnptr->getobject()->gettype())
    {
      case TOWNHALL:
        underconstructionimg->drawpic(dtw.GetBufferHDC(),scrx + 15,scry - 14);
      break;
      case STOCKPILE:
        underconstructionimg->drawpic(dtw.GetBufferHDC(),scrx + 45,scry - 25);
      break;
      case SHELTER:
        underconstructionimg->drawpic(dtw.GetBufferHDC(),scrx - 15,scry - 25);
      break;
      case FARM:
        underconstructionimg->drawpic(dtw.GetBufferHDC(),scrx + 45,scry - 15);
      break;
    }

  }

} // end drawdisplaynode

displaynode::displaynode(Object* newobjptr,int whattodraw,int theiry, displaynode** firstdisplaynode)
{
  targettype = OTHER;
  personpointer = NULL;
  objectpointer = newobjptr;
  displayy = theiry;

  if ((*firstdisplaynode) == NULL)
  {
    (*firstdisplaynode) = this;
    nextdisplaynode = NULL;
    return;
  }

  displaynode* displaynodepointer = (*firstdisplaynode);

  while(displaynodepointer->getnextdisplaynode() != NULL)
  {

    if (displaynodepointer->getnextdisplaynode()->getdisplayy() >= displayy)
      break;

    displaynodepointer = displaynodepointer->getnextdisplaynode();
  }

  nextdisplaynode = displaynodepointer->getnextdisplaynode();
  displaynodepointer->setnextdisplaynode(this);

} // end constructor (other)

displaynode::displaynode(Person* newperptr,int theiry, displaynode** firstdisplaynode)
{
  targettype = PERSON;
  personpointer = newperptr;
  objectpointer = NULL;
  displayy = theiry;

  if ((*firstdisplaynode) == NULL)
  {
    (*firstdisplaynode) = this;
    nextdisplaynode = NULL;
    return;
  }

  displaynode* displaynodepointer = (*firstdisplaynode);

  while(displaynodepointer->getnextdisplaynode() != NULL)
  {

    if (displaynodepointer->getnextdisplaynode()->getdisplayy() >= displayy)
      break;

    displaynodepointer = displaynodepointer->getnextdisplaynode();
  }

  nextdisplaynode = displaynodepointer->getnextdisplaynode();
  displaynodepointer->setnextdisplaynode(this);

} // end constructor (Person*)

displaynode::displaynode(Object* newobjptr,int theiry, displaynode** firstdisplaynode)
{
  targettype = OBJECT;
  personpointer = NULL;
  objectpointer = newobjptr;
  displayy = theiry;

  if ((*firstdisplaynode) == NULL)
  {
    (*firstdisplaynode) = this;
    nextdisplaynode = NULL;
    return;
  }

  displaynode* displaynodepointer = (*firstdisplaynode);

  while(displaynodepointer->getnextdisplaynode() != NULL)
  {

    if (displaynodepointer->getnextdisplaynode()->getdisplayy() >= displayy)
      break;

    displaynodepointer = displaynodepointer->getnextdisplaynode();
  }

  nextdisplaynode = displaynodepointer->getnextdisplaynode();
  displaynodepointer->setnextdisplaynode(this);

} // end constructor (Object*)

displaynode::~displaynode()
{
  if (nextdisplaynode != NULL)
    delete nextdisplaynode;
} // end destructor

