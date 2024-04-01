//-------------------------------------------------------------------------------------
//  main.cpp
//  PongOpenGL
//  Created by juan gabriel lopez on 20/03/24.
//-------------------------------------------------------------------------------------


#include <GLUT/glut.h>
#include <math.h>
#include <string>
#include <iostream>


//Screen values
int screenHeight=1000;
int screenWidth=1600;
double sx, sy, squash;        // xy scale factors
double rot, rdir;             // rotation
//Global Vars----------------------------------------------------------------------------
int velocidad=1;//0 es la velocidad por default para mas lento aumentar esta variable
int contador=0;
GLfloat T1[16] = {1.,0.,0.,0.,\
                  0.,1.,0.,0.,\
                  0.,0.,1.,0.,\
                  0.,0.,0.,1.};
GLfloat S[16] = {1.,0.,0.,0.,\
                 0.,1.,0.,0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};
GLfloat T[16] = {1.,0.,0.,0.,\
                 0., 1., 0., 0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};
#define PI 3.1415926535898
//ball
GLfloat RadiusOfBall = 2.5;//default 15
double xpos, ypos, ydir, xdir;// x and y position for house to be drawn
GLint circle_points = 100;
//paletas
float heightPaleta=20.0f;//15
float widthPaleta=2.0f;
float posPaleta1X = 12.0f;//posiciones iniciales paleta 1 y 2
float posPaleta1Y = 50.0f;
float posPaleta2X = 145.0f;
float posPaleta2Y = 50.0f;
int scoreP1=0;
int scoreP2=0;


void printLogs(const std::string&TAG,const std::string& ms){
    std::cout <<TAG<<":"<<ms  << std::endl;
}

void checkColission(){
    //printLogs("checkcolission","paletax:"+ std::to_string(posPaleta2X));
    //printLogs("checkcolission","xpos:"+ std::to_string(xpos));
    //printLogs("checkcolission","ypos:"+ std::to_string(ypos));
    //printLogs("checkcolission","yposPaleta:"+ std::to_string(posPaleta2Y));
    if(xpos<posPaleta1X+RadiusOfBall+widthPaleta && ypos<posPaleta1Y+heightPaleta+RadiusOfBall && ypos>posPaleta1Y-RadiusOfBall){
        printLogs("checkcolission","paleta1");
        xdir=+1;
    }
    if(xpos>posPaleta2X-RadiusOfBall && ypos<posPaleta2Y+heightPaleta+RadiusOfBall && ypos>posPaleta2Y-RadiusOfBall){
        printLogs("checkcolission","paleta2");
        xdir=-1;
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            if(posPaleta1Y<100){
                posPaleta1Y += 15.0f;
            }
            break;
        case 's':
            if(posPaleta1Y>0){
                posPaleta1Y -= 15.0f;
            }
            break;
        default:
            break;
    }
    glutPostRedisplay();
}



void specialKeyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if(posPaleta2Y<100){
                posPaleta2Y += 15.0f;
            }
            break;
        case GLUT_KEY_DOWN:
            if(posPaleta2Y>0){
                posPaleta2Y -= 15.0f;
            }
            break;
        default:
            break;
    }
    glutPostRedisplay();
}



void MyCircle2f(GLfloat centerx, GLfloat centery, GLfloat radius){
  GLint i;
  GLdouble angle;
  glBegin(GL_POLYGON);
  for (i = 0; i < circle_points; i++) {
    angle = 2*PI*i/circle_points;
    glVertex2f(centerx+radius*cos(angle), centery+radius*sin(angle));
  }
  glEnd();
}



// Draw the ball, centered at the origin
void draw_ball() {
  glColor3f(0.0,1.0,0.0);
  MyCircle2f(0.,0.,RadiusOfBall);
}



void drawScore(GLint scorep1,GLint scorep2){
    glColor3f(1.0f, 1.0f, 1.0f); // Establecer el color a blanco
    glRasterPos2f(1.0, 115.0); // Establecer la posición inicial del texto
    std::string text = std::string("P1: ") + std::to_string(scorep1);
    const char *texts = text.c_str();
    for (const char *c = texts; *c != '\0'; c++) {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glRasterPos2f(150.0, 115.0); // Establecer la posición inicial del texto
    std::string text2 = std::string("P2: ") + std::to_string(scorep2);
    const char *textss = text2.c_str();
    for (const char *c = textss; *c != '\0'; c++) {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glFlush();
}


void drawPaleta(float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f); // Color blanco
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + widthPaleta, y);
    glVertex2f(x + widthPaleta, y + heightPaleta);
    glVertex2f(x, y + heightPaleta);
    glEnd();
    
}

