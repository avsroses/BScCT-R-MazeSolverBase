# Pololu 3Pi
This project is for a maze solving robot that can navigate a maze to find the optimal route and then remember that route to retraverse the maze.
# Prerequisites

## Hardware
- [Pololu 3Pi 32U4 OLED Robot](https://www.pololu.com/product/4975)
  
## Software
- ArduinoIDE
- Pololu A-Star Boards
- Pololu3PiPlus32U4 library
  
# Getting Started

## Hardware

Pololu 3Pi - the pololu requires four AAA batteries and a USB A to Micro-B cable to connect to a computer.

## Software

To connect the Pololu to the Arduino IDE you must install the board drivers. To do so follow the steps below:

1. In the Arduino IDE, open the File menu (Windows/Linux) or the Arduino menu (macOS) and select "Preferences".
   
2. In the Preferences dialog, find the "Additional Boards Manager URLs" text box. Copy and paste the following URL into this box:
   
  **'https://files.pololu.com/arduino/package_pololu_index.json'**
  
  If there are already other URLs in the box, you can either add this one
  separated by a comma or click the button next to the box to open an input
  dialog where you can add the URL on a new line.
  
3. Click the "OK" button to close the Preferences dialog.

4.  In the **Tools > Board** menu, select "Boards Manager..." (at the top of the
    menu).

5.  In the Boards Manager dialog, search for "Pololu A-Star Boards".

6.  Select the "Pololu A-Star Boards" entry in the list, and click the
    "Install" button.
    
After installing the boards, you will now need to install the Pololu3PiPlus32U4 library. This is done by simply searching for Pololu3PiPlus32U4 in the libraries manager. 

## Notes

### FAKE_END
FAKE_END is a variable that can be used for debugging. It's declared in the state enum and its code can be found in the loop function of both MazeSolver.cc and SolutionFollower.cc. 
The code is used to bring the Pololu to a halt and display the sensors reading in a bar graph style, to show where it is detecting a black line. This state may be needed when setting up to change the delays throughout the code. To do this the state after a movement should be changed from LINE_FOLLOWER to FAKE_END. This will allow you to see if the Pololu has rotated too much or too little. This is important to check and experiment with for the code to run smoothly.

### LinMazeSolverBaseCode.ino
In this file you will find variables called 'proportional' and 'derivative'. If the Pololu's movements are "shaky" changing these numbers to be higher or lower can help make the movements smoother.

### MazeSolver.h
There are two enums in this file, one for the current state of the Pololu and one for the decisions that can be made by it. These enums are then used in other files (MazeSolver.cc, SolutionFollower.cc) to call on different functions

### SolutionFollower.cc and MazeSolver.cc
SolutionFollower.cc has many of the same functions as MazeSolver.cc, as they operate in similar ways. MazeSolver.cc is used for the first navigation of the maze, when the robot has to explore every route and track the decisions in a vector - Decisions path[64]. SolutionFollower.cc then follows the path using functions to identify junctions, turn left, turn right, and finish. 

