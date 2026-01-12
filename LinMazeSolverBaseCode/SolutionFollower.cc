#include "SolutionFollower.h"
#include "Shared.h"

//***********
//FOLLOW LINE
//follows line accounting for corrections 
//***********
void SolutionFollower::followLine() {
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


//*********
//LEFT TURN
//turns the robots direction to the left
//*********
void SolutionFollower::turnLeft() {
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
void SolutionFollower::turnRight() {
  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(250);
  motors.setSpeeds(0, 0);
  //turns robot right
  motors.setSpeeds(baseSpeed, -baseSpeed);
  delay(780);
  motors.setSpeeds(0, 0);
  state = LINE_FOLLOWER;
}