void drawMidleLine() {
    glColor3f(0.1f, 0.1f, 0.1f); // Color blanco
    glBegin(GL_QUADS);
    glVertex2f(76, 0);
    glVertex2f(76 + 0.8, 0);
    glVertex2f(76 + 0.8, 120);
    glVertex2f(76,  120);
    glEnd();
}



void drawPaletasAndScore() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 160.0, 0.0, 120.0); // Keep our logical coordinate system constant
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    drawMidleLine();
    drawScore(scoreP1,scoreP2);
    drawPaleta(posPaleta1X,posPaleta1Y);
    drawPaleta(posPaleta2X,posPaleta2Y);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}



void Display(void)
{
  glClearColor(0.0,0.0,0.0,0.0);//set the clear color to be white. initial position set to 0,0
  // swap the buffers
  glutSwapBuffers();//clear all pixels with the specified clear color
  glClear(GL_COLOR_BUFFER_BIT);
  // 160 is max X value in our world
  // Define X position of the ball to be at center of window
     
   
   // Shape has hit the ground! Stop moving and start squashing down and then back up
  if (ypos == RadiusOfBall && ydir == -1  ) {
        sy = sy*squash ;
        if (sy < 0.8)//0.8 defaul
            squash = 1.1;// reached maximum suqash, now unsquash back up
        else if (sy > 1.) {
            sy = 1.; // reset squash parameters and bounce ball back upwards
            squash = 0.9;
            ydir = 1;
        }
        sx = 1./sy;
   }
    // 120 is max Y value in our world set Y position to increment 1.5 times the direction of the bounce
   else {
       
       if(contador==velocidad){
           ypos = ypos+ydir *1.5 - (1.-sy)*RadiusOfBall;
           xpos = xpos+xdir *1.5 - (1.-sx)*RadiusOfBall;
           contador=0;
       }else{
           contador=contador+1;
       }
       
       
      if (ypos > 120-RadiusOfBall)    // If ball touches the top, change direction of ball downwards
         ydir = -1;
      else if (ypos <=RadiusOfBall)    // If ball touches the bottom, change direction of ball upwards
         ydir = 1;
       
       
       if (xpos > 157-RadiusOfBall) {   // If ball touches the top, change direction of ball downwards
           xdir = -1;
           //glClearColor(1.0,0.0,0.0,0.0);//set the clear color to be white. initial position set to 0,0
           //glClear(GL_COLOR_BUFFER_BIT);
           //printLogs("checkcolission","----------");
           scoreP1=scoreP1+1;
       }
       else if (xpos <=RadiusOfBall) {   // If ball touches the bottom, change direction of ball upwards
           xdir = 1;
           //glClearColor(1.0,0.0,0.0,0.0);
           //glClear(GL_COLOR_BUFFER_BIT);
           //printLogs("checkcolission","+++++");
           scoreP2=scoreP2+1;
       }
       
   }

  //Translate the bouncing ball to its new position
  T[12]= xpos;
  T[13] = ypos;
  glLoadMatrixf(T);
    
  T1[13] = -RadiusOfBall;// Translate ball back to center
  glMultMatrixf(T1);
  S[0] = sx;
  S[5] = sy;// Scale the ball about its bottom
  glMultMatrixf(S);
  T1[13] = RadiusOfBall;// Translate ball up so bottom is at the origin
  glMultMatrixf(T1);
     
    
  
  
  drawPaletasAndScore();
  checkColission();
  draw_ball();
    
  glutPostRedisplay();
}







void reshape (int w, int h)
{
   // on reshape and on startup, keep the viewport to be the entire size of the window
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   // keep our logical coordinate system constant
   gluOrtho2D(0.0, 160.0, 0.0, 120.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();
}


void init(void){
  glClearColor(0.0,0.0,0.0,0.0);//set the clear color to be white. initial position set to 0,0
  xpos = 60; ypos = RadiusOfBall; 
  xdir = 1; ydir = 1;
  sx = 1.; sy = 1.; squash = 0.9;
  rot = 0;
}


int main(int argc, char* argv[])
{
  glutInit( & argc, argv );
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (screenWidth, screenHeight);
  glutCreateWindow("Bouncing Ball");
  init();
  glutDisplayFunc(Display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);//leer del teclado
  glutSpecialFunc(specialKeyboard);//leer del teclado
  glutMainLoop();
  return 1;
}
