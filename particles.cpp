/********************************************************
  particles.cpp

  CPSC8170 - Proj 2   GBG   9/2013
*********************************************************/

#include "Pmanager.h"
#include "Pgenerator.h"
#include "Entity.h"

#include <cstdlib>
#include <cstdio>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define WINDOW_WIDTH	800		/* window dimensions */
#define WINDOW_HEIGHT	600

#define MAXSTEPS	10000

#define MenuContinuous	1	// menu; switch from continues to step
#define MenuQuit	2		// quits program

#define NEAR		10		// distance of near clipping plane
#define FAR		1000		// distance of far clipping plane
#define DEPTH		-500	// initial z coord. of center of cube

#define ORTHO		0		// projection system codes
#define PERSPECTIVE	1
#define NONE		-1		// used to indicate no mouse button pressed

#define ROTFACTOR	0.2     // degrees rotation per pixel of mouse movement
#define XLATEFACTOR	0.5     // units of translation per pixel of mouse movement

#define DRAWWIDTH	200		// view volume sizes (note: width and
#define DRAWHEIGHT	150		//   height should be in same ratio as window)

#define AMBIENT_FRACTION 0.1	// lighting
#define DIFFUSE_FRACTION 0.4
#define SPECULAR_FRACTION 0.4


/******************* SHADING & COLORS *****************************/
const float BRIGHT_PALEBLUE[] = {0.5, 0.5, 1, 0.25};
const float WHITE[] = {1, 1, 1, 1};
const float VIOLET[] = {1, 0, 0.5, 1};
const float YELLOW[] = {1, 1, 0, 1};
const float BG[] = {0, 0, 0};

float hues[][4] = { {1, 1, 1},    // white
		    {0.5, 0.5, 1},    // dim paleblue
		    {1, 0, 0.5, 0},  // violet
		    {1, 1, 0},    // yellow
		  };


/*** Global variables updated and shared by callback routines ***/
// Viewing parameters
static int Projection;
// Camera position and orientation
static double Pan;
static double Tilt;
static double Approach;

// model orientation
static double ThetaX;
static double ThetaY;

// global variables to track mouse and shift key
static int MouseX;
static int MouseY;
static int Button = NONE;

static int MenuAttached;

static double WinWidth = WINDOW_WIDTH;
static double WinHeight = WINDOW_HEIGHT;
static int MiddleButton = false;


/********************** FOR THE ACTUAL SIMULATION ********************/
int FrameNumber = 0;

void TimerCallback(int value);

static char *ParamFilename = NULL;

static double TimeStep;
static double DispTime;
static int TimeStepsPerDisplay;
static int TimerDelay;

static int NSteps = 0;
static int NTimeSteps = -1;
static double Time = 0;

static int Toggle = 0;

Pmanager Manager;
Pgenerator Generator1, Generator2;
Entity Pl;

static int AllowBlend = true;

struct Env {
    Vector3d G;
    Vector3d Wind;
    double Viscosity;
 } env;

struct Attractor {
    Vector3d g;
    Vector3d center;
    double r;
} pa1;

/************** DRAWING & SHADING FUNCTIONS ***********************/
//
// Get the shading setup for the objects
//
void GetShading(int hueIndx) {
  float ambient_color[4];
  float diffuse_color[4];
  float specular_color[4];
  int shininess;

    // set up material colors to current hue.
    for(int i = 0; i < 3; i++)
      ambient_color[i] = diffuse_color[i] = specular_color[i] = 0;

	for(int i = 0; i < 3; i++) {
		ambient_color[i] = AMBIENT_FRACTION * hues[hueIndx][i];
		diffuse_color[i] = DIFFUSE_FRACTION * hues[hueIndx][i];
		specular_color[i] = SPECULAR_FRACTION * hues[0][i];
		shininess = 60;
	}

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

//
// Draw the moving objects
//
void DrawMovingObj() {
    Manager.DrawSystem();
}

//
// Draw the non moving objects
//
void DrawNonMovingObj() {
}

//
//  Draw the ball, its traces and the floor if needed
//
void DrawScene(int collision){

  int i,j;
  Model p;

  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);

  //DrawNonMovingObj();
  DrawMovingObj();

  glutSwapBuffers();
}


