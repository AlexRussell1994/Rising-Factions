/*
 * GameFunctions.cpp
 *
 *  Created on: 28/08/2015
 *      Author: Alex
 */

// includes
#include "GameDataHeader.h"
#include "Debugging.h"

extern Map map;

Map::Map()
{
  mapwidth = gmapwidth;
  mapheight = gmapheight;

  objectLL = NULL;
  factionLL = NULL;

  activemaploaded = FALSE;

  nextobjectID = 0;
  nextfactionID = 0;
  nextpersonID = 0;

} // end constructor

Map::~Map()
{

  if (activemaploaded == TRUE) // if there is a map loaded
  {
    delete (objectLL);
    delete (factionLL);


    for (int i = 0; i < mapwidth; i++)
      for (int j = 0; j < mapheight; j++)
        delete (tiles[i][j]);

  } // end if there is a map loaded

} // end destructor

void Map::updatemap(int timesincelastupdate)
{

  if (activemaploaded == TRUE) // if there is a game loaded
  {
    if (factionLL != NULL) // update factions, and through then persons ("people" may be confusing here)
    {
      factionLL->updateall(timesincelastupdate);
    }
    if (objectLL != NULL) // update factions, and through then persons ("people" may be confusing here)
    {
      objectLL->updateall(timesincelastupdate);
    }
  } // end if there is a game loaded

} // end updatemap

void Map::blocktile(int x,int y)
{
  UnitSize temp = MAXSIZE; // because accessing an enumeration is hard
  int maxunitsize = (int)temp; // (no I haven't looked for a better way)

  tile* tilepointer = NULL;

  tilepointer = map.gettile(x,y); // make this tile inpassable
    if (NULL != tilepointer)
      tilepointer->makeinpassable();

  for (int i = x - maxunitsize; i <= x + maxunitsize; i++) // cause nearby tiles to update
  {
    for (int j = y - maxunitsize; j <= y + maxunitsize; j++)
    {
      tilepointer = map.gettile(i,j);
      if (NULL != tilepointer)
        tilepointer->updatepassable();
    }
  }

} // end blocktile

void Map::unblocktile(int x,int y)
{
  UnitSize temp = MAXSIZE; // because accessing an enumeration is hard
  int maxunitsize = (int)temp; // (no I haven't looked for a better way)

  tile* tilepointer = NULL;

  tilepointer = map.gettile(x,y); // make this tile passable
    if (NULL != tilepointer)
      tilepointer->makepassable();

  for (int i = x - maxunitsize; i <= x + maxunitsize; i++) // cause nearby tiles to update
  {
    for (int j = y - maxunitsize; j <= y + maxunitsize; j++)
    {
      tilepointer = map.gettile(i,j);
      if (NULL != tilepointer)
        tilepointer->updatepassable();
    }
  }

} // end blocktile

tile::tile()
{
  for (UnitSize i = ONE; i < MAXSIZE; i = (UnitSize)(i + 1)) // I shouldn't be hard-casting UnitSize here
    isblocked[i] = FALSE;                              // but dannit, this is what I made these enums for

  x = 0; // these values must be explicitly overwritten by setxy before the tile is used
  y = 0;

  typeoftile = DIRT;
} // end constructior

void tile::updatepassable()
{
  UnitSize temp = MAXSIZE; // because accessing an enumeration is hard
  int maxunitsize = (int)temp; // (no I haven't looked for a better way)

  tile* tilepointer = NULL;

  for (int a = 1; a <= maxunitsize; a++) // alow everything through, unless you can't
    isblocked[(UnitSize)a] = FALSE;

  for (int a = 1; a <= maxunitsize; a++) // for each size
  {
    for (int i = x - a; i <= x + a; i++) // for each applicable x
    {
      for (int j = y - a; j <= y + a; j++) // for each applicable y
      {
        tilepointer = map.gettile(i,j);
        if (tilepointer != NULL) // if tile is on map
        {
          if (tilepointer->istileinpassible(ONE) == TRUE)
            isblocked[(UnitSize)a] = TRUE;
        } else
        {
          // do nothing; tiles not on the map count as passable, to avoid hefty calculations
        } // end if tile is on map
      } // end for each applicable y
    } // end for each applicable x
  } // end for each size

} // end updatepassable

void tile::settiledata(tile* setto)
{
  for (int i = 0; i < (int)MAXSIZE; i++)
    isblocked[i] = setto->istileinpassible((UnitSize)i);
} // end settiledata

void Object::updateall(int timesincelast)
{
  update(timesincelast);

  if (nextobject != NULL)
    nextobject->updateall(timesincelast);

} // end (object) updateall

