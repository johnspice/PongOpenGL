//-------------------------------------------------------------------------------------
//  main.cpp
//  PongOpenGL
//  Created by juan gabriel lopez on 20/03/24.
//-------------------------------------------------------------------------------------
#include <GLUT/glut.h>
#include <math.h>
#include <string>


//Screen values
int screenHeight=1000;
int screenWidth=1500;

double sx, sy, squash;        // xy scale factors
double rot, rdir;             // rotation
//Global Vars----------------------------------------------------------------------------
int SPEED = 60;        // speed of timer call back in msecs
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
GLfloat RadiusOfBall = 15.;
double xpos, ypos, ydir, xdir;// x and y position for house to be drawn
GLint circle_points = 100;

//paletas
float heightPaleta=12.0f;
float widthPaleta=2.0f;
float posPaleta1X = 5.0f;//posiciones iniciales paleta 1 y 2
float posPaleta1Y = 50.0f;
float posPaleta2X = 152.0f;
float posPaleta2Y = 50.0f;



void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':posPaleta1Y += 10.0f;
            break;
        case 's':posPaleta1Y -= 10.0f;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}



void specialKeyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:posPaleta2Y += 10.0f;
            break;
        case GLUT_KEY_DOWN:posPaleta2Y -= 10.0f;
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



void drawPaletasAndScore() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 160.0, 0.0, 120.0); // Keep our logical coordinate system constant
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    drawScore(3,5);
    drawPaleta(posPaleta1X,posPaleta1Y);
    drawPaleta(posPaleta2X,posPaleta2Y);
 
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}



void Display(void)
{
  // swap the buffers
  glutSwapBuffers();//clear all pixels with the specified clear color
  glClear(GL_COLOR_BUFFER_BIT);
  // 160 is max X value in our world
  // Define X position of the ball to be at center of window
  xpos = 80.;
     
   // Shape has hit the ground! Stop moving and start squashing down and then back up
  if (ypos == RadiusOfBall && ydir == -1  ) {
        sy = sy*squash ;
        if (sy < 0.8)
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
      ypos = ypos+ydir *1.5 - (1.-sy)*RadiusOfBall;
      if (ypos == 120-RadiusOfBall)    // If ball touches the top, change direction of ball downwards
         ydir = -1;
      else if (ypos <RadiusOfBall)    // If ball touches the bottom, change direction of ball upwards
         ydir = 1;
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
  
  draw_ball();
  drawPaletasAndScore();
    
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
  xpos = 60; ypos = RadiusOfBall; xdir = 1; ydir = 1;
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
