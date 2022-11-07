#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define SIZET 128
struct PATTERN
{
    unsigned char r, g, b;
};
struct PATTERN pixels[SIZET][SIZET];

GLfloat body[][3] =
    {{0.2f, 0.2f, -0.8f}, //top
     {-0.2f, 0.2f, -0.8f},
     {-0.2f, 0.2f, 0.2f},
     {0.2f, 0.2f, 0.2f},

     {0.2f, -0.2f, 0.2f}, //bottom
     {-0.2f, -0.2f, 0.2f},
     {-0.2f, -0.2f, -0.8f},
     {0.2f, -0.2f, -0.8f},

     {0.2f, 0.2f, 0.2f}, //front
     {-0.2f, 0.2f, 0.2f},
     {-0.2f, -0.2f, 0.2f},
     {0.2f, -0.2f, 0.2f},

     {0.2f, -0.2f, -0.8f}, //back
     {-0.2f, -0.2f, -0.8f},
     {-0.2f, 0.2f, -0.8f},
     {0.2f, 0.2f, -0.8f},

     {-0.2f, 0.2f, 0.2f}, //left
     {-0.2f, 0.2f, -0.8f},
     {-0.2f, -0.2f, -0.8f},
     {-0.2f, -0.2f, 0.2f},

     {0.2f, 0.2f, -0.8f}, //right
     {0.2f, 0.2f, 0.2f},
     {0.2f, -0.2f, 0.2f},
     {0.2f, -0.2f, -0.8f}};

GLfloat lfLeg[][3] =
    {{-0.1f, -0.2f, 0.2f},
     {-0.2f, -0.2f, 0.2f}, //back
     {-0.2f, -0.5f, 0.2f},
     {-0.1f, -0.5f, 0.2f},

     {-0.1f, -0.2f, -0.2f},
     {-0.2f, -0.2f, -0.2f}, //front
     {-0.2f, -0.5f, -0.2f},
     {-0.1f, -0.5f, -0.2f},
     
     {-0.1f, -0.2f, -0.2f},
     {-0.1f, -0.2f, -0.1f}, //left
     {-0.1f, -0.5f, -0.1f},
     {-0.1f, -0.5f, -0.2f},

     };

void displayBackground()
{
    GLubyte mask[128];
    for (int i = 0; i < 128; i++) // Заполнение текстуры
        if (i % 7 == 0)
        {
            mask[i] = 0x0F;
        }
        else
        {
            mask[i] = 0;
        }
    glClearColor(0., 0.6, 0.4, 1.);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POLYGON_STIPPLE);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.9);
    glColor3f(0.0, 0.55, 0.2); // Зеленый
    glPolygonStipple(mask);
    glRectf(-1, 0, 1, -1);
    glDisable(GL_POLYGON_STIPPLE);
    glEnd();

    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.9);
    glColor3f(0.0, 0.6, 0.9); // blue
    glPolygonStipple(mask);
    glRectf(-1, 1, 1, 0);
    glEnd();
}

void drawRect(GLfloat body[][3], int len, float color[3])
{
    glLoadIdentity();
    glRotatef(90.0, 0.0, 0.2, 0.0);
    glBegin(GL_QUADS);
    glColor3f(color[0], color[1], color[2]);
    for (int i = 0; i < len; i++)
    {
        glVertex3f(body[i][0], body[i][1], body[i][2]);
    }
    glEnd();
}

void drawCaw(GLUquadricObj *q) {
    gluSphere(q, 0.3, 30, 30);
    glTranslatef(0.3, 0.0, 0.0);
    gluSphere(q, 0.3, 30, 30);
    glTranslatef(0.0, -0.3, -0.1);
    gluSphere(q, 0.1, 30, 30);
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, +0.2);
    gluSphere(q, 0.1, 30, 30);
    glTranslatef(-0.45, 0.0, 0.0);
    gluSphere(q, 0.1, 30, 30);
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, 0.0, -0.2);
    gluSphere(q, 0.1, 30, 30);
    glTranslatef(-0.1, +0.4, +0.1);
    gluSphere(q, 0.2, 30, 30);
    glTranslatef(-0.20, +0.05, +0.05);
    gluSphere(q, 0.05, 30, 30);
    glTranslatef(0.0, +0.04, -0.01);
    gluSphere(q, 0.03, 30, 30);
    glLoadIdentity();
}

void display(void)
{
    GLfloat lp[4] = {0.2, 0.1, 0.0, 1}; // x,y,z лампочки
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    displayBackground();
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);               // Разрешить работу Z-буфера
    glLightfv(GL_LIGHT0, GL_POSITION, lp); // Загрузить x,y,z
    glEnable(GL_LIGHT0);                   // Разрешить 0-ю лампочку
    glEnable(GL_LIGHTING);                 // Включить лампочку
    glEnable(GL_COLOR_MATERIAL);           // Разрешить материал
    glEnable(GL_NORMALIZE);

    GLUquadricObj *q = gluNewQuadric();
    gluQuadricDrawStyle(q, GLU_FILL);
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(-90.0, 0.0, 30.0, 0.0);
    drawCaw(q);
    gluDeleteQuadric(q);
    glFlush();

    glutSwapBuffers();
    glFlush();
}

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(640, 640);
    glutCreateWindow("My simple GLUT example");
    glClearColor(0., 0., 0., 0.);
    glEnable(GL_NORMALIZE);
    glutDisplayFunc(display);
    glutMainLoop();
}