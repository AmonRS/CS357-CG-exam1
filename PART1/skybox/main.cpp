
/////////////////////////////////////////////////////////
//NOTE:
//// move right and left using right and left arrow keys
//// look up and down using up and down arrow keys
//// move forward using 'w' key 
//// move backward using 's' key
/////////////////////////////////////////////////////////



////MAKE ZOOM IN AND OUT WORK --make sure I can't zoom in the up/down directions
//randomly call diff. scenes?
//change background (image is in folder under "mountains")

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
#include "Brick.h"
#include "tree.h"
#include "Zebra.h"


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

// The objects
Brick go_brick_1;
Brick go_brick_2;
tree go_tree_1;
Zebra go_zebra_2;
point4  eye(0.0, 0.0, 1.0, 1.0);
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

	// tell the bricks to draw themselves and rotate too!
	go_brick_1.draw( theta, vec3(0.0,0.0,0.0) );
   	go_brick_2.draw( theta, vec3(1.0,0.0,0.0) );
	go_zebra_2.draw( theta, vec3(1.0,0.0,0.0) );

	angle += 0.5;
	if( angle > 360.0 ) angle -= 360.0;

	go_tree_1.draw( vec3(0.0,angle,0.0), vec3(-1.0,0.0,0.0) );



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

void arrowKey(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) //look up--doesn't allow you to look past straight up/down (no sommersaults)
	{
		at.y += .5;
		cout << "at.y: " << at.y << endl;
	}
	if (key == GLUT_KEY_DOWN) //look down
	{
		at.y -= .5;
		cout << "at.y: " << at.y << endl;
	}
	if (key == GLUT_KEY_RIGHT) //turn right
	{
		view = RotateY(-5) * view;//rotate eye -5 degrees
		at = eye + view;
	}
	if (key == GLUT_KEY_LEFT) //turn left
	{
		view = RotateY(5) * view;//rotate eye 5 degrees
		at = eye + view;
	}
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{	
	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)	axis = 1;
	if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 0;
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
	glutPostRedisplay();
}

//void mouse_move( int x, int y )
//{
//	zoom = ( 10.0 / 500.0 ) * y +2.0;  // compute zoom factor 
//}

GLfloat prev_x, prev_y, x, y;
GLint firsttime = 1;
GLfloat mousespeed = 0.1;
void mouse_look(int x, int y) {
	if (firsttime == 1){
		cout << "mouse_look first time" << endl;
		prev_x = x;
		prev_y = y;
		firsttime = 0;
		return;
	}

	GLfloat dx = (prev_x - x) * mousespeed;
	GLfloat dy = (prev_y - y) * mousespeed;
	cout << "x: " << x << " - y: " << y << " - dx: " << dx << " - dy: " << dy << endl;

	at.x = at.x + (sinf(dx) * cosf(dy));
	at.y = at.y + sinf(dy);
	at.z = at.z + (cosf(dx) * cosf(dy));

	prev_x = x;
	prev_y = y;
	//glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

	glutPostRedisplay();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
	aspect =  GLfloat (w) / h;
}

void key(unsigned char key, int x, int y) //MAKE ZOOM IN AND OUT WORK
{
	if (key == 'w')//move forward (zoom)
	{
		eye = eye + 0.25*view;
		at = at + 0.25*view;
	}

	if (key == 's')//move backward (zoom)
	{
		eye = eye - 0.25*view;
		at = at - 0.25*view;
	}

    if(key == 'q') exit(0);
	glutPostRedisplay();
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

	go_brick_2.init_data();	        // Setup the data for the skybox object
	go_brick_2.init_VAO();          // Initialize the vertex array object for this object
	go_brick_2.init_VBO();			// Initialize the data buffers for this object
	go_brick_2.init_shader();		// Initialize the shader objects and textures for skybox
	go_brick_2.init_texture_map();	// Initialize the texture map for this object
	
	go_tree_1.init_data();	    // Setup the data for the skybox object
	go_tree_1.init_VAO();        // Initialize the vertex array object for this object
	go_tree_1.init_VBO();		// Initialize the data buffers for this object
	go_tree_1.init_shader();		// Initialize the shader objects and textures for skybox
	go_tree_1.init_texture_map();// Initialize the texture map for this object

	go_zebra_2.init_data();	        // Setup the data for the skybox object
	go_zebra_2.init_VAO();          // Initialize the vertex array object for this object
	go_zebra_2.init_VBO();			// Initialize the data buffers for this object
	go_zebra_2.init_shader();		// Initialize the shader objects and textures for skybox
	go_zebra_2.init_texture_map();	// Initialize the texture map for this object


	view = RotateY(15) * view;//rotate eye 30 degrees
	at = eye + view;

	GL_CHECK_ERRORS
}

void OnShutdown()
{
	go_skybox.cleanup(); // release the textures on the graphics card
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
    glutInitWindowSize(500, 500);
    glutCreateWindow( "SkyBox" );


	checkGlew();
    init();
    glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutSpecialFunc(arrowKey);
	//glutIdleFunc(arrowKey);       // set in key press
	//glutMouseFunc(mouse);
	///glutMotionFunc(mouse_move);   // Called when mouse moves with a mouse button pressed
	glutPassiveMotionFunc(mouse_look);         // mouse look
    glutKeyboardFunc(key);


	cout << "*****************************************************" << endl;
	cout << "*   w moves forward" << endl;
	cout << "*   s moves backward" << endl;
	cout << "*   left arrow key rotates view left" << endl;
	cout << "*   right arrow key rotates view right" << endl;
	cout << "*****************************************************" << endl;

    glutMainLoop();

    return 0;
}