Faction::Faction(int newfactionID)
{

  personLL = NULL;
  nextfaction = NULL;

  for (int i = 0; i < map.getwidth(); i++)
    for (int j = 0; j < map.getheight(); j++)
      los[i][j] = 0;

  factionID = newfactionID;

} // end constructor

Faction::~Faction()
{

  if (nextfaction != NULL)
    delete nextfaction;

  if (personLL != NULL)
    delete personLL;

} // end destructor

void Faction::updateall(int timesincelast) // updates all of the factions from here on in the factionLL
{
  if (DEBUG == TRUE)
    cout << "Faction::updateall called.\n";

  update(timesincelast);

  if (nextfaction != NULL)
    nextfaction->updateall(timesincelast);

} // end (faction) updateall

void Faction::update(int timesincelast) // updates this faction an its constituent people (fine! "people," not "persons")
{

/*  for (int i = 0;i <= gmapwidth;i++) // set once-visible terrain to not-visible
    for (int j = 0;j <= gmapheight;j++)
      if (los[i][j] == 2)
        los[i][j] = 1; DEBUG*/

  if (DISABLE_FACTIONPERSONUSE == TRUE) // DEBUG
    return;

  if(personLL != NULL) // update constituent persons (includes update currently visible los)
  {
    personLL->updateall(timesincelast);
  }

  // non-manual faction AI would go here

} // end (faction) update

Person::Person(int newx, int newy)
{

  if (DEBUG == TRUE)
    cout << "Person constructor reached. Called location x: " << newx << " y: " << newy << "\n";

  personID = 0; // these must be explicitly overwritten by setid before the person is used
  owningfactionID = 0;

  inventorycontents = 0;
  maxinventory = 10;
  inventorytype = WOOD;

  instructionLL = NULL;
  nextwaypoint = NULL;
  personspeed = 0.005; // 5 tiles/second

  sightrange = 10; // sees 10 tiles

  nextperson = NULL;

  if (DEBUG == TRUE)
    cout << "location setting begins\n";

  double* personslocationx = personsunit.getxref();
  double* personslocationy = personsunit.getyref();
  *personslocationx = newx;
  *personslocationy = newy;

  if (DEBUG == TRUE)
    cout << "location setting ends\n";

} // end constructor

Person::~Person()
{

  if (nextperson != NULL)
    delete nextperson;

  if (instructionLL != NULL)
    delete instructionLL;
} // end destructor


void Person::updateall(int timesincelast) // updates the rest of the people in the personLL
{
  update(timesincelast);

  if (nextperson != NULL)
    nextperson->updateall(timesincelast);

} // end (person) updateall

void Person::update(int timesincelast) // updates this person
{

  Instruction* returnvalue = NULL;

  if (instructionLL != NULL) // carry out current instruction
  {
    returnvalue = instructionLL->performtask(this, timesincelast);
  }

  if (returnvalue != NULL)
  {
    delete (instructionLL); // delete completed instructions
    instructionLL = returnvalue;
  }

  /*
  Faction* powningfaction = map.getfaction(owningfactionID); // keep faction's los up to date
  if (powningfaction != NULL)
    powningfaction->addtolos(personsunit.getx(),personsunit.gety(),sightrange);
    DEBUG*/

} // end (person) update

void Person::addinstructiontochain(Instruction* newinstruction)
{
  Instruction* instructionpointer = instructionLL;

  if (instructionpointer == NULL) // if there is to instructionLL
  {
    instructionLL = newinstruction;
    return;
  } // end if there is to instructionLL

  while (instructionpointer->getnextinstruction() != NULL)
    instructionpointer = instructionpointer->getnextinstruction();

  instructionpointer->setnextinstruction(newinstruction);

} // end addinstructiontochain

void Person::insertnewfirstinstruction(Instruction* newinstruction)
{
  if (newinstruction == NULL)
    return;

  Instruction* instructionpointer = newinstruction;

  while (instructionpointer->getnextinstruction() != NULL)
    instructionpointer = instructionpointer->getnextinstruction();

  instructionpointer->setnextinstruction(instructionLL);
  instructionLL = newinstruction;
} // end insertnewfirstinstruction

void Person::replaceinstructionchain(Instruction* newinstruction)
{
  if (instructionLL != NULL)
    delete instructionLL;

  instructionLL = newinstruction;

} // end replaceinstructionchain