/********************* SIMULATE FUNCTION **********************/
///
//  Run a single time step in the simulation
//
void Simulate(){
    int i, j, p, phit,ahit;
    float f, fhit;
    Vector3d temphit, p0, p1, g;

    // don't do anything if our simulation is stopped
    if(Manager.IsStopped()) {
        return;
    }

    // for now, kill off particles whose age > 20
    int killed = Manager.KillParticles(Time);

    //cout << "killed: " << killed << endl;
    Generator1.MoveGenerator(TimeStep);
    Generator2.MoveGenerator(TimeStep);

    // for every particle the manager has....
    for (j = 0; j < Manager.GetNused(); j++ ) {
         //Manager.Particles[j].A.PrintAttr();
         //cout << "j: " << j << endl;

        // get the new acceleration
        Manager.Particles[j].A.CalcAccel(env.G, env.Wind, env.Viscosity);

        //check if we hit within the attractor's grasp
        if((Manager.Particles[j].A.GetCenter() - pa1.center).norm() < pa1.r)
            Manager.Particles[j].A.CalcPtAttract(pa1.center, pa1.g);


        // evil Euler integration to get velocity and position at next timestep
        Manager.Particles[j].A.CalcTempCV(TimeStep);

        // Manager.Particles[j].PrintAttr();
        if((int)Manager.Particles[j].GetAge(Time) % 5 == 0)
            Manager.Particles[j].A.SetColor(Generator1.GenerateColor(Manager.Particles[j].A.GetColor()));


        phit = -1;
        phit = Pl.CheckCollision(Manager.Particles[j].A.GetCenter(), Manager.Particles[j].A.GetTempv(), Manager.Particles[j].A.GetTempc());

        if(phit != -1) {
          //reflect it from the plane -- data during collision
          Manager.Particles[j].A.Reflect(Pl.GetNormal(phit), Pl.GetVertex(Pl.GetTriangle(phit).x));

          //DrawScene(1, ihit);  // should do something with this in terms of collision; change draw scene function
        } else {
            Manager.Particles[j].A.SetVelocity(Manager.Particles[j].A.GetTempv());
            Manager.Particles[j].A.SetCenter(Manager.Particles[j].A.GetTempc());
        }

        //cout << "Im adding inside the simulate for loop... " << endl;
        Manager.Particles[j].AddHistory(Manager.Particles[j].A.GetCenter());
         //Manager.Particles[j].PrintInfo();
    }

    //cout << "ADDING!! " << endl;
    // generate particles if we can
    if(Manager.HasFreeParticles()) {
    //cout << "Manager.FreePLeft(): " << Manager.FreePLeft() << endl;
        for(i = 0; i < Manager.FreePLeft() - 1 && i < Generator1.GetPNum() + Generator2.GetPNum(); i++) {
            Generator1.GenerateAttr(1);
            Manager.UseParticle(Generator1.GenC0(), Generator1.GenV0(), Time, Generator1.GenC0(), Generator1.GenMass(), Generator1.GetCoefff(), Generator1.GetCoeffr(), AllowBlend);
            Generator2.GenerateAttr(0);
            Manager.UseParticle(Generator2.GenC0(), Generator2.GenV0(), Time, Generator2.GenC0(), Generator2.GenMass(), Generator2.GetCoefff(), Generator2.GetCoeffr(), AllowBlend);
        }
    }

    // advance the real timestep
    Time += TimeStep;
    NTimeSteps++;

    ///////////////////////////////////////////////////////////////////////

    // draw only if we are at a display time
    if(NTimeSteps % TimeStepsPerDisplay == 0)
    DrawScene(0);

    // set up time for next timestep if in continuous mode
    glutIdleFunc(NULL);
    if(Manager.IsStep())
        Manager.SetStopped(true);
    else{
        Manager.SetStopped(false);
        glutTimerFunc(TimerDelay, TimerCallback, 0);
    }

}

//
//  Run a single time step in the simulation
//
void TimerCallback(int){
  Simulate();
}

/******************** LOAD PARAMETERS / RESET ****************************/
//
//  Load parameter file and reinitialize global parameters
//
void LoadParameters(char *filename){

    FILE *paramfile;

    double numofparticles, bspeed, speedstd, bmass, bstd, colstd, coeffr, coefff, genr, psize, blendsize;
    Vector3d  bcenter, bvelocity;
    Vector4d bcolor;

    if((paramfile = fopen(filename, "r")) == NULL){
        fprintf(stderr, "error opening parameter file %s\n", filename);
        exit(1);
    }

    ParamFilename = filename;

    if(fscanf(paramfile, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
    &TimeStep, &DispTime, &numofparticles, &psize, &blendsize,
    &bspeed, &speedstd,
    &bmass, &bstd,
    &(bcolor.x), &(bcolor.y), &(bcolor.z), &(bcolor.w), &colstd,
    &coeffr, &coefff,
    &(bcenter.x), &(bcenter.y), &(bcenter.z), &genr,
    &(bvelocity.x), &(bvelocity.y), &(bvelocity.z),
    &(env.Wind.x), &(env.Wind.y), &(env.Wind.z),
    &(env.G.x), &(env.G.y), &(env.G.z),
    &env.Viscosity) != 30){
        fprintf(stderr, "error reading parameter file %s\n", filename);
        fclose(paramfile);
        exit(1);
    }

    Manager.SetMaxPart((int)psize, (int)blendsize);

    Generator1.SetBaseAttr(2, bspeed, speedstd, bmass, bstd, bcolor, colstd, numofparticles, coefff, coeffr);
    Generator1.SetCenterRadius(bcenter, genr);
    Generator1.SetVelocity(bvelocity);
    Generator1.SetModel();

    Generator2.SetBaseAttr(2, bspeed, speedstd, bmass, bstd, bcolor, colstd, numofparticles, coefff, coeffr);
    Generator2.SetCenterRadius(bcenter, genr);
    Generator2.SetVelocity(bvelocity);
    Generator2.SetModel();

    TimeStepsPerDisplay = Max(1, int(DispTime / TimeStep + 0.5));
    TimerDelay = int(0.5 * TimeStep * 1000);
}

