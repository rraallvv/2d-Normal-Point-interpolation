#include <iostream>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <math.h>

typedef struct {
	GLfloat x,y;
} Vec2;

void idle() {
	glutPostRedisplay();
}

inline void pointNormalInterpolation(float *x, float *x0, float *n0, float *x1, float *n1, float e) {
	float d[2];
	d[0] = x1[0] - x0[0];
	d[1] = x1[1] - x0[1];
	
	float k1 = n0[0]*n1[1]+n0[1]*n1[0];
	float k2 = n0[0]*n1[1]-n0[1]*n1[0];
	
	float c[2];
	c[0] = (k1*d[0]+2*n0[1]*n1[1]*d[1])/k2;
	c[1] = -(2*n0[0]*n1[0]*d[0]+k1*d[1])/k2;
	
	x[0] = x0[0]+(d[0]-c[0])*e+c[0]*e*e;
	x[1] = x0[1]+(d[1]-c[1])*e+c[1]*e*e;
}

void displayCall() {
	static float t = 0;
	t += 1.0e-2;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 500.0);
    
	glColor3f(1,1,1);
	
	Vec2 x0 = {-.5, 0};
	Vec2 x1 = {+.5, 0};
	Vec2 n0 = {-cosf(t)*cosf(M_PI_4), cosf(t)*sinf(M_PI_4)};
	Vec2 n1 = {cosf(M_PI_4*t), sinf(M_PI_4*t)};
	
	glPointSize(4);
	
	glBegin(GL_POINTS);
	glVertex2fv(&x0.x);
	glVertex2fv(&x1.x);
	glEnd();
	
	glBegin(GL_POINTS);
	glVertex2f(x0.x+n0.x, x0.y+n0.y);
	glVertex2f(x1.x+n1.x, x1.y+n1.y);
	glEnd();
	
	glBegin(GL_LINES);
	glVertex2f(x0.x, x0.y);
	glVertex2f(x0.x+n0.x, x0.y+n0.y);
	glVertex2f(x1.x, x1.y);
	glVertex2f(x1.x+n1.x, x1.y+n1.y);
	glEnd();
	
	glBegin(GL_LINES);
	Vec2 x;
	float e;
	for (int i=0; i<10; ++i) {
		e = (float)i/10.0;
		pointNormalInterpolation(&x.x, &x0.x, &n0.x, &x1.x, &n1.x, e);
		glVertex2fv(&x.x);
		
		e = (float)(i+1)/10.0;
		pointNormalInterpolation(&x.x, &x0.x, &n0.x, &x1.x, &n1.x, e);
		glVertex2fv(&x.x);
	}
	glEnd();
	
	glutSwapBuffers();
}

int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 200);
	glutCreateWindow("Hello World!");
	glutIdleFunc(idle);
	glutDisplayFunc(displayCall);
	glutMainLoop();
	return 0;
}
