/*
 *  DrawToWindowFunctions.cpp
 *
 *  This file defines the functions of a class mean to simplify output
 *
 *  From the program this originated in:
 *  This program creates a class that handles buffering and output to the screen
 *  Using the class requires including the header and the file(this)
 *  as well as calling the appropriate functions for WM_PAINT and periodic updates
 *  and drawing output to a buffer maintained by the class through functions provided by the class
 *
 */

// includes
#include "DrawToWindowHeader.h"
#include "Debugging.h"

DrawToWindow dtw; // declare the drawtowindow object

// DrawToWindow section:

// constructor
DrawToWindow::DrawToWindow()
{
  initialized = FALSE; // actually important
  picnodelinkedlist = NULL;
  timesincelastcall = 0;
  hwnd = NULL; // shutup warnings, I know they're not initalised
  bitmapbuffer1 = NULL; // I wasn't goint to use them without initalizing them anyway
  bitmapbuffer2 = NULL;
  handlebuffer1 = NULL;
  handlebuffer2 = NULL;
  thescreen = NULL;
  oldbitmapbuffer1 = NULL;
  oldbitmapbuffer2 = NULL;
  screenwidth = 0;
  screenheight = 0;
} // end constructor

// destructor
DrawToWindow::~DrawToWindow()
{
  if (initialized == TRUE) // do variables need destroying
  {

    SelectObject(handlebuffer1, oldbitmapbuffer1); // safely destroy the buffers
    SelectObject(handlebuffer2, oldbitmapbuffer2);
    DeleteDC(handlebuffer1);
    DeleteDC(handlebuffer2);
    DeleteObject(bitmapbuffer1);
    DeleteObject(bitmapbuffer2);

    // stepthrough linked list and delete every picnode here
    while (getfirstpicnode() != NULL)
    {
      Picnode::deletepic(getfirstpicnode());
    }

  } // endif do variables need destroying
} // end destructor

// this functions saves important variables and sets the class up to output to the screen
// warining: only the first initialize call will do anything
void DrawToWindow::Initialize(HWND newhwnd)
{
  if (initialized == FALSE)
  {
    hwnd = newhwnd;

    // get the stats for the size of the screen
    RECT screenstats;
    bool checkreturnvalue = TRUE;
    checkreturnvalue = GetWindowRect(hwnd,&screenstats);
    screenwidth = screenstats.right - screenstats.left;
    screenheight = screenstats.bottom - screenstats.top;

    thescreen = GetDC(hwnd); // gets a device context for the screen

    handlebuffer1 = CreateCompatibleDC(thescreen); // make device contexts and bitmaps for the buffers
    handlebuffer2 = CreateCompatibleDC(thescreen);
    bitmapbuffer1 = CreateCompatibleBitmap(thescreen, screenwidth, screenheight);
    bitmapbuffer2 = CreateCompatibleBitmap(thescreen, screenwidth, screenheight);

    // select the bitmaps for the buffers into the device contexts for the buffers, saving the old contents
    oldbitmapbuffer1 = (HBITMAP)SelectObject(handlebuffer1, bitmapbuffer1);
    oldbitmapbuffer2 = (HBITMAP)SelectObject(handlebuffer2, bitmapbuffer2);

    ReleaseDC(hwnd,thescreen); // safely destroys the device context for the screen

    if (checkreturnvalue == FALSE or handlebuffer1 == NULL or handlebuffer2 == NULL or bitmapbuffer1 == NULL or bitmapbuffer2 == NULL or oldbitmapbuffer1 == NULL or oldbitmapbuffer2 == NULL or hwnd == NULL)
    { // if any function call above failed
      MessageBox(NULL, "DrawToWindow Initialization Failed.", "Error", MB_ICONEXCLAMATION | MB_OK); // report error
      return; // initalized remails FALSE
    }
    initialized = TRUE;
  }
  return;
} // end Initialize

