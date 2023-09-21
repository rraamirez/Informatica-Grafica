#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>



void Circle (GLfloat radio, GLfloat cx, GLfloat cy, GLint n, GLenum modo)
{
   int i;
   if (modo==GL_LINE) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else if (modo==GL_FILL) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
   glBegin( GL_POLYGON );
   for (i=0;i<n;i++)
   glVertex2f( cx+radio*cos(2.0*M_PI*i/n), cy+radio*sin(2.0*M_PI*i/n));
   glEnd();
}

void Ejes (int width)
{   
    glLineWidth(width);
    glBegin(GL_LINES);
       glColor3f(1.0,0.0,0.0);
       glVertex3f(-1.0,0.0,0.0);
       glVertex3f(1.0,0.0,0.0);
       glColor3f(0.0,1.0,0.0);
       glVertex3f(0.0,-1.0,0.0);
       glColor3f(1.0,1.0,0.0);
       glVertex3f(0.0,1.0,0.0);   
    glEnd();
    glLineWidth(1);
       
}


void Monigote ()
{
  
 
// // cara
//    glLineWidth(1);
//    glColor3f(1.0,0.8,0.6);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    glBegin(GL_POLYGON);
//       glVertex3f(-0.2,0.0,0.0);
//       glVertex3f(0.2,0.0,0.0);
//       glVertex3f(0.2,0.55,0.0);
//       glVertex3f(-0.2,0.55,0.0);
//    glEnd(); 

//    glColor3f(0.0,0.0,0.);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glBegin(GL_POLYGON);
//       glVertex3f(-0.2,0.0,0.0);
//       glVertex3f(0.2,0.0,0.0);
//       glVertex3f(0.2,0.55,0.0);
//       glVertex3f(-0.2,0.55,0.0);
//    glEnd();

//    //ojos
//    glColor3f(1.0,0.0,1.0);
//    Circle(0.05,0.1,0.35,20,GL_FILL);
//    Circle(0.05,-0.1,0.35,20,GL_LINE);

//    //DRAWING THE MOUTH
//    glColor3f(1.0,0.0,0.0);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    glBegin(GL_POLYGON);
//       glVertex3f(-0.1,0.2,0.0);
//       glVertex3f(0.1,0.2,0.0);
//       glVertex3f(0.1,0.25,0.0);
//       glVertex3f(-0.1,0.25,0.0);
//    glEnd();

   
}

static void Init( )
{

   glShadeModel( GL_SMOOTH );
}


static void Reshape( int width, int height )
{
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity( );
    glViewport(0, 0, (GLint)width, (GLint)height);
    glOrtho (-1.0, 1.0,-1.0, 1.0, -10, 10.0);
}

static void Display( )
{

  glClearColor(0.6,0.6,0.9,0.0);
  glClear( GL_COLOR_BUFFER_BIT );
   
   glMatrixMode( GL_MODELVIEW);
   glLoadIdentity( );
  
   
   Ejes(6);
   Monigote();
   
   glFlush();
}


static void Keyboard(unsigned char key, int x, int y )
{
 
  if (key==27)
      exit(0);

}


int main( int argc, char **argv )
{
   glutInit(&argc,argv);
   glutInitDisplayMode( GLUT_RGB );

   glutInitWindowPosition( 20, 100 );
   glutInitWindowSize(500, 500 );
   glutCreateWindow("Practica 0 IG");


   Init();

   glutReshapeFunc(Reshape);
   glutDisplayFunc(Display);
   glutKeyboardFunc(Keyboard);
  
   glutMainLoop( );

   return 0;
}

