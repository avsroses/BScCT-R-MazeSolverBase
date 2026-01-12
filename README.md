# Pololu 3Pi
This project is for a maze solving robot
# Prerequisites
## Hardware
- [Pololu 3Pi 32U4 OLED Robot](https://www.pololu.com/product/4975)
## Software
- ArduinoIDE
-
-
# Getting Started
## Hardware
Pololu
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