//
// Routine to restart the ball at the top
//
void RestartSim(){

  LoadParameters(ParamFilename); // reload parameters in case changed

  NTimeSteps = -1;
  glutIdleFunc(NULL);
  Time = 0;

  DrawScene(0);
}


/************************* INITIALIZATIONS ****************************/
//
//  Initialize the Simulation
//
void InitSimulation(int argc, char* argv[]){

  if(argc != 2){
    fprintf(stderr, "usage: particles paramfile\n");
    exit(1);
  }

  LoadParameters(argv[1]);

  Pl.BuildPlane(Vector3d(-20, -10, 0), Vector3d(20, -10, 0), Vector3d(20, 20, -50), Vector3d(-20, 20, -50));

  pa1.center.set(0, -15, 0);
  pa1.g.set(-5,-5,-5);
  pa1.r = 15;

  NSteps = 0;
  NTimeSteps = -1;
  Time = 0;
}

//
// Initialize Camera
//
void InitCamera() {
  Projection = PERSPECTIVE;

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  MenuAttached = false;

  Pan = 0;
  Tilt = 0;
  Approach = DEPTH;

  ThetaX = 0;
  ThetaY = 0;
}

/**************** ACTUAL (RE)DRAW FUNCTIONS ***********************/
//
//  On Redraw request, erase the window and redraw everything
//
void drawDisplay(){
  // distant light source, parallel rays coming from front upper right
  const float light_position1[] = {0, 1, 0, 0};

  // clear the window to the background color
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);  // solid - clear depth buffer
  // establish shading model, flat or smooth
  glShadeModel(GL_SMOOTH);

  // light is positioned in camera space so it does not move with object
  glLoadIdentity();
  glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
  glLightfv(GL_LIGHT0, GL_AMBIENT, WHITE);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
  glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // establish camera coordinates
  glRotatef(Tilt, 1, 0, 0);	    // tilt - rotate camera about x axis
  glRotatef(Pan, 0, 1, 0);	    // pan - rotate camera about y axis
  glTranslatef(0, 0, Approach);     // approach - translate camera along z axis

  // rotate the model
  glRotatef(ThetaY, 0, 1, 0);       // rotate model about x axis
  glRotatef(ThetaX, 1, 0, 0);       // rotate model about y axis

  DrawScene(0);

  glutSwapBuffers();
}

//
// Set up the projection matrix to be either orthographic or perspective
//
void updateProjection(){

  // initialize the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // determine the projection system and drawing coordinates
  if(Projection == ORTHO)
    glOrtho(-DRAWWIDTH/2, DRAWWIDTH/2, -DRAWHEIGHT/2, DRAWHEIGHT/2, NEAR, FAR);
  else{
    // scale drawing coords so center of cube is same size as in ortho
    // if it is at its nominal location
    double scale = fabs((double)NEAR / (double)DEPTH);
    double xmax = scale * DRAWWIDTH / 2;
    double ymax = scale * DRAWHEIGHT / 2;
    glFrustum(-xmax, xmax, -ymax, ymax, NEAR, FAR);
  }

  // restore modelview matrix as the one being updated
  glMatrixMode(GL_MODELVIEW);
}

/****************** MENU & CONTROL FUNCTIONS *********************/
//
//  On Reshape request, reshape viewing coordinates to keep the viewport set
//  to the original window proportions and to keep the window coordinates fixed
//
void doReshape(int w, int h){

  glViewport(0, 0, w, h);
  WinWidth = w;
  WinHeight = h;

  updateProjection();
}

