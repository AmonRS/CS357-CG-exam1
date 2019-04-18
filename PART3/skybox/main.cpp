#include <iostream>
#include <cmath>
#include <iomanip>
#include <cassert>
#include <vector>
using namespace std;
#include "Angel.h";
#include <GL/glew.h> // for OpenGL extensions
#include <GL/glut.h> // for Glut utility kit
#include "texture.h" // for the bitmap texture loader
// Global Projection Matrices
mat4 projection, modelview, translate;  
#include "GraphicsObject.h"
#include "SkyBox.h"
#include "Maze.h"
#include "Brick.h"
#include "Tree.h"



GLfloat  zoom = 0.0;         // Translation factor
vec4 view(0.0, 0.0, -2.0, 0.0);

GLfloat  fovy = 60.0;		 // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect = 1.0;       // Viewport aspect ratio
GLfloat  zNear = 0.01, zFar = 1000.0;

GLfloat dir = 1.0;
GLfloat theta[] = {0.0,0.0,0.0};
GLint axis = 1;

// THE global SkyBox Object
SkyBox go_skybox;
SkyBox go_skybox_2;
//maze
Maze go_maze;
// The objects
Brick go_brick_1;
point4  eye(90.0, 0.0, 104.0, 1.0);
point4  at(0.0, 0.0, -1.0, 1.0);
vec4    up(0.0, 1.0, 0.0, 0.0);





void display( void )
{
	static float angle = 0.0;

	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  /*clear the window */

	projection = Perspective(fovy, aspect, zNear, zFar);
	modelview = Translate(0.0, 0.0, 1.0)*LookAt(eye, at, up);

	// tell the skybox to draw its vertex
	go_skybox.draw( theta );

	// draw the maze
	go_maze.draw( theta );

	// tell the bricks to draw themselves and rotate too!
	go_brick_1.draw( theta, vec3(-4.0,0.0,0.0) );

	angle += 0.5;
	if( angle > 360.0 ) angle -= 360.0;


	// swap the buffers
	glutSwapBuffers();

	glutPostRedisplay();
}

//void spinCube(int key, int x, int y)
//{	
	//if ( axis > -1 )
	//{
	//	//if (key==)
	//	theta[axis] += 0.5 * dir;
	//	if( theta[axis] > 360.0 ) theta[axis] -= 360.0;
	//	glutPostRedisplay();
	//}
//}





/*******************************************************************************************/
/* LOOK / MOVE */
/*******************************************************************************************/

void arrowKey(int key, int x, int y) {
	// look around with arrow keys

	GLfloat prev_aty;

	if (key == GLUT_KEY_UP){ //look up--doesn't allow you to look past straight up/down (no sommersaults)
		at.y += .5;
		if (at.y > eye.y +5) at.y = eye.y +5;
		///cout << "at.y: " << at.y << endl;
	}
	if (key == GLUT_KEY_DOWN){ //look down
		at.y -= .5;
		if (at.y < eye.y -5) at.y = eye.y -5;
		///cout << "at.y: " << at.y << endl;
	}
	if (key == GLUT_KEY_RIGHT){ //turn right
		prev_aty = at.y;
		view = RotateY(-5) * view;//rotate eye -5 degrees
		at = eye + view;
		at.y = prev_aty;
	}
	if (key == GLUT_KEY_LEFT){ //turn left
		prev_aty = at.y;
		view = RotateY(5) * view;//rotate eye 5 degrees
		at = eye + view;
		at.y = prev_aty;
	}

	///cout << "view: " << view << " - eye: " << eye << " - at: " << at << endl;
	glutPostRedisplay();
}

GLfloat prev_x, prev_y;
GLint firsttime = 1;
void mouse_look(int x1, int y1) {
	// look around with mouse (passive move)

	GLfloat mousespeed = 0.1;
	GLfloat x = (float)x1; GLfloat y = (float)y1;

	if (firsttime == 1){
		///cout << "mouse_look first time" << endl;
		prev_x = x;
		prev_y = y;
		firsttime = 0;
		return;
	}

	// get change in mouse movement
	GLfloat gww = glutGet(GLUT_WINDOW_WIDTH); GLfloat gwh = glutGet(GLUT_WINDOW_HEIGHT);
	GLfloat dx = (prev_x - x) * mousespeed;
	GLfloat dy = (prev_y - y) * mousespeed;
	///cout << "x: " << x << " - y: " << y << " - dx: " << dx << " - dy: " << dy << endl;

	// rotate view horizontally
	GLfloat prev_aty = at.y;
	view = RotateY(dx*5) * view;
	at = eye + view;
	at.y = prev_aty;

	// rotate view vertically
	at.y = at.y + (dy * mousespeed);
	if (at.y > eye.y +5) at.y = eye.y +5;
	if (at.y < eye.y -5) at.y = eye.y -5;
	///cout << at.y << endl;


	glutWarpPointer(gww / 2, gwh / 2);
	prev_x = gww/2;
	prev_y = gwh/2;

	///cout << "view: " << view << " - eye: " << eye << " - at: " << at << endl;
	glutPostRedisplay();
}