int Person::move(int movement)
{
  if (DEBUG == TRUE)
    cout << "Person::move called.\n";
//  if (DISABLE_WALKINGCOMPONENTS == TRUE) //DEBUG
//    return 0;

  double& currentx = *personsunit.getxref();
  double& currenty = *personsunit.getyref();
  double potentialnextx = currentx;
  double potentialnexty = currenty;
  double gotox = nextwaypoint->getdoublex();
  double gotoy = nextwaypoint->getdoubley();

  double distancetotarget = sqrt((gotox-currentx)*(gotox-currentx) + (gotoy-currenty)*(gotoy-currenty));

  if (distancetotarget <= movement) // target is less than movement away
  {
    potentialnextx = gotox; // you are at target
    potentialnexty = gotoy;
  } else
  {
    potentialnextx = currentx + movement * (gotox-currentx) / distancetotarget; // get where you would be after move
    potentialnexty = currenty + movement * (gotoy-currenty) / distancetotarget;
  } // end if is target <= movement away


  bool returnvalue = TRUE; // DEBUG value setting

  /*DEBUG commenting

*////*
  waypoint* waypointtopass = new waypoint(currentx,currenty,personsunit.getsize());
  waypointtopass->setnextwaypoint(new waypoint(potentialnextx,potentialnexty,personsunit.getsize()));
//  waypointtopass->checkpassable();
  delete (waypointtopass);
  // */
  /*

  returnvalue = waypointtopass->checkpassable(); // checkpassable there

  */ //DEBUG commenting ends

  if (returnvalue == FALSE) // checkpassable failed
  {
    waypoint* waypointpointer;
    for (waypointpointer = nextwaypoint; waypointpointer->getnextwaypoint() != NULL; waypointpointer = waypointpointer->getnextwaypoint())
      ; // nothing necessary, waypointpointer will point to the last waypoint in the chain

    tile* currenttile = NULL; // is it on an impassible tile
    bool isonaninpassibletile = TRUE;
    for (int i = waypointpointer->getdoublex(); ((double)i) >= waypointpointer->getdoublex() - 1 ; i -= 1)
    {
      for (int j = waypointpointer->getdoubley(); ((double)j) >= waypointpointer->getdoubley() - 1 ; j -= 1)
      {
        currenttile = map.gettile(i,j);
        if (currenttile != NULL)
          if (currenttile->istileinpassible(personsunit.getsize()) == FALSE)
            isonaninpassibletile = FALSE;
      }
    } // end section is it on an impassible tile

    if (isonaninpassibletile == TRUE) // final waypoint in chain is in an inpassable tile
      return -2; // return -2
    else
      return -1; // return -1
  } // end if checkpassable failed

  currentx = potentialnextx; // update unit location
  currenty = potentialnexty;

  if (gotox == currentx and gotoy == currenty) // you are at target
    return 1; // return 1
  else
    return 0; // return 0

} // end move

int Person::getresourceamount(ResourceType type)
{
  if (type != inventorytype)
    return 0;
  else
    return inventorycontents;
} // end getresourceamount

Instruction::Instruction()
{
  nextinstruction = NULL;
} // end constructor

Instruction::~Instruction()
{
  if (nextinstruction != NULL)
    delete nextinstruction;
  nextinstruction = NULL;
} // end destructor

void Faction::addtolos(int sourcex, int sourcey, int sightrange)
{
  int tilex;
  int tiley;

  for (tilex = sourcex - sightrange; tilex <= sourcex + sightrange; sourcex++)
    for (tiley = sourcey - sightrange; tilex <= sourcey + sightrange; sourcey++)
      if (map.gettile(tilex,tiley) != NULL)
        los[tilex][tiley] = 2;

} // end addtolos (stationary)

Person* Faction::getperson(int personID)
{
  if (DISABLE_FACTIONPERSONUSE == TRUE) // DEBUG
  {
    cout << "getperson called";
    return NULL;
  }

  Person* personpointer = personLL;

  while (personpointer != NULL)
  {
    if (personpointer->getpersonID() == personID)
      break;
    personpointer = personpointer->getnextperson();
  }

  return personpointer;
} // end getperson

Faction* Map::getfaction(int factionID)
{
  Faction* factionpointer = factionLL;

  while (factionpointer != NULL)
  {
    if (factionpointer->getfactionID() == factionID)
      break;
    factionpointer = factionpointer->getnextfaction();
  }

  return factionpointer;
} // end getfaction

Object* Map::getobject(int objectID)
{
  Object* objectpointer = objectLL;

  while (objectpointer != NULL)
  {
    if (objectpointer->getID() == objectID)
      break;
    objectpointer = objectpointer->getnextobject();
  }

  return objectpointer;
} // end getobject


// location is the upper left corner of the smallest possible bounding box
Object::Object(int newx,int newy, int newobjectID)
{
  objectx = newx;
  objecty = newy;
  objectwidth = 1; // will be overwritten by inherited construcotrs
  objectheight = 1;

  objectID = newobjectID;
  objecttype = NONEOBJ; // derived constructors will overwrite this

  nextobject = NULL;

} // end constructor


Object::~Object()
{
  if (nextobject != NULL) // delete any other objects in this object's chain
    delete nextobject;
} // end virtual destructor
// (unlike othervirtual functions, using this calls the inheriting (destructor) as well as this one)

