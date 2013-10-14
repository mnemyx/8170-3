/********************************************************
  State.h

  Header File for state vector class

  Gina Guerrero - Fall 2013
********************************************************/


#ifndef _STATEVECTOR_H_
#define _STATEVECTOR_H_

#include "Vector.h"

struct Env {
    Vector3d G;
    Vector3d Wind;
    double Viscosity;
 };

class State {
	private:
        int nmaxp;
		Vector3d *StateVector;          // Vector of Vector3ds
		Vector3d *X;

	public:
        State();
		~State();
		State(const State& other);
		State& operator= (const State& other);

		void SetSize(int numofp);
		Vector3d& operator[](int i);

        Vector3d Acceleration(float t, int indx, Env e);
        void Force(float t, double m, Env e);

        void SetState(float timestep);

		void AddState(int indx, Vector3d center, Vector3d velocity);
		void MoveState(int indx, int maxindx);
		void RemoveState(int indx);
		void GetState(int indx, Vector3d *c, Vector3d *v);
		Vector3d GetCenter(int indx);

		void PrintState();           // debugging
};



#endif
