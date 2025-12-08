
#include <Pololu3piPlus32U4.h>
#include <PololuMenu.h>

using namespace Pololu3piPlus32U4;

#include "MazeSolver.h"
#include "Shared.h"
MazeSolver::MazeSolver() {
  state = LINE_FOLLOWER;
}

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


//check for junction 
void MazeSolver::checkPathChange() {
  if ((lineSensorValues[1] >= 950 && lineSensorValues[0] >= 400) || (lineSensorValues[3] >= 950 && lineSensorValues[4]>= 400)) {
    state = JUNCTION;
  }
}

bool first = true;

void MazeSolver::identifyPathChange() {
  if(!first) return;
  first = false;
  display.clear();
  //display.print(state);
  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(300);
  motors.setSpeeds(0,0);
  lineSensors.readLineBlack(lineSensorValues);

  if(lineSensorValues[0] >= 950 && lineSensorValues[1] >= 950 && lineSensorValues[2] >= 950 && lineSensorValues[3] >= 950 && lineSensorValues[4] >= 950) {
    state = FINISHED;
    return;
  } else if (lineSensorValues[0] >= 950) {
    state = TURN_LEFT;
    return;
  } else {
    motors.setSpeeds(0, 0);
    //state = LINE_FOLLOWER;
  }
}


void MazeSolver::turnLeft() {
  motors.setSpeeds(minSpeed , maxSpeed);
  delay(600);
  motors.setSpeeds(0,0);
  state = LINE_FOLLOWER;
}

void MazeSolver::checkIfDeadEnd() {
  if(lineSensorValues[2] <= 50 && lineSensorValues[1] <= 50 && lineSensorValues[3] <= 50 && lineSensorValues[0] <= 50 && lineSensorValues[4] <= 50) {
    state = U_TURN;
  }
}

void MazeSolver::uTurn() {
  motors.setSpeeds(minSpeed, maxSpeed);
  delay(800);
  motors.setSpeeds(0,0);
  state = LINE_FOLLOWER;
}

void MazeSolver::loop() {
  if (state == LINE_FOLLOWER) {
    display.clear();
    display.print('A');
    followLine();
    checkPathChange();
    //checkIfDeadEnd();
  
  }

  if (state == JUNCTION) {
    // call junciton identifier function
    display.clear();
    display.print('J');
    identifyPathChange();
  }
  if (state == TURN_LEFT) {
    // call left turn function
    motors.setSpeeds(0, 0);
    display.clear();
    display.print('L');
    //turnLeft();
  }
  if (state == U_TURN) {
    // call u turn function
    motors.setSpeeds(0, 0);
    display.clear();
    display.print('U');
    //uTurn();
  }
  if (state == FINISHED) {
    motors.setSpeeds(0, 0);
    display.clear();
    display.print('F');
    return;
  }
}
