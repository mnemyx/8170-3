/********************************************************
  State.cpp

  Source File for the state vector class

  Gina Guerrero - Fall 2013
********************************************************/

#include "State.h"

using namespace std;

///////////////////////////// PUBLIC FUNCTIONS /////////////////////////
State::State() {
    nmaxp = 0;
    StateVector = NULL;
    X = NULL;
}

State::~State() {
    if(StateVector != NULL) {
        delete[] StateVector;
        StateVector = NULL;
    }

    if(X != NULL) {
        delete[] X;
        X = NULL;
    }
}

State::State(const State& o) {
    if(nmaxp != o.nmaxp) {
      cerr << "error in copying - state must have same size" << endl;
      exit(1);
    }

    int i;
    for (i = 0; i < o.nmaxp * 2; i++) {
        StateVector[i] = o.StateVector[i];
        X[i] = o.X[i];
    }
}

State& State::operator=(const State& o) {
    State temp(o);

    swap(StateVector, temp.StateVector);
    swap(X, temp.X);

    return *this;
}

void State::SetSize(int numofp) {
    nmaxp = numofp;
    StateVector = new Vector3d[numofp * 2];
    X = new Vector3d[numofp * 2];
}

Vector3d& State::operator[](int i) {
    if(i < 0 || i > (nmaxp * 2)) {
        cerr << " out of bounds: state class (state.cpp line 30) -- i: " << i << " & nmaxp*2: " << nmaxp * 2 << endl;
        exit(1);
    }

    return StateVector[i];
}

//////////////////////////// CALCULATIONS /////////////////////////////
Vector3d State::Acceleration(float t, int indx, Env e) {
    if (e.Wind.x == 0 && e.Wind.y == 0 && e.Wind.z == 0)
        return (e.G - e.Viscosity * StateVector[indx + nmaxp]);
    else
        return (e.G + e.Viscosity * (e.Wind - StateVector[indx + nmaxp]));
}


void State::Force(float t, double m, Env e) {
    int i;

    for (i = 0; i < nmaxp; i++) {
        X[i] = StateVector[nmaxp + i];
        X[nmaxp + i] = (1 / m) * Acceleration(t, i, e);
    }
}


void State::SetState(float t) {
    int i;

    for (i = 0; i < (nmaxp * 2); i++) {
        StateVector[i] = StateVector[i] + X[i] * t;
    }
}

//////////////////////////// ACCESS FUNCTIONS /////////////////////////

void State::AddState(int indx, Vector3d c, Vector3d v) {
    StateVector[indx] = c;
    StateVector[indx + nmaxp] = v;
}

void State::MoveState(int indx, int maxused) {
    Vector3d tempc = StateVector[indx];
    Vector3d tempv = StateVector[indx + nmaxp];

    StateVector[indx] = StateVector[maxused];
    StateVector[indx + nmaxp] = StateVector[maxused + nmaxp];
}

void State::RemoveState(int indx) {
    Vector3d tempc = StateVector[indx];
    Vector3d tempv = StateVector[indx + nmaxp];

    StateVector[indx].set(0,0,0);
    StateVector[indx + nmaxp].set(0,0,0);
}

void State::GetState(int indx, Vector3d *c, Vector3d *v) {
    *c = StateVector[indx];
    *v = StateVector[indx + nmaxp];
}

Vector3d State::GetCenter(int indx) {
    return StateVector[indx];
}

void State::PrintState() {
    int i;

    for (i = 0; i < nmaxp * 2; i++) {
        cout << i << ": ";
        StateVector[i].print();
        cout << endl;
    }
}

