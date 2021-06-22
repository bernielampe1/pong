#include<iostream>
#include<math.h>
#include<stdlib.h>

#ifdef __APPLE_CC__
#define GL_SILENCE_DEPRECATION 1
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

using namespace std;

// screen window default size
GLint width = 640, height = 480;

// screen window handle
GLint handle;

// positions and dimensions of both paddles
GLfloat pdl_x0 = 10, pdl_y0 = 470;
GLfloat pdl_x1 = 610, pdl_y1 = 470;
GLfloat pdl_height = 80, pdl_width = 20;
GLfloat pdl_inc = 10;

// initial position, radius, speed and direction of the ball
GLfloat ball_x = 320, ball_y = 240;
GLfloat ball_r = 10;
GLfloat ball_speed = 3;
GLfloat ball_dx = M_SQRT1_2, ball_dy = M_SQRT1_2;

// scores for each player
GLint p1_score = 0, p2_score = 0;

void disp(void)
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);

  // draw paddles
  glBegin(GL_QUADS);

  // paddle 0
  glVertex2f(pdl_x0, pdl_y0);
  glVertex2f(pdl_x0 + pdl_width, pdl_y0);
  glVertex2f(pdl_x0 + pdl_width, pdl_y0 - pdl_height);
  glVertex2f(pdl_x0, pdl_y0 - pdl_height);

  // paddle 1
  glVertex2f(pdl_x1, pdl_y1);
  glVertex2f(pdl_x1 + pdl_width, pdl_y1);
  glVertex2f(pdl_x1 + pdl_width, pdl_y1 - pdl_height);
  glVertex2f(pdl_x1, pdl_y1 - pdl_height);
  glEnd();

  // draw ball
  glBegin(GL_TRIANGLE_FAN);

  for(int i = 0; i <= 90; i++)
  {
    GLfloat a = i * M_PI / 45.0;
    GLfloat x = ball_x + ball_r * cos(a);
    GLfloat y = ball_y + ball_r * sin(a);
    glVertex2f(x, y);
  }

  glEnd();

  // draw score
  glRasterPos2f(width / 2 - 20, height - 20);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (char)(p1_score + 48));

  glBegin(GL_QUADS);
  glVertex2f(width / 2, height - 20);
  glVertex2f(width / 2 + 2, height - 20);
  glVertex2f(width / 2 + 2, height);
  glVertex2f(width / 2, height);
  glEnd();

  glRasterPos2f(width / 2 + 10, height - 20);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (char)(p2_score + 48));

  glutSwapBuffers();
}

void idle(void)
{
  ball_x += ball_speed * ball_dx;
  ball_y += ball_speed * ball_dy;

  if (ball_y - ball_r / 2 <= 0)
  {
    ball_dy *= -1.0;
    ball_y += 1;
  }
  else if (ball_y + ball_r / 2 >= height)
  {
    ball_dy *= -1.0;
    ball_y -= 1;
  }

  if (ball_x - ball_r / 2 <= pdl_x0 + pdl_width &&
      ball_y >= pdl_y0 - pdl_height &&
      ball_y <= pdl_y0)
  {
    ball_dx *= -1.0;
    ball_x += 1;
  }
  else if (ball_x + ball_r / 2 >= pdl_x1 &&
      ball_y >= pdl_y1 - pdl_height &&
      ball_y <= pdl_y1)
  {
    ball_dx *= -1.0;
    ball_x -= 1;
  }
  else if (ball_x - ball_r / 2 <= pdl_x0)
  {
    p2_score++;

    ball_dx = -M_SQRT1_2; ball_dy = M_SQRT1_2;
    ball_x = 320; ball_y = 240;
  }
  else if (ball_x + ball_r / 2 >= pdl_x1 + pdl_width)
  {
    p1_score++;

    ball_dx = M_SQRT1_2; ball_dy = M_SQRT1_2;
    ball_x = 320; ball_y = 240;
  }

  double offset = (ball_y - pdl_y1 + pdl_height / 2) * 
                  pow((double)ball_x / (width * 2.0), 2.5);
  if (offset > 0.32) pdl_y1 += 1;
  else if (offset < -0.32) pdl_y1 -= 1;
  else pdl_y1 += offset;

  if (pdl_y1 >= height) pdl_y1 = height;
  else if (pdl_y1 <= pdl_height) pdl_y1 = pdl_height;

  disp();
}

void reshape(int w, int h)
{
  width = (GLdouble)w;
  height = (GLdouble)h;

  pdl_x0 = 10;
  pdl_x1 = width - 30;
  if (pdl_y0 >= height) pdl_y0 = height;
  if (pdl_y0 - pdl_height <= 0) pdl_y0 = pdl_height;

  if (pdl_y1 >= height) pdl_y1 = height;
  if (pdl_y1 - pdl_height <= 0) pdl_y1 = pdl_height;

  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, width, 0.0, height);
}

void keyb(unsigned char key, int x, int y)
{
  switch((char)key)
  {
    case 'a':
    case 'A':
      pdl_y0 += pdl_inc;
      if (pdl_y0 >= height) pdl_y0 = height;
      break;

    case 'z':
    case 'Z':
      pdl_y0 -= pdl_inc;
      if (pdl_y0 <= pdl_height) pdl_y0 = pdl_height;
      break;

    case 'q':
    case 27:
      glutDestroyWindow(handle);
      return;
  }

  glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(width, height);
  handle = glutCreateWindow("Classic Pong");
  glutDisplayFunc(disp);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyb);
  glutMainLoop();

  return(0);
}

