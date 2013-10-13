#----------------------------------------------
#  Makefile for CpSc8170 - Proj1: Bouncing Balls
#----------------------------------------------
#
#  Gina Guerrero
#  August 28, 2013
#

CC      = g++
C	= cpp
H	= h
CFLAGS 	= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lm -L/usr/local/lib -lGLU
  endif
endif

HFILES 	= Matrix.${H} Vector.${H} Utility.${H} Model.${H} Attributes.${H} Entity.${H} gauss.${H} Particle.${H} Pmanager.${H} Pgenerator.${H}
OFILES 	= Matrix.o Vector.o Utility.o Model.o  Attributes.o Entity.o gauss.o Particle.o Pmanager.o Pgenerator.o
PROJECT = particles

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

Pgenerator.o: Pgenerator.${C} Pgenerator.${H}
	${CC} ${CFLAGS} -c ${INCFLAGS} Pgenerator.${C}

Pmanager.o: Pmanager.${C} Pmanager.${H}
	${CC} ${CFLAGS} -c ${INCFLAGS} Pmanager.${C}

Particle.o: Particle.${C} Particle.${H}
	${CC} ${CFLAGS} -c ${INCFLAGS} Particle.${C}

gauss.o: gauss.${C} gauss.${H}
	${CC} ${CFLAGS} -c ${INCFLAGS} gauss.${C}

Entity.o: Entity.${C} Entity.${H}
	${CC} ${CFLAGS} -c ${INCFLAGS} Entity.${C}

Attributes.o: Attributes.${C} Attributes.${H}
	${CC} ${CFLAGS} -c ${INCFLAGS} Attributes.${C}

Model.o: Model.${C} Model.${H}
	${CC} ${CFLAGS} -c ${INCFLAGS} Model.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c Matrix.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}
