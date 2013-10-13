/********************************************************
  State.h

  Header File for state vector class

  Gina Guerrero - Fall 2013
********************************************************/


#ifndef _STATEVECTOR_H_
#define _STATEVECTOR_H_

#include "Vector.h"

class State {
	private:
		Vector3d *StateVector;          // Vector of Vector3ds


	public:
        State();
		~State();
		void SetSize(int numofp);

};

#endif