bool collision_detection(point4 eye1) {
	// collision detection
	// collision detection by invisible sphere around object (brick)
	
	if (go_brick_1.collision_detection(eye1) == true){
		return true;
	}
	if (go_maze.collision_detection(eye1) == true) {
		return true;
	}

	return false;
}

void key(unsigned char key, int x, int y) {
	// move around with  WSADEQ
	// if no collision if moved, then move

	if (key == 'w'){   //move forward (zoom)
		if (!collision_detection(eye + 0.25*view)){
			eye = eye + 0.25*view;
			at = at + 0.25*view;
		}
	}
	if (key == 's'){ //move backward (zoom)
		if (!collision_detection(eye - 0.25*view)){
			eye = eye - 0.25*view;
			at = at - 0.25*view;
		}
	}
	if (key == 'a'){ //move left
		if (!collision_detection(eye - 0.25*cross(view, up))){
			eye = eye - 0.25*cross(view, up);
			at = at - 0.25*cross(view, up);
		}
	}
	if (key == 'd'){ //move right
		if (!collision_detection(eye + 0.25*cross(view, up))){
			eye = eye + 0.25*cross(view, up);
			at = at + 0.25*cross(view, up);
		}
	}
	if (key == 'e') { //move up
		if (!collision_detection(eye + 0.25*up)) {
			eye = eye + 0.25*up;
			at = at + 0.25*up;
		}
	}
	if (key == 'q') { //move up
		if (!collision_detection(eye + 0.25*up)) {
			eye = eye - 0.25*up;
			at = at - 0.25*up;
		}
	}

    if(key == 'p') exit(0);
	glutPostRedisplay();
}

void mouse_move( int x, int y )
{
	zoom = ( 10.0 / 500.0 ) * y +2.0;  // compute zoom factor 
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)	axis = 1;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
	glutPostRedisplay();
}





void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	aspect = GLfloat(w) / h;
}

void init_gl()
{
	glEnable(GL_DEPTH_TEST);
}

void init()
{   
	init_gl();			    // Setup general OpenGL stuff of the object //could do all of this by creating  skybox.init function that does all 5 things

	go_skybox.init_data();	        // Setup the data for the this object
	go_skybox.init_VAO();           // Initialize the vertex array object for this object
	go_skybox.init_VBO();			// Initialize the data buffers for this object
	go_skybox.init_shader();		// Initialize the shader objects and textures for skybox
	go_skybox.init_texture_map();	// Initialize the texture map for this object
	
	go_brick_1.init_data();	        // Setup the data for the skybox object
	go_brick_1.init_VAO();          // Initialize the vertex array object for this object
	go_brick_1.init_VBO();			// Initialize the data buffers for this object
	go_brick_1.init_shader();		// Initialize the shader objects and textures for skybox
	go_brick_1.init_texture_map();	// Initialize the texture map for this object


	go_maze.init();					// set up the maze

	view = RotateY(15) * view;//rotate eye 30 degrees
	at = eye + view;

	GL_CHECK_ERRORS
}

void OnShutdown()
{
	go_skybox.cleanup(); // release the textures on the graphics card
	go_maze.cleanup();
	go_brick_1.cleanup();
}

void checkGlew()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		cerr<<"Error: " << glewGetErrorString(err)<<endl;
	} else {
		if (GLEW_VERSION_3_3)
		{
			cout<<"Driver supports OpenGL 3.3\nDetails:"<<endl;
		}
	}
	cout<<"Using GLEW "<<glewGetString(GLEW_VERSION)<<endl;
	cout<<"Vendor: "<<glGetString (GL_VENDOR)<<endl;
	cout<<"Renderer: "<<glGetString (GL_RENDERER)<<endl;
	cout<<"Version: "<<glGetString (GL_VERSION)<<endl;
	cout<<"GLSL: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<endl;
}

int main(int argc, char **argv)
{
	atexit(OnShutdown);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow( "SkyBox" );


	go_maze.generate_maze();


	checkGlew();
    init();
    glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutSpecialFunc(arrowKey);
	//glutIdleFunc(arrowKey);       // set in key press
	//glutMouseFunc(mouse);
	//glutMotionFunc(mouse_move);   // Called when mouse moves with a mouse button pressed
	glutPassiveMotionFunc(mouse_look); // mouse look
    glutKeyboardFunc(key);


	cout << "*****************************************************" << endl;
	cout << "*   w s a d e q		: move around" << endl;
	cout << "*   p			: exit" << endl;
	cout << "*   arrow keys		: look around" << endl;
	cout << "*   mouse cursor	: look around" << endl;
	cout << "*****************************************************" << endl;

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);

    glutMainLoop();

    return 0;
}