void DrawToWindow::Update()
{
  // copy the buffer over
  BitBlt(handlebuffer1, 0, 0, screenwidth, screenheight, handlebuffer2, 0, 0, SRCCOPY);

  Periodic(); // update the screen
} // end Update()


void DrawToWindow::Update(int timesincelastcall)
{
  Picnode* currentnode = picnodelinkedlist;
  while(currentnode != NULL)
  { // for-each-picnode
    currentnode->updateanimation(timesincelastcall);
    currentnode = currentnode->getnextpicnode();
  } // end for-each-picnode
  Update(); // note that updates to animation cycles above will not be shown until redrawn
} // end Update(int)


void DrawToWindow::Periodic()
{
  thescreen = GetDC(hwnd); // get a device context for the screen

  if (DEBUG == TRUE and DISPLAY_DTW_PERIODIC_HWND_USED == TRUE)
    cout << "hwnd: " << hwnd << "\n";

  BitBlt(thescreen, 0, 0, screenwidth, screenheight, handlebuffer1, 0, 0, SRCCOPY); // draw to the screen

  ReleaseDC(hwnd,thescreen); // safely destroys the device context for the screen
  thescreen = NULL;
} // end Periodic



int DrawToWindow::Paint()
{

  PAINTSTRUCT ps;
  thescreen = BeginPaint(hwnd, &ps); // get a device context for the screen

  BitBlt(thescreen, 0, 0, screenwidth, screenheight, handlebuffer1, 0, 0, SRCCOPY); // draw to the screen

  EndPaint(hwnd, &ps); // safely destroys the device context for the screen
  thescreen = NULL;

  return 0;
} // end Paint

// end DrawToWindow section

// Picnode section

// constructor
Picnode::Picnode(char* filename,char* transparencyfile,bool istransparent,int width,int height,int time,int newlastframe, bool isanimated, bool isautomatic)
{

  // set everything safely
  // note: a picture value of NULL means no image was loaded successfully
  // and a transparencymap value of NULL means that no transparancy map was loaded successfully
  // this is independant of bool transparency, which only records if it was intended to include transparency
  // both scenarios notify the user of a failed load of picture "filename"

  firstframe = 0;
  currentframe = 0;
  currentframex = 0;
  currentframey = 0;
  framewidth = (width > 0) ? width : 1; // prevents later divide by 0
  frameheight = (height > 0) ? height : 1;

  frameswide = 1; // will be overwritten if loading picture is successful
  framestall = 1;
  totalframes = 1;
  lastframe = 1;

  if ((picture = (HBITMAP)LoadImage(NULL,filename,0,0,0,LR_LOADFROMFILE)) != NULL)
  {
    if (NULL != (picturehandle = CreateCompatibleDC(NULL)))
    {
      if ((oldpicturehandlecontents = (HBITMAP)SelectObject(picturehandle,picture)) != NULL)
      {
        if ((transparency = istransparent) == TRUE)
        {
          if ((transparencymap = (HBITMAP)LoadImage(NULL,transparencyfile,0,0,0,LR_LOADFROMFILE)) != NULL)
          {
            if ((transparencymaphandle = CreateCompatibleDC(NULL)) != NULL)
            {
              if ((oldtransparencymaphandlecontents = (HBITMAP)SelectObject(transparencymaphandle,transparencymap)) != NULL)
              {
                goto transparencyestablished;
              }
              DeleteDC(transparencymaphandle);
            }
            DeleteObject(transparencymap);
            transparencymap = NULL;
          }
        } else
        {
          transparencymap = NULL;
        }
        transparencyestablished:; // transparency established

        if ((animated = isanimated) == TRUE)
        {
          timetonextframe = timebetweenframes = (time > 0) ? time : 1;
          if (time == 0)
          {
            automatic = FALSE;
          } else
          {
            automatic = TRUE;
          }
        } else
        {
          animated = FALSE;
          automatic = FALSE;
        }
        // animation properties established

        GetObject(picture, sizeof(pictureinfo), &pictureinfo); // get stats on the picture loaded
        frameswide = pictureinfo.bmWidth / framewidth;
        framestall = pictureinfo.bmHeight / frameheight;
        lastframe = totalframes = ((newlastframe < frameswide * framestall) ? newlastframe : (frameswide * framestall));

        nextpicnode = dtw.getfirstpicnode(); // add this picnode to the linked list
        if (nextpicnode != NULL)
        {
          nextpicnode->setlastpicnode(this);
        }
        lastpicnode = NULL;
        dtw.setfirstpicnode(this);

        return; // end this constructor

      }
      DeleteDC(picturehandle);
    }

    DeleteObject(picture);
  }
  string errorreport("Pic Loading failed: ");
  errorreport += filename;
  MessageBox(NULL,errorreport.c_str(), "Error", MB_ICONEXCLAMATION | MB_OK);
  picture = NULL;

} // end constructor

