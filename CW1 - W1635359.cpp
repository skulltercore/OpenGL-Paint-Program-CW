/*
 5CCGD003W - CW1 // Code by Joseph Calabro W1635359
*/

#include "include\freeglut.h"//OpenGL toolkit - in the local shared folder
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

//using namespace std;

//set up some constants
#define X_CENTRE 0.0      /* centre point of square */
#define Y_CENTRE 0.0
#define LENGTH   1.0      /* lengths of sides of square */

bool up, down, left, right;

GLfloat red = 0.0, green = 0.0, blue = 1.0, spdX = 0, mouseX, mouseY, DDmouseX, DDmouseY;

GLfloat ddx1_1, ddy1_1, ddx2_1, ddy2_1, ddx3_1, ddy3_1, ddx4_1, ddy4_1; //Drop Down Coordinates
GLfloat ddx1_2, ddy1_2, ddx2_2, ddy2_2, ddx3_2, ddy3_2, ddx4_2, ddy4_2;
GLfloat ddx1_3, ddy1_3, ddx2_3, ddy2_3, ddx3_3, ddy3_3, ddx4_3, ddy4_3;

std::string last; //used so I know what the last shape created was (so I know which one to move with the arrows)

GLfloat point1x = 2, point1y = 2, point2x = 2, point2y = 2; //for drawing a line

int STATE = 0, DDstate = 1 , window;

bool gridOn = true, DD = false, animation = false; //Toggle Grid//Drop Down//Animation 

std::vector< float > points;



/////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------> LINE CLASS 
/////////////////////////////////////////////////////////////////////////////////////////////////////

class Line
{
private:
	GLfloat x1, y1, x2, y2; 
	bool stopx = false, stopy = false;
	GLfloat r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //Random Colour
	GLfloat g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	GLfloat b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

public:
	Line(GLfloat xx1, GLfloat yy1, GLfloat xx2, GLfloat yy2)
	{
		x1 = xx1;
		y1 = yy1;

		x2 = xx2;
		y2 = yy2;
	}
	Line()
	{

	}

	void Move(GLfloat xin, GLfloat yin)
	{
		if ((x1 <= -0.7|| x2 <= -0.7) && xin < 0)
		{
			stopx = true;
		}
		else
		{
			stopx = false;
		}
		
		if ((y1 >= 0.7 || y2 >= 0.7) && yin > 0)
		{
			stopy = true;
		}
		else
		{
			stopy = false;
		}

		if (!stopx)
		{
			x1 += xin;
			x2 += xin;
		}

		if (!stopy )
		{
			y1 += yin;
			y2 += yin;
		}
		
	}

	void draw()
	{
		glColor3f(r, g, b);
		glLineWidth(0);

		glBegin(GL_LINE_LOOP);
		
		glVertex2f(x1, y1); //LINE
		glVertex2f(x2, y2);

		glEnd();
	}
};

std::vector <Line> Vlines(0);

/////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------> CIRCLE CLASS 
/////////////////////////////////////////////////////////////////////////////////////////////////////

class Circle
{
private:

	bool polygon, ran = false, rando = false;
	GLfloat rad;
	GLfloat r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //Random Colour
	GLfloat g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	GLfloat b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = 0.05 - 0.01;
	float ra = random * diff;

	float random2 = ((float)rand()) / (float)RAND_MAX; //calculate random speed and size for animation
	float diff2 = 0.00001 - 0.0000001;
	float ra2 = random * diff;

public:

	GLfloat centreX, centreY, spd = 0.0001;

	Circle(GLfloat cx, GLfloat cy, bool poly)
	{
		centreX = cx;
		centreY = cy;
		polygon = poly;
	}
	Circle(GLfloat cx, GLfloat cy, bool poly, bool random)
	{
		centreX = cx;
		centreY = cy;
		polygon = poly;
		ran = random;
	}
	Circle()
	{

	}

	void Move(GLfloat xin, GLfloat yin)
	{
		centreX += xin;
		centreY += yin;
	}

	void animate()
	{
		spd = 0.0000001 + ra2; //Move by speed
		centreY += ((((spd)/2)/2)/2)/2;
		rando = true;
	}

	void draw()
	{

		if (ran == false)
		{
			rad = 0.1;
		}
		else
		{
			rad =  0.01 + ra;
		}

		GLfloat thickness = 2;

		if (!rando)
		{
			if (centreX <= -0.7)//Collision
			{
				centreX = -0.699;
			}
			if (centreY >= 0.7)
			{
				centreY = 0.699;
			}
		}

		glColor3f(r, g, b);
		glLineWidth(thickness);

		if (polygon)
		{
			glBegin(GL_POLYGON);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		for (int i = 0; i < 360; i++)
		{
			GLfloat theta = 2.0f * 3.1415926f * GLfloat(i) / GLfloat(360);//get the current angle 
			GLfloat x = rad * cosf(theta);//calculate the x
			GLfloat y = rad * sinf(theta);//calculate the y 
			glVertex2f(x + centreX, y + centreY);//output vertex 
		}

		glEnd();
	}
};

std::vector <Circle> Vcircles(0);

std::vector <Circle> Vanimation;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------> TRIANGLE CLASS 
/////////////////////////////////////////////////////////////////////////////////////////////////////

class Triangle
{
private:
	GLfloat centreX, centreY;
	bool polygon;
	GLfloat r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //Random Colour
	GLfloat g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	GLfloat b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

public:
	Triangle(GLfloat cx, GLfloat cy, bool poly)
	{
		centreX = cx;
		centreY = cy;
		polygon = poly;
	}
	Triangle()
	{

	}

