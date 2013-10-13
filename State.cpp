/********************************************************
  State.cpp

  Source File for the state vector class

  Gina Guerrero - Fall 2013
********************************************************/

#include "State.h"

using namespace std;

///////////////////////////// PUBLIC FUNCTIONS /////////////////////////
State::State() {
    StateVector = NULL;
}

State::~State() {
    delete[] StateVector;
}

void State::SetSize(int numofp) {
    StateVector = new Vector3d[numofp * 2];
}
