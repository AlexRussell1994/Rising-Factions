# Rising-Factions

This is the code I've written so far for an RTS game I'd like to make someday. It's written in C++ using the Windows API. As it stands it includes the following:

-a menu from which you can start or quit the game
-a tiled background, dirt by default but individual tiles can be changed by mouse location + key press
-the ability to spawn people, building foundations, trees and bushes
-the ability to get the people to gather resources and either contribute them to a building under construction or add them to a resource stockpile (which is a building)

Known problems:

-the graphics are processed purely in the cpu, and inefficiently at that, such that I keep the game in its current state limited to 5 frames per second. While I could make this more efficient, I want to use the graphics card too, and am leaving this until I've learned enough OpenGL to replace the Windows API portions of the program.
-this includes an algorithm for pathing that causes people to navigate in the shortest route around any impassable terrain, but as of right now I'm still working the bugs out of it, so I just have this version ignore impassable terrain.
