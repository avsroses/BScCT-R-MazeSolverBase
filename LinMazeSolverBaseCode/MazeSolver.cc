#include <Pololu3piPlus32U4.h>
#include <PololuMenu.h>

using namespace Pololu3piPlus32U4;

#include "MazeSolver.h"
#include "Shared.h"
MazeSolver::MazeSolver() {
  state = LINE_FOLLOWER;
}

//***********
//FOLLOW LINE
//follows line accounting for corrections 
//***********
void MazeSolver::followLine() {
  // get position & error
  int16_t position = lineSensors.readLineBlack(lineSensorValues);
  int16_t error = position - 2000;

  // calculate speed difference with PID formula
  int16_t speedDifference = error * (int32_t)proportional / 256 + (error - lastError) * (int32_t)derivative / 256;

  // store error
  lastError = error;

  // get new speed & constrain
  int16_t leftSpeed = (int16_t)baseSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)baseSpeed - speedDifference;
  leftSpeed = constrain(leftSpeed, minSpeed, (int16_t)maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, (int16_t)maxSpeed);

  // update motor speed
  motors.setSpeeds(leftSpeed, rightSpeed);
}

//***********************
//CHECKING FOR A JUNCTION
//Check sensors to see if there could be a junction
//***********************
void MazeSolver::checkIfJunction() {
  lineSensors.readLineBlack(lineSensorValues);

  bool junction = false;

  if (lineSensorValues[0] > 950) junction = true;  // detect a line to the left
  if (lineSensorValues[1] > 950) junction = true;  // detect a line to the left
  if (lineSensorValues[3] > 950) junction = true;  // detect a line to the right
  if (lineSensorValues[4] > 950) junction = true;  // detect a line to the right
  // any other case contains one of these types

  if (junction) {
    state = JUNCTION;
    motors.setSpeeds(0, 0);
  }
}

//******************
//CHECK FOR DEAD END
//checks if line ends
//******************
void MazeSolver::checkIfDeadEnd() {
  lineSensors.readLineBlack(lineSensorValues);
  if (lineSensorValues[2] < 500) {
    state = U_TURN;
  } 
}


//*****************
//IDENTIFY JUNCTION
//decide what junction is and change state accordingly
//*****************
void MazeSolver::identifyJunction() {

  delay(500);

  // move forward to identify other junctions
  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(260);
  motors.setSpeeds(0, 0);
  lineSensors.readLineBlack(lineSensorValues);

  // if can sense everywhere -> FINISHED
  if (lineSensorValues[0] > 950 && lineSensorValues[1] > 950 && lineSensorValues[2] > 950 && lineSensorValues[3] > 950 && lineSensorValues[4] > 950) {
    state = FINISHED;
    return;
  }

  // if there's a left take it
  if (lineSensorValues[0] > 750) {
    state = TURN_LEFT;
    if(lineSensorValues[2] > 750 || lineSensorValues[4] > 750) {
      state = TURN_LEFT;
      addDecision(LEFT);
      displayOnScreen();
    }
    return;
  } 

  if (lineSensorValues[2] > 750) {
    motors.setSpeeds(baseSpeed, baseSpeed);
    delay(110);
    state = LINE_FOLLOWER;
    //memory
    addDecision(FORWARD);
    displayOnScreen();
    return;
  }

  if (lineSensorValues[4] > 750) {
    // addDecision(RIGHT);
    // displayOnScreen();
    state = TURN_RIGHT;
    return;
  }

  // any other case -> keep going
  state = LINE_FOLLOWER;
}



//bool first = true;

//*********
//LEFT TURN
//turns the robots direction to the left
//*********
void MazeSolver::turnLeft() {
  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(250);
  motors.setSpeeds(0, 0);
  //turns robot left
  motors.setSpeeds(-baseSpeed, baseSpeed);
  delay(780);
  motors.setSpeeds(0, 0);
  state = LINE_FOLLOWER;
}

//**********
//RIGHT TURN
//turns the robots direction to the right
//**********
void MazeSolver::turnRight() {
  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(250);
  motors.setSpeeds(0, 0);
  //turns robot right
  motors.setSpeeds(baseSpeed, -baseSpeed);
  delay(780);
  motors.setSpeeds(0, 0);
  state = LINE_FOLLOWER;
}