Building::Building(int newx,int newy, int newobjectID): Object(newx, newy, newobjectID)
{
  // nothing else needs doing here yet
} // end constructor

bool Map::initialisegame(Object* newobjectLL, Faction* newfactionLL)
{

  if (activemaploaded == TRUE) // if map already loadad
  {
    delete (newobjectLL); // delete everything
    delete (newfactionLL);

    for (int i = 0; i < mapwidth; i++)
      for (int j = 0; j < mapheight; j++)
        delete (tiles[i][j]);

    return FALSE; // and return FASLE
  } // end if map already loaded

  objectLL = newobjectLL; // place the new objects in the appropriate variables
  factionLL = newfactionLL;

  for (int i = 0; i < mapwidth; i++)
  {
    for (int j = 0; j < mapheight; j++)
    {
      tiles[i][j] = new tile;
      tiles[i][j]->setxy(i,j);
    }
  }

  nextobjectID = 0; // re-ID everything
  nextfactionID = 0;
  nextpersonID = 0;

  for(Object* objptr = objectLL; objptr != NULL; objptr = objptr->getnextobject())
    objptr->setID(nextobjectID++);

  for(Faction* facptr = factionLL; facptr != NULL; facptr = facptr->getnextfaction())
    facptr->setID(nextfactionID++);

  for(Faction* facptr = factionLL; facptr != NULL; facptr = facptr->getnextfaction())
  {
    for(Person* perptr = facptr->getfirstperson(); perptr != NULL; perptr = perptr->getnextperson())
    {
      perptr->setIDs(nextpersonID++,facptr->getfactionID());
      perptr->replaceinstructionchain(NULL); // and delete any existing instruction chains
    }
  }

  activemaploaded = TRUE; // a map is now loaded
  return TRUE; // return TRUE
} // end initialisegame

void Map::endgame()
{
  if (activemaploaded == TRUE) // if there is a map loaded
  {
    delete (objectLL);
    delete (factionLL);


    for (int i = 0; i < mapwidth; i++)
      for (int j = 0; j < mapheight; j++)
        delete (tiles[i][j]);

    activemaploaded = FALSE;
  } // end if there is a map loaded

} // end endgame

void Person::setIDs(int newpersonID, int newowningfactionID)
{
  personID = newpersonID;
  owningfactionID = newowningfactionID;
} // end setIDs

void Faction::addperson(Person* newperson)
{
  Person* personpointer = personLL;

  if (personpointer == NULL) // add to the end of the current person linked list
    personLL = newperson;
  else
  {
    while (personpointer->getnextperson() != NULL)
      personpointer = personpointer->getnextperson();

    personpointer->setnextperson(newperson);
  }

  personpointer = newperson;
  while (personpointer != NULL) // for each new perosn added
  {
    personpointer->setIDs(map.getnextpersonID(),factionID); // assign IDs

//DEBUG    addtolos(personpointer->getx(),personpointer->gety(),personpointer->getsightrange()); // faction los

    personpointer = personpointer->getnextperson();
  } // end for each new perosn added
} // end addperson

void Faction::removeperson(int personID)
{
  Person* personpointer = personLL;

  if (personpointer == NULL)
    return;

  while (personpointer->getnextperson() != NULL)
  {
    if (personpointer->getnextperson()->getpersonID() == personID)
      break;
    personpointer = personpointer->getnextperson();
  }

  if (personpointer->getnextperson() != NULL) // if the person to be deleted was found
  {
    Person* temp = personpointer->getnextperson()->getnextperson();
    personpointer->getnextperson()->setnextperson(NULL);
    delete (personpointer->getnextperson());
    personpointer->setnextperson(temp);
  } // end if the object to be deleted was found

} // end removeperson

void Map::addobject(Object* newobject)
{
  Object* objectpointer = objectLL;

  if (objectLL == NULL) // add to the end of the current object linked list
    objectLL = newobject;
  else
  {
    while (objectpointer->getnextobject() != NULL)
      objectpointer = objectpointer->getnextobject();

    objectpointer->setnextobject(newobject);
  }

} // end addobject

void Map::removeobject(int objectID)
{
  Object* objectpointer = objectLL;

  while (objectpointer->getnextobject() != NULL)
  {
    if (objectpointer->getnextobject()->getID() == objectID)
      break;
    objectpointer = objectpointer->getnextobject();
  }

  if (objectpointer->getnextobject() != NULL) // if the object to be deleted was found
  {
    Object* temp = objectpointer->getnextobject()->getnextobject();
    objectpointer->getnextobject()->setnextobject(NULL);
    delete (objectpointer->getnextobject());
    objectpointer->setnextobject(temp);
  } // end if the object to be deleted was found

} // end removeobject

