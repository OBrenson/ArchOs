#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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
    glColor3f(0.0, 0.8, 0.2); // Зеленый
    glPolygonStipple(mask);
    glRectf(-1, 0, 1, -1);
    glDisable(GL_POLYGON_STIPPLE);
    glEnd();

    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.9);
    glColor3f(0.22,  0.69, 0.87); // blue
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

void drawCaw(GLUquadricObj *q, float bodyC[], float headC[])
{
    // body
    glColor3f(bodyC[0], bodyC[1], bodyC[2]);
    glutSolidCube(0.4);
    glTranslatef(0.34, 0.0, 0.0);
    glutSolidCube(0.4);

    glColor3f(1.0, 1.0, 1.0);
    // legs
    glTranslatef(0.0, -0.27, -0.1);
    glutSolidCube(0.14);
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, +0.2);
    glutSolidCube(0.14);
    glTranslatef(-0.45, 0.0, 0.0);
    glutSolidCube(0.14);
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, 0.0, -0.2);
    glutSolidCube(0.14);

    // head
    glColor3f(headC[0], headC[1], headC[2]);
    glTranslatef(-0.2, +0.4, 0.1);
    glutSolidCube(0.26);

    // nose
    glColor3f(0.737, 0.560, 0.56);
    glTranslatef(-0.15, -0.034, +0.0);
    glutSolidCube(0.17);
    glColor3f(0.0, 0.0, 0.0);

    // eyes
    glTranslatef(+0.025, +0.12, +0.07);
    gluSphere(q, 0.03, 30, 30);
    glTranslatef(0.0, 0.0, -0.14);
    gluSphere(q, 0.03, 30, 30);

    // horns
    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(+0.1, +0.05, -0.07);
    glutSolidCube(0.07);
    glTranslatef(+0.0, +0.00, +0.27);
    glutSolidCube(0.07);

    glColor3f(0.1, 0.1, 0.1);
    glTranslatef(+0.87, -0.091, -0.1);
    glutSolidCube(0.06);

    glPopMatrix();
}

void display(void)
{
    GLfloat lp[4] = {-0.3,0.3,+0.2,1};
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    displayBackground();
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);               // Разрешить работу Z-буфера
    glLightfv(GL_LIGHT0, GL_POSITION, lp); // Загрузить x,y,z
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);                 // Включить лампочку
    glEnable(GL_COLOR_MATERIAL);           // Разрешить материал
    glEnable(GL_NORMALIZE);

    GLUquadricObj *q = gluNewQuadric();

    gluQuadricDrawStyle(q, GLU_FILL);

    glPushMatrix();
    glLoadIdentity();
    glScaled(0.5, 0.5, 0.5);
    glRotatef(-45.0, 1.0, 4.0, -1.0);
    glTranslatef(-1.3, -0.5, 0.4);
    drawCaw(q, (float[]){0.85, 0.85, 0.85}, (float[]){0.7, 0.7, 0.7});

    glPushMatrix();
    glLoadIdentity();
    glScaled(0.42, 0.42, 0.42);
    glRotatef(-135.0, 0.0, 4.0, -0.5);
    glTranslatef(-2.25, -0.0, 0.4);
    drawCaw(q, (float[]){0.4, 0.4, 0.4}, (float[]){0.3, 0.3, 0.3});

    glPushMatrix();
    glLoadIdentity();
    glScaled(1.8, 1.8, 1.8);
    glRotatef(-70.0, 0.0, 4.0, 0.0);
    glTranslatef(0.0, -0.5, 0.0);
    drawCaw(q, (float[]){0.6, 0.6, 0.6}, (float[]){0.7, 0.7, 0.7});

    glPushMatrix();
    glLoadIdentity();
    glScaled(0.3, 0.3, 0.3);
    glRotatef(+90, 0, 1, 0);
    drawCaw(q, (float[]){0.65, 0.65, 0.65}, (float[]){0.7, 0.7, 0.7});

    glPushMatrix();
    glLoadIdentity();
    glScaled(0.3, 0.3, 0.3);
    glTranslatef(-2.0, 0.0, 0.0);
    drawCaw(q, (float[]){0.309804, 0.184314, 0.184314}, (float[]){0.36, 0.25, 0.20});

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