//******
//U-TURN
//turns robot around to face opposite direction
//******
void MazeSolver::uTurn() {
  addDecision(BACK);
  displayOnScreen();
  motors.setSpeeds(-baseSpeed, baseSpeed);
  delay(1600);
  motors.setSpeeds(0, 0);
  state = LINE_FOLLOWER;
}


//************
//ADD DECISIONS
//remove unnecessary paths
//************
void MazeSolver::addDecision(Decisions d) {
  path[count] = d;

  if(path[count -1] == BACK) {
    if(path[count - 2] == LEFT) {
      if(path[count] == LEFT){ //LBL = F
        path[count - 2] = FORWARD;
        path[count] = NONE;
        path[count - 1] = NONE;
        count--;
        return; 
      } else if(path[count] == FORWARD) { //LBF = R
        path[count - 2] = RIGHT;
        path[count] = NONE;
        path[count - 1] = NONE;
        count--;
        return; 
      }
    }
    else if(path[count - 2] == FORWARD) { 
      if(path[count] == LEFT) { //FBL = R
        path[count - 2] = RIGHT;
        path[count] = NONE;
        path[count - 1] = NONE;
        count--;
        return; 
      } else if(path[count] == FORWARD) { //FBF = B
        path[count - 2] = BACK;
        path[count] = NONE;
        path[count - 1] = NONE;
        count--;
        return; 
      }
    }
    else if(path[count - 2] == RIGHT) { //RBL = B
      if(path[count] == LEFT){
        path[count - 2] = BACK;
        path[count] = NONE;
        path[count - 1] = NONE;
        count--;
        return;        
      }
    }
  }
  count ++;

}

//**********************
//DIRECTION TO CHARACTER
//Changing numerial value of direction to a letter
//**********************
char MazeSolver::directionToCharacter(Decisions d) {
  if (d == NONE) {
    return ' ';
  }else if (d == RIGHT){
    return 'R';
  } else if (d == LEFT){
    return 'L';
  } else if (d == BACK) {
    return 'B';
  } else if (d == FORWARD) {
    return 'F';
  }
}

//**********************
//DISPLAY PATH ON SCREEN
//displays the path of decisions on screen
//**********************
void MazeSolver::displayOnScreen() {
  display.gotoXY(0, 0);
  for (int i = 0; i <= 7; i++) {
    // Direction d = path[i];
    // char character = directionToCharacter(d);
    display.print(directionToCharacter(path[i]));
  }

  display.gotoXY(0, 1);
  for (int j = 8; j <= 15; j++) {
    display.print(directionToCharacter(path[j]));
  }
}

//PUBLIC FUNCTIONS

//**************
//IS IT FINISHED
//checks if the Pololu has reached the finish
//**************
bool MazeSolver::isFinished() {
  if(state == FINISHED) {
    return true;
  } else {
    return false;
  }
}


//*****************
//THE LOOP FUNCTION
//changes the state that the robot follows based on checking for a junction or dead end
//*****************
void MazeSolver::loop() {
  // display.clear();
  display.gotoXY(0, 0);
  // display.print(state);

  if (state == LINE_FOLLOWER) {
    followLine();
    //check if junction there's a junction and change state otherwise
    checkIfJunction();
    checkIfDeadEnd();
  }

  if (state == JUNCTION) {
    identifyJunction();
  }

  if (state == TURN_LEFT) {
    turnLeft();
  }

  if (state == TURN_RIGHT) {
    turnRight();
  }
  if (state == U_TURN) {
    uTurn();
  }
  if (state == FINISHED) {
    motors.setSpeeds(0, 0);
    return;
  }

  if (state == FAKE_END) {
    display.clear();

    while (!buttonB.getSingleDebouncedPress()) {
      uint16_t position = lineSensors.readLineBlack(lineSensorValues);

      display.gotoXY(0, 0);
      display.print(position);
      display.print("    ");
      display.gotoXY(0, 1);
      for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        uint8_t barHeight = map(lineSensorValues[i], 0, 1000, 0, 8);

        if (barHeight > 8) { barHeight = 8; }
        const char barChars[] = { ' ', 0, 1, 2, 3, 4, 5, 6, (char)255 };
        display.print(barChars[barHeight]);
      }

      delay(50);
    }
  }
}
