#ifndef MACOS
#include <windows.h>
#pragma comment(lib, "user32.lib") 
#endif

#include <GL/glut.h>  // (or others, depending on the system in use)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

#include "Color.h"
#include "Vector.h"
using namespace mathtool;
#include "Simulator.h"
#include "Environment.h"

double theta = 0;
double speedTheta = 0.1;

bool doSkeletalAnimation = false;// true;
int cameraMode = 0; //default = Projection (value 0), other value = Orthographic (value 1)
int selectedAgent = 0;
Vector3d lastNonZeroVel(1,0,0);
bool editEnvironment = true;


int numDraws=0;
int windowW = 800;
int windowH = 800;
static int window_id;
static int menu_id;

vector<MyColor> colors;
//Color bColor(0.2,0.2,0.2);
MyColor bColor(1,1,1);
bool isConvex=true;

Simulator gSim(false);
Simulator gSim2(true);
Environment* gEnv;
Environment* gEnv2;
bool isSimulating=false;
bool addAttractionPt = false;
string fileToLoad = "";
bool fileSpecified = false;

bool addIndividual = false;

int initial_time = time(NULL), final_time, frame_count = 0;

void initView();

void changeSize(int w, int h) {

  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
	/*
  if (h == 0)
    h = 1;
  float ratio =  w * 1.0 / h;

  // Use the Projection Matrix
  glMatrixMode(GL_PROJECTION);

  // Reset Matrix
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);

  // Set the correct perspective.
  gluPerspective(45.0f, ratio, 0.1f, 10000.0f);

  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);
  */
	initView();
}
void initView() {
	int w = windowW;
	int h = windowH;
	cout << "initView cameraMode=" << cameraMode << endl;
	if (cameraMode == 0 || cameraMode == 1) {
		// Prevent a divide by zero, when window is too short
		// (you cant make a window of zero width).
		if (h == 0)
			h = 1;
		float ratio = w * 1.0 / h;

		// Use the Projection Matrix
		glMatrixMode(GL_PROJECTION);

		// Reset Matrix
		glLoadIdentity();

		// Set the viewport to be the entire window
		glViewport(0, 0, w, h);

		// Set the correct perspective.
		//gluPerspective(45.0f, ratio, 0.1f, 10000.0f);
		gluPerspective(90.0f, ratio, 0.1f, 10000.0f);

		// Get Back to the Modelview
		glMatrixMode(GL_MODELVIEW);
	}
	else {
		/*
		glMatrixMode(GL_PROJECTION);      //Set projection parameters.
		glLoadIdentity();
		//gluOrtho2D(0.0, 200.0, 0.0, 150.0);
		//gluOrtho2D(0.0, windowW, 0.0, windowH);
		//gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
		glOrtho(-w/2, w/2, -h/2, h/2, 1.0, 3000.0);
		// Get Back to the Modelview
		glMatrixMode(GL_MODELVIEW);
		*/
	}

	GLfloat Diffuse[] =  { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat WhiteLight[] =  { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat light_position0[] = { 100.0, 100.0, 100.0, 0.0 };
	GLfloat light_position1[] = { -100.0, 100.0, 100.0, 0.0 };

	//glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT0,GL_DIFFUSE,WhiteLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,WhiteLight);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

void initSim() {
        gEnv = new Environment("env.txt",-windowW/2,windowW/2, -windowH/2, windowH/2);
		gEnv2 = new Environment("env2.txt", -windowW / 2, windowW / 2, -windowH / 2, windowH / 2);

}

void init() {
	//glClearColor(1.0, 1.0, 1.0, 0.0); //Set display-window color to white.
	glClearColor(bColor.r, bColor.g, bColor.b, 0.0); //Set display-window color to white.

	glMatrixMode(GL_PROJECTION);      //Set projection parameters.
	//gluOrtho2D(0.0, 200.0, 0.0, 150.0);
	//gluOrtho2D(0.0, windowW, 0.0, windowH);
	gluOrtho2D(-windowW/2,windowW/2, -windowH/2, windowH/2);
        gEnv = new Environment("env.txt",-windowW/2,windowW/2, -windowH/2, windowH/2);
		gEnv2 = new Environment("env2.txt", -windowW / 2, windowW / 2, -windowH / 2, windowH / 2);
		


}




void menu(int num) {
	if(num==0) {
		glutDestroyWindow(window_id);
		exit(0);
	}
	else if (num == 1) {
		//set perspective view
		cameraMode = 0;
		initView();
	}
	else if (num == 2) {
		//set perspective view
		cameraMode = 1;
		initView();
	}
	else if (num == 5) {
		//set follow mode 
		cameraMode = 2;
		initView();
	}
	else if (num == 3) {
		doSkeletalAnimation = !doSkeletalAnimation;
	}
	else if (num == 4) {
	  editEnvironment = !editEnvironment;
	}

	else if (num == 6) {
		fileToLoad = "user.txt";
		fileSpecified = !fileSpecified;
		gSim.setTak(fileSpecified);
	}

	else if (num == 7) {
		addAttractionPt = !addAttractionPt;
	}

	else if (num == 8) {
		addIndividual = !addIndividual;
	}

}

void createMenu() {
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Quit",0);
	glutAddMenuEntry("Perspective View", 1);
	glutAddMenuEntry("Top View", 2);
        glutAddMenuEntry("FollowMode",5);	
	glutAddMenuEntry("Toggle Skeletal Animation",3);
	glutAddMenuEntry("Toggle Environment",4);
	glutAddMenuEntry("Switch to User Define File", 6);
	glutAddMenuEntry("Adding or Removing Cell", 7);
	glutAddMenuEntry("Add Agent", 8);
	

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int homeX, homeY;
bool mousePressed=false;
int timeSincePress=0;
int changeY=0;
int changeX=0;
double alpha = 0.5;

void customDraw();
void update() {
	if (isSimulating) {
		if (gSim.getTak() == false)
			gSim.Update();
		else
			gSim2.Update();
	}

  //cout << "update mousePressed=" << mousePressed << endl;
  if( mousePressed ) {
    speedTheta = changeY*0.2;
    timeSincePress += 1;
  }
  customDraw();
}
void customDraw() {
	if (0 && ++numDraws % 100 == 0) {
		cout << " customDraw numDraws = " << numDraws << " doSkel: " << doSkeletalAnimation << endl;
	}
	//////////////////////////////////////////////////////////////
	//perspective drawing
	//
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bColor.r, bColor.g, bColor.b, 1.0); //Set display-window color to white.
	//glClear(GL_COLOR_BUFFER_BIT);     //Clear display window.
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (cameraMode == 0) {
		// Reset transformations
		glLoadIdentity();
		// Set the camera
		float x = windowW / 2.0;
		float y = 80;
		float z = windowH / 2.0;
		gluLookAt(x, y, z,
			0, 0, 0,
			0.0f, 1.0f, 0.0f);
	}
	else if (cameraMode == 1) {
		// Reset transformations
		glLoadIdentity();
		// Set the camera
		float y = windowW / 2.0;
		gluLookAt(0, y, 0,
			0, 0, 0,
			0.0f, 0.0f, 1.0f);
	}
	else {
		// Reset transformations
		glLoadIdentity();
		// Set the camera

		if (gSim.getTak() == false) {
			Vector3d pos = gSim.GetAgent(selectedAgent).GetPos();
			Vector3d vel = gSim.GetAgent(selectedAgent).GetVel();
			if (vel.norm() > 0.001) lastNonZeroVel = vel;
			lastNonZeroVel.selfNormalize();
			float y = windowW / 2.0;
			gluLookAt(pos.GetX() - 10 * lastNonZeroVel.GetX(), 30, pos.GetY() - 10 * lastNonZeroVel.GetX(),
				pos.GetX() + 10 * lastNonZeroVel.GetX(), 0, pos.GetY() + 10 * lastNonZeroVel.GetY(),
				0.0f, 1.0f, 0.0f);
		}
		else {
			Vector3d pos = gSim2.GetAgent(selectedAgent).GetPos();
			Vector3d vel = gSim2.GetAgent(selectedAgent).GetVel();
			if (vel.norm() > 0.001) lastNonZeroVel = vel;
			lastNonZeroVel.selfNormalize();
			float y = windowW / 2.0;
			gluLookAt(pos.GetX() - 10 * lastNonZeroVel.GetX(), 30, pos.GetY() - 10 * lastNonZeroVel.GetX(),
				pos.GetX() + 10 * lastNonZeroVel.GetX(), 0, pos.GetY() + 10 * lastNonZeroVel.GetY(),
				0.0f, 1.0f, 0.0f);
		}
	}
	//done test code-perspective drawing
	//////////////////////////////////////////////////////////////


	if (editEnvironment == true)
		gEnv->Draw();
	else
		gEnv2->Draw();

	if (gSim.getTak() == false)
		gSim.Draw();
	else
		gSim2.Draw();


	//glFlush();                        //Process all OpenGL routines as quickly as possible.
	glutSwapBuffers();

	frame_count++;
	final_time = time(NULL);
	if (final_time - initial_time > 0)
	{
		std::cout << "FPS: " << frame_count / (final_time - initial_time) << std::endl;
		frame_count = 0;
		initial_time = final_time;
	}
}

void mousebutton(int button, int state, int x, int y)
{
  if( cameraMode == 1 /*top*/ && editEnvironment && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
    cout << "Toggle environment at x="<<x<< " y=" << y << endl;
    double tx = -1.0*x + windowW/2.0;
    double tz = 1.0*windowH/2.0-y;

	if (addAttractionPt) {
		if (gEnv != NULL) {
			gEnv->ToggleBlockedCell(tx, tz);
		}

		else if (gEnv2 != NULL) {
			gEnv2->ToggleBlockedCell(tx, tz);
		}
	}
  }
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		if( !mousePressed ) {
			homeX = x;
			homeY = y;
			timeSincePress = 0;
		}
		mousePressed = true;

		double tx = -1.0 * x + windowW / 2.0;
		double tz = 1.0 * windowH / 2.0 - y;


		if (addIndividual)
			gSim.AddMember(-1, tx, tz);

		glutPostRedisplay();
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		cout << " release button " << endl;
		theta = theta+timeSincePress*speedTheta;
		speedTheta = 1;
		changeY = 0;
		//alpha = 1;
		mousePressed = false;
		timeSincePress = 0;
 	}
	if( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN ) {
	  cout << "glut_middle_button == down" << endl;
	}
}
void setBackgroundColor(int colorIndex, double colorDif, double colorRange) {
	cout << "setBackgroundColor colorIndex = " << colorIndex
	     << " colorDif = " << colorDif
	     << " colorRange = " << colorRange << endl;
	if( colorIndex>=colors.size() ) colorIndex = colors.size()-1;
	MyColor& c_i = colors[colorIndex];
	int j = colorIndex + 1;
	if( j>= colors.size() ) j = colors.size()-1;
	MyColor& c_j = colors[j];

	c_i.Print();
	c_j.Print();
	double s = colorDif / colorRange;	
	double tr = (1-s)*c_i.r + s*c_j.r;
	double tg = (1-s)*c_i.g + s*c_j.g;
	double tb = (1-s)*c_i.b + s*c_j.b;

	bColor.r = tr;	
	bColor.g = tg;	
	bColor.b = tb;	
	
	cout << "Setting color (" << tr << "," << tg << "," << tb << ")" << endl;
	
}
/*
void mouseMove(int mx, int my) {
	cout << "mouseMove mx: " << mx << " my: " << my << endl;
	//int changeY = my - homeY;
	changeY = my - homeY;
	//speedTheta *=1.05;
	//theta += changeY*speedTheta;
	cout << "theta " << theta << " speedTheta " << speedTheta << " changeY " << changeY << endl;
	changeX = mx - homeX;
	alpha = 0.5+ (1.0*changeX)/windowW;
	double ratioInScreen = double(mx)/windowW;
	if(ratioInScreen<0) ratioInScreen=0; //bound the allowable values
	if(ratioInScreen>1) ratioInScreen=1;
	int colorIndex = int(ratioInScreen*colors.size());
	double colorRange = 1.0/colors.size();
	double colorDif = ((ratioInScreen*colors.size())-colorIndex)/colors.size();
	cout << "alpha " << alpha << " mx " << mx << " homeX " << homeX << endl;
	cout <<" ratioInScreen " << ratioInScreen << " colorIndex " << colorIndex << " colorRange " << colorRange << " colorDif " << colorDif << endl;
 	setBackgroundColor(colorIndex, colorDif, colorRange);	

}
*/

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'q':
  case 27:             // ESCAPE key
	  exit (0);
	  break;
  case 't': //birdview
	  cameraMode = 1; break;
  case 'p': //orgin view
	  cameraMode = 0; break;
  case 'o': //follow
	  cameraMode = 2; break;
  case ' ':
	isSimulating = !isSimulating; break;
  case 's': //switch
	doSkeletalAnimation = !doSkeletalAnimation; break;
  case 'f': //frames
	  gSim.Update();
	  isSimulating = false;
	  break;
  case '0': //change agent
  	selectedAgent++;
        int numAgents = gSim.GetNumAgents();
	if(selectedAgent>=numAgents) selectedAgent=0;
	break;
  }
}