	void Move(GLfloat xin, GLfloat yin)
	{
		centreY += yin;
		centreX += xin;
	}

	void draw()
	{
		GLfloat x1, y1, x2, y2, x3, y3;
		GLfloat length = 0.2, thickness = 2;

		if (centreX <= -0.7)//Collision
		{
			centreX = -0.699;
		}
		if (centreY >= 0.7)
		{
			centreY = 0.699;
		}

		x1 = centreX + length / 2; // calculate vertex based on centre
		y1 = centreY - length / 2;

		x2 = centreX + length / 2; 
		y2 = centreY + length / 2;

		x3 = centreX - length / 2;
		y3 = centreY - length / 2;

		glColor3f(r, g, b);
		glLineWidth(thickness);


		if (polygon)
		{
			glBegin(GL_POLYGON);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		glVertex2f(x1, y1); //Triangle
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);

		glEnd();
	}
};

std::vector <Triangle> Vtriangles(0);

/////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------> SQUARE CLASS 
/////////////////////////////////////////////////////////////////////////////////////////////////////

class Square
{
private:
	GLfloat centreX, centreY, length;
	bool polygon;
	GLfloat r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //Random Colour
	GLfloat g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	GLfloat b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

public:
	Square(GLfloat cx, GLfloat cy, bool poly, GLfloat l)
	{
		centreX = cx;
		centreY = cy;
		polygon = poly;
		length = l;
	}
	Square()
	{

	}

	void Move(GLfloat xin, GLfloat yin)
	{
		centreY += yin;
		centreX += xin;
	}

