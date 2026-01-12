#ifndef SOLUTION_H
#define SOLUTION_H
#include "MazeSolver.h"

class SolutionFollower {
  private:
  State state; //value of type state

  //display path on screen
  void displayOnScreen();
  //follows line
  void followLine();
  //checking if there is a junction
  void checkIfJunction();
  //checking what type of junction
  void identifyJunction();
  //turn direction left
  void turnLeft();
  //turn direction right
  void turnRight();


  public:

  Decisions path[64];
  // function to be called at every main loop
  void loop();

};

#endif