void otherKeyInput(int key, int x, int y) {
	/*
  switch(key) {
    case GLUT_KEY_UP:
      //do something here
      cout << "GLUT_KEY_UP" << endl;
      translation.y += 2;
      break;	
    case GLUT_KEY_DOWN:
      //do something here
      cout << "GLUT_KEY_DOWN" << endl;
      translation.y -= 2;
      break;
    case GLUT_KEY_LEFT:
      //do something here
      cout << "GLUT_KEY_LEFT" << endl;
      translation.x += 2;
      break;
    case GLUT_KEY_RIGHT:
      //do something here
      cout << "GLUT_KEY_RIGHT" << endl;
      translation.x -= 2;
      break;
  }
  */
  glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);                         //Initialize GLUT.
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);   //Set display mode.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   //Set display mode.
	//glutInitWindowPosition(50, 100);               //Set top-left display-window position.
	glutInitWindowSize(windowW, windowH);                  //Set display-window width and height.
	window_id = glutCreateWindow("Crowd Sim!!!");  //Create display window.

	initSim();                                        //Execute initialization procedure.

	if (fileSpecified == true) {
		createMenu();
		glutIdleFunc(update);
		glutDisplayFunc(customDraw);                   //Send graphics to display window.
		glutReshapeFunc(changeSize);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mousebutton);                    //How to handle mouse press events. 
		//glutMotionFunc(mouseMove);
		glutSpecialFunc(otherKeyInput);
		glutMainLoop();                                //Display everything and wait.

	}
	else {

		createMenu();
		glutIdleFunc(update);
		glutDisplayFunc(customDraw);                   //Send graphics to display window.
		glutReshapeFunc(changeSize);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mousebutton);                    //How to handle mouse press events. 
		//glutMotionFunc(mouseMove);
		glutSpecialFunc(otherKeyInput);
		glutMainLoop();                                //Display everything and wait.
	}
}

