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
	b or B		changs from blend to non-blend mode
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
	Model.cpp, Model.h
	Entity.cpp, Entity.h
	Attributes.cpp, Attributes.h
	Pgenerator.cpp, Pgenerator.h
	Pmanager.cpp, Pmanager.h,
	Particle.cpp, Particl.h


MISC FILES
==================================================================
	README.md
	Makefile
	Matrix.cpp, Matrix.h
	Utility.cpp, Utility.h
	Vector.cpp, Vector.h
	gauss.cpp, gauss.h


SAMPLE OUTPUT FILES
==================================================================
	N/A


TOTAL FILE COUNT
==================================================================
26
