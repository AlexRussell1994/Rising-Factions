/*
 *  DrawToWindowHeader.h
 *
 *  This header includes a class for simplifying output
 *
 *  From the program this originated in:
 *  This program creates a class that handles buffering and output to the screen
 *  Using the class requires including the header(this) and the file
 *  as well as calling the appropriate functions for WM_PAINT and periodic updates
 *  and drawing output to a buffer maintained by the class through functions provided by the class
 *
 */

#ifndef DRAWTOWINDOWHEADER_H_ // standard header safety brackets
#define DRAWTOWINDOWHEADER_H_

#include <windows.h>
#include <iostream>
using namespace std;

class Picnode;

class DrawToWindow
{
private:
  bool initialized; // keep track of if you are ready to write to the screen
  HWND hwnd; // the handle of the window this object is for
  HBITMAP bitmapbuffer1; // recieves input from buffer 2 and draws to the screen
  HBITMAP bitmapbuffer2; // recieves input from the program and draws to buffer 2
  HDC handlebuffer1; // handles for the above bitmaps
  HDC handlebuffer2;
  HDC thescreen; // no HDC is actually kept in here; it is destroyed in each function that creates it
  HBITMAP oldbitmapbuffer1; // holds data for safely deleting the buffer device contexts
  HBITMAP oldbitmapbuffer2;
  int screenwidth; // the size of the screen
  int screenheight;
  int timesincelastcall; // milliseconds since update was last called with a time since last call parameter
  Picnode * picnodelinkedlist; // the linked list of loaded pictures

public:
  DrawToWindow();
  ~DrawToWindow();

  Picnode* getfirstpicnode(){return picnodelinkedlist;} // for new picnodes to be inserted
  void setfirstpicnode(Picnode* newfirstpicnode){picnodelinkedlist = newfirstpicnode;}

  void Initialize(HWND); // sets the class up, call only once
  void Update(); // should be called when the program is ready to push what it has drawn to the screen
  void Update(int timesincelastcall); // use this instead of Update() to allow for automated animation
  void Periodic(); // can be called periodically by to program to keep the screen updating
  // but that's only necessary if you want DrawToWindow to automate some moving images
  // (and I haven't programmed it to do that yet)
  // it is also called by update()
  int Paint(); // should be called when the main window recieves a WM_PAINT message
  // this function returns 0 to indicate it processes this message
  // this should be passed back throug hthe window procedure
  HDC GetBufferHDC(){return handlebuffer2;} // to be used only to draw stuff
  // do not delete a device context returned this way
  // do not call a device context returned this way after the object that supplied it is deleted

}; // end class DrawToWindow

class Picnode
{
private:
  HBITMAP picture; // the picture
  HDC picturehandle;
  HBITMAP oldpicturehandlecontents;
  HBITMAP transparencymap; // what parts are transparent
  HDC transparencymaphandle;
  HBITMAP oldtransparencymaphandlecontents;
  bool transparency; // is transparency a thing for this pic
  int framewidth, frameheight, currentframe; // how big is the animated image, and what part is next
  int frameswide, framestall, currentframex, currentframey, totalframes; // information for changing frames
  int timetonextframe, timebetweenframes; // when should new animation parts be displayed
  bool animated, automatic; // is animation a thing for this pic, and is frame change automatic (something for this class to handle)
  BITMAP pictureinfo; // info about the picture, especially useful for navigating frames for animation
  int firstframe, lastframe; // the subset of frames that the animation is currently looping
  Picnode* nextpicnode; // linked list links
  Picnode* lastpicnode;

  // called only by other loadpics, specifying everything
  static Picnode* loadpic(char* filename,char* transparencyfile,bool istransparency,int width,int height,int time,int totalframes, bool isanimated, bool isautomatic)
  {return new Picnode(filename,transparencyfile,istransparency,width,height,time,totalframes,isanimated,isautomatic);}

public:
  Picnode(char* filename,char* transparencyfile,bool istransparency,int width,int height,int time,int newtotalframes, bool isanimated, bool isautomatic);
  ~Picnode();

  void setnextpicnode(Picnode* newnextpicnode){nextpicnode = newnextpicnode;} // functions to change links
  void setlastpicnode(Picnode* newlastpicnode){lastpicnode = newlastpicnode;}
  Picnode* getnextpicnode(){return nextpicnode;} // functions to access links
  Picnode* getlastpicnode(){return lastpicnode;}

  void drawpic(HDC drawtolocation,int x,int y); // draws this pic at x and y on drawtolocation
  void updateanimation(int timesincelastupdate);

  static Picnode* loadpic(char* filename) // prety self explanatory
    {return loadpic(filename,NULL,FALSE,0,0,1,0,FALSE,FALSE);}
  static Picnode* loadpic(char* filename,char* transparencyfile) // now with transparency
    {return loadpic(filename,transparencyfile,TRUE,0,0,1,0,FALSE,FALSE);}
  // width and height of a frame of animation, and time delay betweenframes
  static Picnode* loadpic(char* filename,int width,int height,int time,int totalframes)
    {return loadpic(filename,NULL,FALSE,width,height,time,totalframes,TRUE,time != 0);}
  static Picnode* loadpic(char* filename,char* transparencyfile,int width,int height,int time,int totalframes)
    {return loadpic(filename,transparencyfile,TRUE,width,height,time,totalframes,TRUE,time != 0);}

  static void deletepic(Picnode* pictodelete); // call only once per pic, and do not use on a deleted pic

  void setframe(int newcurrentframe);
  void setanimation(int newfirstframe, int newlastframe)
  {firstframe = newfirstframe;lastframe = newlastframe;}
  void setanimationtimedaley(int newtimedelay){timetonextframe = timebetweenframes = newtimedelay;}

}; // end class Picnode

typedef Picnode Pic;

#endif /* DRAWTOWINDOWHEADER_H_ */