	void draw()
	{
		GLfloat x1, y1, x2, y2, x3, y3, x4, y4;
		GLfloat thickness = 2;

		if (centreX <= -0.7) //Collision
		{
			centreX = -0.699;
		}
		if (centreY >= 0.7)
		{
			centreY = 0.699;
		}

		x1 = centreX - length / 2; // calculate vertex based on centre
		y1 = centreY + length / 2;

		x2 = centreX + length / 2;
		y2 = centreY + length / 2;

		x3 = centreX + length / 2;
		y3 = centreY - length / 2;

		x4 = centreX - length / 2;
		y4 = centreY - length / 2;

		glColor3f(r, g, b);
		glLineWidth(thickness);


		if (polygon)
		{
			glBegin(GL_POLYGON);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}
		
		glVertex2f(x1, y1); //Square
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
		glVertex2f(x4, y4);

		glEnd();
	}
};

std::vector <Square> Vsquares(0);
std::vector <Square> Vpoints(0);

/////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//-------------------ANIMATION---------------//-----------------------------------------------------------------> ANIMATION
//////////////////////////////////////////////

void anime()
{
	if(animation)
	{
		while(Vanimation.size() < 200)
		{
			float random = ((float)rand()) / (float)RAND_MAX;
			float diff = 1 - -1;
			float ra = random * diff;
			float randX = -1 + ra; //GET RANDOM X POSITION

			Circle c = Circle(randX, -1, true, true); //create circle at random X
			Vanimation.push_back(c); //add to vector

		}

		
		for (int i = 0; i < Vanimation.size(); i++) //DRAW CIRCLES
		{
			Vanimation[i].animate(); //call animate func
			Vanimation[i].draw(); //call draw func

			if (Vanimation[i].centreY > 0.9)
			{
				std::vector<Circle>::iterator it = Vanimation.begin(); //if at the top of the screen remove from vector
				Vanimation.erase(it + i);
			}
		}
		
	}
	else
	{
		Vanimation.clear(); //CLEAR VECTOR
	}
}

////////////////////////////////////////////////
//--------------TRUE MOUSE POS---------------//-----------------------------------------------------------------> TRUE MOUSE POS //Update Drop Down State
//////////////////////////////////////////////

void mousePos(int x, int y)
{

	mouseX = float(x / (float)400 - 1.0);
	mouseY = float(-(y / (float)400 - 1.0));

	if (DD) //Update Drop Down State
	{
		if (mouseX > ddx1_1 && mouseX < ddx2_1 && mouseY < ddy1_1 && mouseY > ddy3_1 && DDstate != 1) //Grid On
		{
			DDstate = 1;
			glutPostRedisplay(); //Refresh
		}
		else
		if (mouseX > ddx1_2 && mouseX < ddx2_2 && mouseY < ddy1_2 && mouseY > ddy3_2 && DDstate != 2) //Grid Off
		{
			DDstate = 2;
			glutPostRedisplay(); //Refresh
		}
		else
		if (mouseX > ddx1_3 && mouseX < ddx2_3 && mouseY < ddy1_3 && mouseY > ddy3_3 && DDstate != 3) //Animate
		{
			DDstate = 3;
			glutPostRedisplay(); //Refresh
		}
		else
		if ((mouseX < ddx1_1 || mouseX > ddx2_1 || mouseY > ddy1_1 || mouseY < ddy3_3) && (DDstate != 0)) //Outside Drop Down
		{
			DDstate = 0;
			glutPostRedisplay(); //Refresh
		}

		if (mouseX >= ddx1_1 - 0.05 && mouseX <= ddx2_1 + 0.05 && mouseY <= ddy1_1 + 0.05 && mouseY >= ddy3_3 - 0.05)
		{
			//Inside Drop Down
		}
		else
		{
			DD = false;
			glutPostRedisplay(); //Refresh
			DDstate = 1;
		}
	}
	else
	{
		DDstate = 1;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//---------Is Click Within The Box-----------//-----------------------------------------------------------------> IS CLICK WITHIN BOX
//////////////////////////////////////////////

bool boxClick(GLfloat length, GLfloat centreX, GLfloat centreY) //Check if mouse is within a defined square
{
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4;// calculate vertex based on centre

	x1 = centreX - length / 2;
	y1 = centreY + length / 2;

	x2 = centreX + length / 2;
	y2 = centreY + length / 2;

	x3 = centreX + length / 2;
	y3 = centreY - length / 2;

	x4 = centreX - length / 2;
	y4 = centreY - length / 2;

	if (mouseX > x1 && mouseX < x2 && mouseY < y1 && mouseY > y3) // is mouse inside square
	{
		return true;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//--------------CREATE NEW SHAPE-------------//-----------------------------------------------------------------> CREATE NEW SHAPE//DELETE
//////////////////////////////////////////////

void Create()
{
	//Create new shape and add to vector based on current state

	if (STATE == 1) //CIRCLE
	{
		Circle c = Circle(mouseX, mouseY, true, false);
		last = "circle";
		Vcircles.push_back(c);
	}

	if (STATE == 2) //SQUARE
	{
		Square s = Square(mouseX, mouseY, true, 0.2);
		last = "square";
		Vsquares.push_back(s);
	}

	if (STATE == 6) //SQUARE LINE
	{
		Square s = Square(mouseX, mouseY, false, 0.2);
		last = "square line";
		Vsquares.push_back(s);
	}

	if (STATE == 4) //POINTS
	{
		Square p = Square(mouseX, mouseY, true, 0.01);
		last = "point";
		Vpoints.push_back(p);
	}

	if (STATE == 3) //TRIANGLE
	{
		Triangle t = Triangle(mouseX, mouseY, true);
		last = "triangle";
		Vtriangles.push_back(t);
	}

	if (STATE == 7) //TRIANGLE LINE
	{
		Triangle t = Triangle(mouseX, mouseY, false);
		last = "triangle line";
		Vtriangles.push_back(t);
	}

	if (STATE == 5) //LINE
	{
		if (point1x == 2)
		{
			point1x = mouseX;
			point1y = mouseY;
		}
		else
		{
			point2x = mouseX;
			point2y = mouseY;

			Line l = Line(point1x, point1y, point2x, point2y);
			last = "line";
			Vlines.push_back(l);


			point1x = 2;
			point1y = 2;

			point2x = 2;
			point2y = 2;
		}
	}
	else
	{
		point1x = 2;
		point1y = 2;

		point2x = 2;
		point2y = 2;
	}

}

//Draw Shapes

void DrawNewShape()
{
	for (int i = 0; i < Vlines.size(); i++) //LINES
	{
		Vlines[i].draw();
	}

	for (int i = 0; i < Vsquares.size(); i++) //SQUARE
	{
		Vsquares[i].draw();
	}

	for (int i = 0; i < Vpoints.size(); i++) //POINTS
	{
		Vpoints[i].draw();
	}

	for (int i = 0; i < Vtriangles.size(); i++) //TRIANGLE
	{
		Vtriangles[i].draw();
	}

	for (int i = 0; i < Vcircles.size(); i++) //CIRCLE
	{
		Vcircles[i].draw();
	}
}

void SpecialInput(int key, int x, int y) //KEYBOARD INPUT MOVEMENT
{
	switch (key)
	{
	case GLUT_KEY_UP:

		if (last == "square")
		{
			Vsquares.back().Move(0, 0.1);
		}
		else
		if (last == "circle")
		{
			Vcircles.back().Move(0, 0.1);
		}
		else
		if (last == "triangle")
		{
			Vtriangles.back().Move(0, 0.1);
		}
		else
		if (last == "square line")
		{
			Vsquares.back().Move(0, 0.1);
		}
		else
		if (last == "triangle line")
		{
			Vtriangles.back().Move(0, 0.1);
		}
		else
		if (last == "point")
		{
			Vpoints.back().Move(0, 0.1);
		}
		else
		if (last == "line")
		{
			Vlines.back().Move(0, 0.1);
		}

		break;
	case GLUT_KEY_DOWN:
		
		if (last == "square")
		{
			Vsquares.back().Move(0, -0.1);
		}
		else
		if (last == "circle")
		{
			Vcircles.back().Move(0, -0.1);
		}
		else
		if (last == "triangle")
		{
			Vtriangles.back().Move(0, -0.1);
		}
		else
		if (last == "square line")
		{
			Vsquares.back().Move(0, -0.1);
		}
		else
		if (last == "triangle line")
		{
			Vtriangles.back().Move(0, -0.1);
		}
		else
		if (last == "point")
		{
			Vpoints.back().Move(0, -0.1);
		}
	    else
		if (last == "line")
		{
			Vlines.back().Move(0, -0.1);
		}

		break;
	case GLUT_KEY_LEFT:
		
		if (last == "square")
		{
			Vsquares.back().Move(-0.1, 0);
		}
		else
		if (last == "circle")
		{
			Vcircles.back().Move(-0.1, 0);
		}
		else
		if (last == "triangle")
		{
			Vtriangles.back().Move(-0.1, 0);
		}
		else
		if (last == "square line")
		{
			Vsquares.back().Move(-0.1, 0);
		}
		else
		if (last == "triangle line")
		{
			Vtriangles.back().Move(-0.1, 0);
		}
		else
		if (last == "point")
		{
			Vpoints.back().Move(-0.1, 0);
		}
		else
		if (last == "line")
		{
			Vlines.back().Move(-0.1, 0);
		}

		break;
	case GLUT_KEY_RIGHT:
		
		if (last == "square")
		{
			Vsquares.back().Move(0.1, 0);
		}
		else
		if (last == "circle")
		{
			Vcircles.back().Move(0.1, 0);
		}
		else
		if (last == "triangle")
		{
			Vtriangles.back().Move(0.1, 0);
		}
		else
		if (last == "square line")
		{
			Vsquares.back().Move(0.1, 0);
		}
		else
		if (last == "triangle line")
	    {
			Vtriangles.back().Move(0.1, 0);
		}
		else
		if (last == "point")
		{
			Vpoints.back().Move(0.1, 0);
		}
		else
		if (last == "line")
		{
			Vlines.back().Move(0.1, 0);
		}

		break;
	}

	glutPostRedisplay();
}

//Keyboard Functions

void DeleteShape(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's': Vsquares.clear(); break;
	case 'c': Vcircles.clear(); break;
	case 't': Vtriangles.clear();  break;
	case 'p': Vpoints.clear(); break;
	case 'l': Vlines.clear(); break;
	case 27: 
		glutDestroyWindow(window);
		exit(0);
		break;
	case 32:
		Vlines.clear();
		Vpoints.clear();
		Vtriangles.clear();
		Vcircles.clear();
		Vsquares.clear();
		break;
	}

	last = "";
	glutPostRedisplay(); //Refresh
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//----------------Change State---------------//-----------------------------------------------------------------> CHANGE STATE
//////////////////////////////////////////////

void stateChange()
{
	//check if button is clicked

	if (boxClick(0.2, -1 + 0.3, 1 - 0.1)) // Triangle Polygon State |3|
	{
		STATE = 3;
		animation = false;
		return;
	}
	else
	if (boxClick(0.2, -1 + 0.5, 1 - 0.1)) // Square Polygon State |2|
	{
		STATE = 2;
		animation = false;
		return;
	}
	else
	if (boxClick(0.2, -1 + 0.7, 1 - 0.1)) // Circle Polygon State |1|
	{
		STATE = 1;
		animation = false;
		return;
	}
	else
	if (boxClick(0.2, -1 + 0.1, 1 - 0.3)) // Point Line State |4|
	{
		STATE = 4;
		animation = false;
		return;
	}
	else
	if (boxClick(0.2, -1 + 0.1, 1 - 0.5)) // Line Line State |5|
	{
		STATE = 5;
		animation = false;
		return;
	}
	else
	if (boxClick(0.2, -1 + 0.1, 1 - 0.7)) // Square Line State |6|
	{
		STATE = 6;
		animation = false;
		return;
	}
	else
	if (boxClick(0.2, -1 + 0.1, 1 - 0.9)) // Triangle Line State |7|
	{
		STATE = 7;
		animation = false;
		return;
	}
	else
	if (boxClick(0.2, -1 + 0.1, 1 - 0.1)) // CORNER State |0|
	{
		STATE = 0;
		animation = false;
		return;
	}

	//LOOP THROUGH THE REST OF THE UI

	for (GLfloat i = 0.9; i < 2.1; i += 0.2)
	{
		if (boxClick(0.2, -1 + i, 1 - 0.1)) // Top Bounds State |0|
		{
			STATE = 0;
			return;
		}
	}

	for (GLfloat i = 1.1; i < 2.1; i += 0.2)
	{
		if (boxClick(0.2, -1 + 0.1, 1 - i)) // Side Bounds State |0| 
		{
			STATE = 0;
			return;
		}
	}

//Canvas has been clicked

	Create();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//----------------DROP DOWN TASK---------------//-----------------------------------------------------------------> DROP DOWN TASK
//////////////////////////////////////////////

void dropDownTask()
{
	if (DDstate == 1) //Grid on
	{
		gridOn = true;
	}
	else
	if (DDstate == 2) //Grid Off
	{
		gridOn = false;
	}
	else
	if (DDstate == 3) //Animation
	{
		animation = true;

		Vsquares.clear(); 
		Vcircles.clear(); 
		Vtriangles.clear(); 
		Vpoints.clear(); 
		Vlines.clear();

		STATE = 0;
	}

	DD = false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//----------------Click Check----------------//-----------------------------------------------------------------> HAS THE USER CLICKED
//////////////////////////////////////////////

void myMouse(int button, int state, int x, int y)
{
	int yy;
	yy = glutGet(GLUT_WINDOW_HEIGHT);
	y = yy - y; 

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{


		if (!DD) //If drop down doesn't exist
		{
			stateChange(); //Is a button pressed
		}
		else
		{
			dropDownTask(); //Where on drop down is pressed
		}
	}

	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{

		DDmouseX = mouseX; //create drop down
		DDmouseY = mouseY;
		DD = true;
	}

	
	glutPostRedisplay(); //Refresh
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//----------------Reshape--------------------//-----------------------------------------------------------------> RESHAPE
//////////////////////////////////////////////

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	/* uses orthographic (parallel) projection
	use xmin = -1, xmax = 1
	ymin = -1, ymax = 1
	znear = -1, zfar = 1 - not relevant here (2D) */
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//----------------DRAW TEXT------------------//-----------------------------------------------------------------> DRAW TEXT
//////////////////////////////////////////////

void drawText(char *string, float x, float y, GLfloat r, GLfloat g, GLfloat b)
{
	glColor3f(r, g, b);

	char *c;
	glRasterPos2f(x, y);

	for (c = string; *c != '\0'; c++)
	{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//----------------DRAW Line------------------//-----------------------------------------------------------------> DRAW LINE (UI)
//////////////////////////////////////////////

void drawLine(GLfloat length, GLfloat centreX, GLfloat centreY)
{
	GLfloat x1, y1, x2, y2; // calculate vertex based on centre

	x1 = centreX + length / 2;
	y1 = centreY + length / 2;

	x2 = centreX - length / 2;
	y2 = centreY - length / 2;

	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(0);

	glBegin(GL_LINE_LOOP);

	glVertex2f(x1, y1); //Line
	glVertex2f(x2, y2);

	glEnd();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//--------------DROP DOWN BOX----------------//-----------------------------------------------------------------> DRAW DROP DOWN BOX (UI)
//////////////////////////////////////////////

void dropDownBox(GLfloat length, GLfloat X, GLfloat Y, GLfloat r, GLfloat g, GLfloat b, int num)
{
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4;// calculate vertex based on centre

	x1 = X;
	y1 = Y;

	x2 = X + length;
	y2 = Y;

	x3 = X + length;
	y3 = Y - length / 4;

	x4 = X;
	y4 = Y - length / 4;

	if (num == 1) //Save Coordinates
	{
		ddx1_1 = X;
		ddy1_1 = Y;

		ddx2_1 = X + length;
		ddy2_1 = Y;

		ddx3_1 = X + length;
		ddy3_1 = Y - length / 4;

		ddx4_1 = X;
		ddy4_1 = Y - length / 4;
	}
	else
	if (num == 2)
	{
		ddx1_2 = X;
		ddy1_2 = Y;

		ddx2_2 = X + length;
		ddy2_2 = Y;

		ddx3_2 = X + length;
		ddy3_2 = Y - length / 4;

		ddx4_2 = X;
		ddy4_2 = Y - length / 4;
	}
	else
	if (num == 3)
	{
		ddx1_3 = X;
		ddy1_3 = Y;

		ddx2_3 = X + length;
		ddy2_3 = Y;

		ddx3_3 = X + length;
		ddy3_3 = Y - length / 4;

		ddx4_3 = X;
		ddy4_3 = Y - length / 4;
	}


	glColor3f(r, g, b);
	glLineWidth(0);

	if (num != 4)
	{
		glBegin(GL_POLYGON);

		glVertex2f(x1, y1); //Rectangle
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
		glVertex2f(x4, y4);

		glEnd();
	}
	else
	{
		glBegin(GL_LINE_LOOP);

		glVertex2f(x1, y1); //Rectangle
		glVertex2f(x2, y2);
		glVertex2f(ddx3_3, ddy3_3);
		glVertex2f(ddx4_3, ddy4_3);

		glEnd();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//--------------DRAW DROP DOWN----------------//-----------------------------------------------------------------> DRAW DROP DOWN (UI)
//////////////////////////////////////////////

void drawDropDown()
{
	if (DD) //if right click create drop down
	{
		if (DDstate != 1)//GRID ON
		{
			dropDownBox(0.3, DDmouseX, DDmouseY, 0.85, 0.85, 0.85, 1);
			drawText("Grid On", DDmouseX + 0.01, DDmouseY - (0.3 / 3) / 2, 0.0, 0.0, 0.0);
		}
		else
		{
			dropDownBox(0.3, DDmouseX, DDmouseY, 0.15, 0.15, 0.45, 1); //hover
			drawText("Grid On", DDmouseX + 0.01, DDmouseY - (0.3 / 3) / 2, 1.0, 1.0, 1.0);
		}

		if (DDstate != 2)//GRID OFF
		{
			dropDownBox(0.3, DDmouseX, DDmouseY - 0.3 / 4, 0.85, 0.85, 0.85, 2);
			drawText("Grid Off", DDmouseX + 0.01, DDmouseY - ((0.3 / 4) / 2) - (0.3 / 4) - 0.01, 0.0, 0.0, 0.0);
		}
		else
		{
			dropDownBox(0.3, DDmouseX, DDmouseY - 0.3 / 4, 0.15, 0.15, 0.45, 2); //hover
			drawText("Grid Off", DDmouseX + 0.01, DDmouseY - ((0.3 / 4) / 2) - (0.3 / 4) - 0.01, 1.0, 1.0, 1.0);
		}

		if (DDstate != 3)//ANIMATION
		{
			dropDownBox(0.3, DDmouseX, DDmouseY - 0.6 / 4, 0.85, 0.85, 0.85, 3);
			drawText("Animation", DDmouseX + 0.01, DDmouseY - ((0.3 / 4) / 2) - (0.3 / 4) - (0.3 / 4) - 0.01, 0.0, 0.0, 0.0);
		}
		else
		{
			dropDownBox(0.3, DDmouseX, DDmouseY - 0.6 / 4, 0.15, 0.15, 0.45, 3); //hover
			drawText("Animation", DDmouseX + 0.01, DDmouseY - ((0.3 / 4) / 2) - (0.3 / 4) - (0.3 / 4) - 0.01, 1.0, 1.0, 1.0);
		}

		dropDownBox(0.3, DDmouseX, DDmouseY, 0.5, 0.5, 0.5, 4);
	}
}

////////////////////////////////////////////////
//--------------DRAW SQUARE------------------//-----------------------------------------------------------------> DRAW SQUARE (UI)
//////////////////////////////////////////////

void drawSquare(GLfloat length, GLfloat centreX, GLfloat centreY, GLfloat r, GLfloat g, GLfloat b, bool poly, bool stipple, int thickness)
{
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4; // calculate vertex based on centre

	x1 = centreX - length / 2;
	y1 = centreY + length / 2;

	x2 = centreX + length / 2;
	y2 = centreY + length / 2;

	x3 = centreX + length / 2;
	y3 = centreY - length / 2;

	x4 = centreX - length / 2;
	y4 = centreY - length / 2;
	
	glColor3f(r, g, b);  
	glLineWidth(thickness);

	if (stipple == false)
	{
		if (poly)
		{
			glBegin(GL_POLYGON);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}
	}
	else
	{
		glLineStipple(1, 0x9999);
		glBegin(GL_LINE_LOOP);
	}

	glVertex2f(x1, y1); //Square
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);

	glEnd();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//--------------DRAW Triangle----------------//-----------------------------------------------------------------> DRAW TRIANGLE (UI)
//////////////////////////////////////////////

void drawTriangle(GLfloat length, GLfloat centreX, GLfloat centreY, bool USD, bool poly, GLfloat r, GLfloat g, GLfloat b, int width)
{
	GLfloat x1, y1, x2, y2, x3, y3; // calculate vertex based on centre

	if (USD)
	{
		x1 = centreX + length / 2;
		y1 = centreY - length / 2;

		x2 = centreX + length / 2; 
		y2 = centreY + length / 2;

		x3 = centreX - length / 2;
		y3 = centreY - length / 2;
	}
	else
	{
		x1 = centreX + length / 2;
		y1 = centreY - length / 2;

		x2 = centreX / 2 - centreY;
		y2 = centreY + length / 2;

		x3 = centreX - length / 2;
		y3 = centreY - length / 2;
	}

	glColor3f(r, g, b);
	glLineWidth(width);

	if (poly)
	{
		glBegin(GL_POLYGON);
	}
	else
	{
		glBegin(GL_LINE_LOOP); 
	}

	glVertex2f(x1, y1); //Triangle
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);

	glEnd();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//---------------DRAW CIRCLE-----------------//-----------------------------------------------------------------> DRAW CIRCLE (UI)
//////////////////////////////////////////////

void drawCircle(GLfloat cx, GLfloat cy, GLfloat r, int segments, bool poly) 
{
	glColor3f(red + 1.0, green, blue);

	if (poly)
	{
		glBegin(GL_POLYGON);
	}
	else
	{
		glBegin(GL_LINE_LOOP);
	}

	for (int i = 0; i < segments; i++)
	{
		GLfloat theta = 2.0f * 3.1415926f * GLfloat(i) / GLfloat(segments);//get the current angle 
		GLfloat x = r * cosf(theta);//calculate the x
		GLfloat y = r * sinf(theta);//calculate the y 
		glVertex2f(x + cx, y + cy);//output vertex 
	}

	glEnd();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//-------------------UI----------------------//-----------------------------------------------------------------> DRAW THE UI
//////////////////////////////////////////////

void drawUI()
{
	////---////---////---////---////---////---////---////---////---////---////---////---////---////---////---////---////---////

	if (gridOn)
	{
		glEnable(GL_LINE_STIPPLE);
		for (GLfloat i = 0.3; i < 2.1; i += 0.2) //Grid
		{
			for (GLfloat j = 0.3; j < 2.1; j += 0.2)
			{
				drawSquare(0.2, -1 + j, 1 - i, 0.84, 0.84, 0.84, false, true, 0);
			}
		}
		glDisable(GL_LINE_STIPPLE);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////


	anime(); //ANIMATION

	glColor3f(0.58, 0.47, 0.9);    

	glBegin(GL_POLYGON); //UI BG
	glVertex2f(-1, 1);
	glVertex2f(1, 1);
	glVertex2f(1, 0.8);
	glVertex2f(-0.8, 0.8);
	glVertex2f(-0.8, -1);
	glVertex2f(-1, -1);
	glEnd();

	glColor3f(0.18, 0.06, 0.29);
	glLineWidth(2);

	glBegin(GL_LINE_LOOP); //UI BG OUTLINE
	glVertex2f(-1, 1);
	glVertex2f(1, 1);
	glVertex2f(1, 0.8);
	glVertex2f(-0.8, 0.8);
	glVertex2f(-0.8, -1);
	glVertex2f(-1, -1);
	glEnd();

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	drawSquare(0.2, -1+0.1, 1-0.1, 1, 1, 1, true, false, 0); // Corner Square

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//BUTTONS//TOP//

	if (STATE != 3)
	{
		drawSquare(0.2, -1 + 0.3, 1 - 0.1, 0.85, 0.68, 0.99, true, false, 0); //row1bg
	}
	else
	{
		drawSquare(0.2, -1 + 0.3, 1 - 0.1, 1.0, 0.89, 0.58, true, false, 0); //row1bg // COLOUR CHANGE
	}

	drawTriangle(0.1, -1 + 0.3, 1 - 0.1, true, true, red + 1.0, green, blue, 0);//Triangle

	//--//--//--//--//--//

	if (STATE != 2)
	{
		drawSquare(0.2, -1 + 0.5, 1 - 0.1, 0.85, 0.68, 0.99, true, false, 0); //row2bg
	}
	else
	{
		drawSquare(0.2, -1 + 0.5, 1 - 0.1, 1.0, 0.89, 0.58, true, false, 0); //row2bg // COLOUR CHANGE
	}

	drawSquare(0.1, -1 + 0.5, 1 - 0.1, red + 1.0, green, blue, true, false, 0);//Square

	//--//--//--//--//--//

	if (STATE != 1)
	{
		drawSquare(0.2, -1 + 0.7, 1 - 0.1, 0.85, 0.68, 0.99, true, false, 0); //row3bg
	}
	else
	{
		drawSquare(0.2, -1 + 0.7, 1 - 0.1, 1.0, 0.89, 0.58, true, false, 0); //row3bg // COLOUR CHANGE
	}

	drawCircle(-1 + 0.7, 1 - 0.1, 0.05, 32, true);//Circle

	//--//--//--//--//--//

	//OUTLINES//
	drawSquare(0.2, -1 + 0.3, 1 - 0.1, 0.18, 0.06, 0.29, false, false, 2); //row1ln
	drawSquare(0.2, -1 + 0.5, 1 - 0.1, 0.18, 0.06, 0.29, false, false, 2); //row2ln
	drawSquare(0.2, -1 + 0.7, 1 - 0.1, 0.18, 0.06, 0.29, false, false, 2); //row2ln

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//BUTTONS//SIDE//

	if (STATE != 4)
	{
		drawSquare(0.2, -1 + 0.1, 1 - 0.3, 0.85, 0.68, 0.99, true, false, 0); //col1bg
	}
	else
	{
		drawSquare(0.2, -1 + 0.1, 1 - 0.3, 1.0, 0.89, 0.58, true, false, 0); //col1bg // COLOUR CHANGE
	}

	drawSquare(0.01, -1 + 0.1, 1 - 0.3, 0.0, 0.0, 0.0, true, false, 0); //Point

	//--//--//--//--//--//

	if (STATE != 5)
	{
		drawSquare(0.2, -1 + 0.1, 1 - 0.5, 0.85, 0.68, 0.99, true, false, 0); //col2bg
	}
	else
	{
		drawSquare(0.2, -1 + 0.1, 1 - 0.5, 1.0, 0.89, 0.58, true, false, 0); //col2bg // COLOUR CHANGE
	}

	drawLine(0.1, -1 + 0.1, 1 - 0.5);//Line

	//--//--//--//--//--//

	if (STATE != 6)
	{
		drawSquare(0.2, -1 + 0.1, 1 - 0.7, 0.85, 0.68, 0.99, true, false, 0); //col3bg
	}
	else
	{
		drawSquare(0.2, -1 + 0.1, 1 - 0.7, 1.0, 0.89, 0.58, true, false, 0); //col3bg // COLOUR CHANGE
	}

	drawSquare(0.1, -1 + 0.1, 1 - 0.7, 0.0, 0.0, 0.0, false, false, 0); //Square

	//--//--//--//--//--//

	if (STATE != 7)
	{
		drawSquare(0.2, -1 + 0.1, 1 - 0.9, 0.85, 0.68, 0.99, true, false, 0); //col4bg
	}
	else
	{
		drawSquare(0.2, -1 + 0.1, 1 - 0.9, 1.0, 0.89, 0.58, true, false, 0); //col4bg // COLOUR CHANGE
	}

	drawTriangle(0.1, -1 + 0.1, 1 - 0.9, true, false, 0.0, 0.0, 0.0, 0);//Triangle

	//--//--//--//--//--//

	//OUTLINES//
	drawSquare(0.2, -1 + 0.1, 1 - 0.3, 0.18, 0.06, 0.29, false, false, 2); //col1ln
	drawSquare(0.2, -1 + 0.1, 1 - 0.5, 0.18, 0.06, 0.29, false, false, 2); //col2ln
	drawSquare(0.2, -1 + 0.1, 1 - 0.7, 0.18, 0.06, 0.29, false, false, 2); //col3ln
	drawSquare(0.2, -1 + 0.1, 1 - 0.9, 0.18, 0.06, 0.29, false, false, 2); //col4ln

	////---////---////---////---////---////---////---////---////---////---////---////---////---////---////---////---////---////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//-------------------DISPLAY-----------------//-----------------------------------------------------------------> DISPLAY
//////////////////////////////////////////////

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);   /* clear window */

	drawUI(); //UI

	DrawNewShape(); //draw new shape

	if (!DD)
	{
		drawText("2D Drawing Canvas", -0.125, -0.9, 0.0, 0.0, 0.0);
	}
	else
	{
		drawText("Leave Menu to Exit Drop Down", -0.22, -0.9, 0.0, 0.0, 0.0);
	}

	drawDropDown(); //Drop Down Menu

	if(animation)
		glutPostRedisplay(); //Refresh

	glFlush();     /* execute drawing commands in buffer */

}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//-------------------INIT--------------------//-----------------------------------------------------------------> INIT
//////////////////////////////////////////////

void init(void)
{
	glClearColor(1, 1, 1, 0.0);   /* window Colour*/
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
//-------------------MAIN--------------------//-----------------------------------------------------------------> MIAN
//////////////////////////////////////////////

int main(int argc, char** argv)
{
	std::cout << "How to draw: Controls and interaction //Program Created by Joseph Calabro W1635359\n\n\n";
	std::cout << "1) Left click on a box to the left or upper menu to select a primitive\nor filled shape.\n\n";
	std::cout << "2) Right click for menu options.\n\n";
	std::cout << "3) Click once on the canvas to draw your selected shape, twice if the line is selected.\n\n";
	std::cout << "4) Use the arrow keys to control the last shape drawn on the canvas.\n\n";
	std::cout << "5) 'S' to delete all squares.\n\n";
	std::cout << "6) 'C' to delete all circles.\n\n";
	std::cout << "7) 'T' to delete all triangles.\n\n";
	std::cout << "8) 'P' to delete all points.\n\n";
	std::cout << "9) 'L' to delete all lines.\n\n";
	std::cout << "10) 'Space' to delete all Shapes.\n\n";
	std::cout << "11) 'Esc' to exit the program.";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	int window;
	window = glutCreateWindow("CW1 - W1635359 - Joseph Calabro");
	init();
	glutDisplayFunc(display);
	glutPassiveMotionFunc(mousePos);

	glutMouseFunc(myMouse);

	glutKeyboardFunc(DeleteShape);
	glutSpecialFunc(SpecialInput);
	glutReshapeFunc(reshape);

	glutMainLoop();
	return 0;
}