// destructor
Picnode::~Picnode()
{

  // safely destroy everything
  if (picture != NULL)
  {
    if (transparencymap != NULL)
    {
      SelectObject(transparencymaphandle,oldtransparencymaphandlecontents);
      DeleteDC(transparencymaphandle);
      DeleteObject(transparencymap);
    }
    SelectObject(picturehandle,oldpicturehandlecontents);
    DeleteDC(picturehandle);
    DeleteObject(picture);
  }

  // remove this picnode from the picnode linked list
  if (getlastpicnode() == NULL)
  {
    dtw.setfirstpicnode(getnextpicnode());
  } else
  {
    getlastpicnode()->setnextpicnode(getnextpicnode());
  }
  if (getnextpicnode() != NULL)
  {
    getnextpicnode()->setlastpicnode(getlastpicnode());
  }

} // end destructor

void Picnode::updateanimation(int timesincelastupdate) // updates animation frame and timer
{
  if (animated == TRUE and automatic == TRUE)
  { // if the picnode handles this animation
    timetonextframe -= timesincelastupdate;
    while (timetonextframe <= 0)
    { // time to change frame
      if  (currentframe +1 > totalframes or currentframe +1 > lastframe)
      { // if this is the final frame
        int x = (firstframe < 0) ? 0 : firstframe;
        currentframe = x;
        currentframex = (currentframe % frameswide) * framewidth;
        currentframey = (currentframe / frameswide) * frameheight;
      } else if(currentframe % frameswide == 0)
      { // if this is a rightmost frame
        currentframe++;
        currentframex = 0;
        currentframey += frameheight;
      } else
      { // if this is neither of the above
        currentframe++;
        currentframex += framewidth;
      }
      timetonextframe += timebetweenframes;
    } // end while time to change frame
  }// endif the picnode handles this animation


} // end updateanimation

void Picnode::setframe(int newcurrentframe)
{
  currentframe = newcurrentframe;
  currentframex = (currentframe % frameswide) * framewidth;
  currentframey = (currentframe / frameswide) * frameheight;
} // end setcurrentframe


void Picnode::drawpic(HDC drawtolocation,int x,int y) // draws this pic at x and y on drawtolocation
{

  int width = (animated) ? framewidth : pictureinfo.bmWidth;
  int height = (animated) ? frameheight : pictureinfo.bmHeight;

  if (transparency == FALSE)
  {
    BitBlt(drawtolocation,x,y,width,height,picturehandle,currentframex,currentframey,SRCCOPY);
  } else
  {
    BitBlt(drawtolocation,x,y,width,height,transparencymaphandle,currentframex,currentframey,SRCAND);
    BitBlt(drawtolocation,x,y,width,height,picturehandle,currentframex,currentframey,SRCPAINT);
  }

} // end drawpic

void Picnode::deletepic(Picnode* pictodelete) // call only once per pic, and do not use a deleted pic
{
  delete pictodelete;
} // end deletepic

// end Picnode section