//
//  Adjust mouse coordinates to match window coordinates
//
void AdjustMouse(int& x, int& y){

  /* reverse y, so zero at bottom, and max at top */
  y = int(WinHeight - y);

  /* rescale x, y to match current window size (may have been rescaled) */
  y = int(y * WINDOW_HEIGHT / WinHeight);
  x = int(x * WINDOW_WIDTH / WinWidth);
}

//
//  Watch mouse motion
//
void handleMotion(int x, int y){
  if(!MenuAttached) {
    int delta;

    y = -y;
    int dy = y - MouseY;
    int dx = x - MouseX;

    switch(Button){
      case GLUT_LEFT_BUTTON:
        ThetaX -= ROTFACTOR * dy;
        ThetaY += ROTFACTOR * dx;
        glutPostRedisplay();
        break;
      case GLUT_MIDDLE_BUTTON:
        Pan -= ROTFACTOR * dx;
        Tilt += ROTFACTOR * dy;
        glutPostRedisplay();
        break;
      case GLUT_RIGHT_BUTTON:
        delta = (fabs(dx) > fabs(dy)? dx: dy);
        Approach += XLATEFACTOR * delta;
        glutPostRedisplay();
        break;
    }

    MouseX = x;
    MouseY = y;
  }
}

//
//  Watch mouse button presses and handle them
//
void handleButton(int button, int state, int x, int y){

  if(MenuAttached) {

    if(button == GLUT_MIDDLE_BUTTON)
      MiddleButton = (state == GLUT_DOWN);

    if(button != GLUT_LEFT_BUTTON)
      return;

    AdjustMouse(x, y);	/* adjust mouse coords to current window size */

    if(state == GLUT_UP){
      if(Manager.IsStarted()){
        Manager.SetStarted(false);
        Manager.SetStopped(false);
        // need to re-initialize...should move to key press?
        DrawScene(0);
        glutIdleFunc(Simulate);
      }
      else if(Manager.IsStopped()){
        Manager.SetStopped(false);
        glutIdleFunc(Simulate);
      }
      else{
        Manager.SetStopped(true);
        glutIdleFunc(NULL);
      }
    }
  } else {

    if(state == GLUT_UP)
      Button = NONE;		// no button pressed
    else{
      MouseY = -y;			// invert y window coordinate to correspond with OpenGL
      MouseX = x;

      Button = button;		// store which button pressed
    }

  }
}

//
//  Menu callback
//
void HandleMenu(int index){

  switch(index){

  case MenuContinuous:
    if(Manager.IsStep()){
      Manager.SetStep(false);
      glutChangeToMenuEntry(index, "Step", index);
    }
    else{
      Manager.SetStep(true);
      glutChangeToMenuEntry(index, "Continuous", index);
    }
    break;

  case MenuQuit:
    exit(0);
  }
}

//
//  Set up pop-up menu on right mouse button
//
void MakeMenu(){
  int id = glutCreateMenu(HandleMenu);

  glutAddMenuEntry("Step", MenuContinuous);
  glutAddMenuEntry("Quit", MenuQuit);

  glutSetMenu(id);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//
// Keypress handling
//
void handleKey(unsigned char key, int x, int y){

  switch(key){
    case 'q':		// q - quit
    case 'Q':
    case 27:		// esc - quit
      exit(0);

    case 'p':			// P -- toggle between ortho and perspective
    case 'P':
      Projection = !Projection;
      updateProjection();
      glutPostRedisplay();
      break;

    case 'm':
    case 'M':
      MenuAttached = !MenuAttached;
      if(MenuAttached) { MakeMenu(); }
      else glutDetachMenu(GLUT_RIGHT_BUTTON);
      break;

    case 'b':
    case 'B':
      AllowBlend = !AllowBlend;
      Manager.EnableBlend(AllowBlend);
      break;

    case 't':
    case 'T':
      Toggle = !Toggle;
      break;

    case 'r':
    case 'R':
        Manager.KillAll();
        RestartSim();
      break;
    default:		// not a valid key -- just ignore it
      return;

  }

  glutPostRedisplay();
}


/********************* MAIN FUNCTION ***********************/
//
// Main program to set up display
//
int main(int argc, char* argv[]){

  glutInit(&argc, argv);

  InitSimulation(argc, argv);
  InitCamera();

  /* open window and establish coordinate system on it */
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Particle Simulation");

  /* register display and mouse-button callback routines */
  glutReshapeFunc(doReshape);
  glutDisplayFunc(drawDisplay);
  glutMouseFunc(handleButton);
  glutMotionFunc(handleMotion);
  glutKeyboardFunc(handleKey);

  /* Set up to clear screen to black */
  glClearColor(BG[0], BG[1], BG[2], 0);


  glutMainLoop();
  return 0;
}
