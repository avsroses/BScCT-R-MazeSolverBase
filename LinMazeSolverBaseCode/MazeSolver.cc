
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
bool MazeSolver::checkPathChange() {
  if ((lineSensorValues[1] >= 950 && lineSensorValues[0] >= 400) || (lineSensorValues[3] >= 950 && lineSensorValues[4]>= 400)) {
    state = JUNCTION;
    motors.setSpeeds(0,0);
    return true;
  }
}

// bool first = true;

void MazeSolver::identifyPathChange() {
  // if(!first) return;
  // first = false;
  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(50);
  motors.setSpeeds(0,0);
  lineSensors.readLineBlack(lineSensorValues);

  //detects solid black line in all sensors
  if(lineSensorValues[0] >= 950 && lineSensorValues[1] >= 950 && lineSensorValues[2] >= 950 && lineSensorValues[3] >= 950 && lineSensorValues[4] >= 950) {
    state = FINISHED;
    return;
  //the left sensor detects a black line
  } else if (lineSensorValues[0] >= 950) {
    state = TURN_LEFT;
    return;
  //junction must be a straight ahead and right, so robot goes straight
  } else {
    motors.setSpeeds(0, 0);
    state = LINE_FOLLOWER;
  }
}


void MazeSolver::turnLeft() {
  motors.setSpeeds(minSpeed , maxSpeed);
  delay(350);
  motors.setSpeeds(0,0);
  state = LINE_FOLLOWER;
}

void MazeSolver::checkIfDeadEnd() {
  if(lineSensorValues[2] <= 20 && lineSensorValues[1] <= 20 && lineSensorValues[3] <= 20 && lineSensorValues[0] <= 20 && lineSensorValues[4] <= 20) {
    state = U_TURN;
  }
}

void MazeSolver::uTurn() {
  motors.setSpeeds(minSpeed, maxSpeed);
  delay(600);
  motors.setSpeeds(0,0);
  delay(200);
  motors.setSpeeds(maxSpeed, maxSpeed);
  delay(200);
  state = LINE_FOLLOWER;
}

void MazeSolver::loop() {
  if (state == LINE_FOLLOWER) {
    display.clear();
    display.print("LINE");
    followLine();
    checkPathChange();
    if (checkPathChange() != true) {
      checkIfDeadEnd();
    } 
    else {
    }
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
    turnLeft();
  }
  if (state == U_TURN) {
    // call u turn function
    motors.setSpeeds(0, 0);
    display.clear();
    display.print('U');
    uTurn();
  }
  if (state == FINISHED) {
    motors.setSpeeds(0, 0);
    display.clear();
    display.print('F');
    return;
  }
}
