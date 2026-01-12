# Pololu 3Pi
This project is for a maze solving robot
# Prerequisites

## Hardware
- [Pololu 3Pi 32U4 OLED Robot](https://www.pololu.com/product/4975)
  
## Software
- ArduinoIDE
- Pololu A-Star Boards
  
# Getting Started

## Hardware

Pololu 3Pi

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
### MazeSolver.h
There are two enums in this file, one for the current state of the Pololu and one for the decisions that can be made by it. These enums are then used in other files (MazeSolver.cc, SolutionFollower.cc) to call on different functions
### SolutionFollower.h
### FAKE_END
FAKE_END is a variable that can be used for debugging. It's declared in the state enum and its code can be found in the loop function of both MazeSolver.cc and SolutionFollower.cc. 
The code is used to bring the Pololu to a halt and display the sensors reading in a bar graph style, to show where it is detecting a black line.
