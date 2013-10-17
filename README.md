PROJECT
==================================================================

	Gina Guerrero

	CpSc8170 - Fall 2013

	Project #3 - Flocking and Interacting Particle Systems

	C++/OpenGL



DESCRIPTION
==================================================================

	Flocking/Interacting particle system

	Gauss/Matrix/Vector/Utility by Dr. House
	Model Class modified to create an isosphere & plane

	Usage: particles [param_filename]


KEY COMMANDS
==================================================================
	m or M		IMPORTANT: transforming camera view to simulation control (default: camera)
	p or P		switches from ortho to perspective views (default: perspective)
	r or R      	resets the simulation, so if you change parameters - this will re-read them
	1 to 0		should trigger a dispersing force
				1 = +X, +Y, +Z quadrant
				2 = +X, -Y, +Z quadrant
				3 = -X, -Y, +Z quadrant
				4 = -X, +Y, +Z quadrant
				5 = +X, +Y, -Z quadrant
				6 = +X, -Y, -Z quadrant
				7 = -X, -Y, -Z quadrant
				8 = -X, +Y, -Z quadrant
				9 = random point...somewhere...over the rainbow
				0 = center of dispersal force is (0,0,0)
	q or ESC	quit


MOUSE COMMANDS
==================================================================
	SIMULATION:
	RMB		opens menu
	LMB 		triggers selected option

	CAMERA: 		  left button		 |	   middle button  	|		right button
	left drag	(-) rotation: model's y	 |  (+)-r: camera's y	|  (+) translation: camera's z
	right drag	(+) rotation: model's y	 |  (-)-r: camera's y	|  (-) translation: camera's z
	down drag	(+) rotation: model's x	 |  (+)-r: camera's x	|  (+) translation: camera's z
	up drag		(-) rotation: model's x	 |  (-)-r: camera's x	|  (-) translation: camera's z


MENU COMMANDS
==================================================================
	Continuous/Step		changes simulation between continuous or step
	Quit				quit


FILES
==================================================================
	particles.cpp (main program)
	State.h, State.cpp
	Model.cpp, Model.h
	Entity.cpp, Entity.h
	Attributes.cpp, Attributes.h
	Pgenerator.cpp, Pgenerator.h
	Pmanager.cpp, Pmanager.h,
	Particle.cpp, Particle.h


MISC FILES
==================================================================
	README.md
	Makefile
	Matrix.cpp, Matrix.h
	Utility.cpp, Utility.h
	Vector.cpp, Vector.h
	gauss.cpp, gauss.h
	p


SAMPLE OUTPUT FILES
==================================================================
	screencap.png


TOTAL FILE COUNT
==================================================================
29
