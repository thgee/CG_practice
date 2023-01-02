#define _CRT_SECURE_NO_WARNINGS

#include <vgl.h>
#include <InitShader.h>
#include "MyShape.h"

#include <vec.h>
#include <mat.h>

float shiness = 10;
float specNum = 0.8f;

MyShape shape;
GLuint program;
GLuint prog_phong;

GLuint uMat;
GLuint uColor;


int winWidth = 500;
int winHeight = 500;

bool bPlay = false;


bool bMoouseLeft = false;
bool bMoouseMid = true;
bool bMoouseRight = false;

mat4 g_Mat = mat4(1.0f);
mat4 CMT = mat4(1.0f);
mat4 ModelMat;

mat4 myLookAt(vec3 eye, vec3 at, vec3 up)
{
	// Implement your own look-at function
	mat4 V(1.0f);
	vec3 n = at - eye;
	n /= length(n);

	float a = dot(up, n);
	vec3 v = up - a * n;
	v /= length(v);

	vec3 w = cross(n, v);

	mat4 Rw(1.0f);

	Rw[0][0] = w.x;	Rw[0][1] = v.x; Rw[0][2] = -n.x;
	Rw[1][0] = w.y;	Rw[1][1] = v.y; Rw[1][2] = -n.y;
	Rw[2][0] = w.z;	Rw[2][1] = v.z; Rw[2][2] = -n.z;

	mat4 Rc(1.0f);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Rc[i][j] = Rw[j][i];

	mat4 Tc = Translate(-eye.x, -eye.y, -eye.z);

	V = Rc * Tc;

	return V;
}


mat4 myPerspective(float fovy, float aspectRatio, float zNear, float zFar)
{
	mat4 P(1.0f);

	float rad = fovy * 3.141592 / 180.0;

	float sz = 1 / zFar;
	float h = zFar * tan(rad / 2);

	float sy = 1 / h;
	float w = h * aspectRatio;
	float sx = 1 / w;

	mat4 S = Scale(sx, sy, sz);
	mat4 M(1.0f);

	float c = -zNear / zFar;
	M[2][2] = 1 / (c + 1);
	M[2][3] = -c / (c + 1);
	M[3][2] = -1;
	M[3][3] = 0;

	P = M * S;

	return P;
}


void myInit()
{
	shape.Init();

	program = InitShader("vshader.glsl", "fshader.glsl");
	prog_phong = InitShader("vphong.glsl", "fphong.glsl");
}



float g_Time = 0;

void display()
{

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	mat4 ViewMat = myLookAt(vec3(2, 2, 2), vec3(0, 0, 0), vec3(0, 1, 0));

	float aspect = winWidth / (float)winHeight;
	float h = 1;

	mat4 ProjMat = myPerspective(40, aspect, 0.1, 100);

	g_Mat = ProjMat * ViewMat;

	glUseProgram(program);
	uMat = glGetUniformLocation(program, "uMat");
	uColor = glGetUniformLocation(program, "uColor");


	glUseProgram(prog_phong);
	GLuint uProjMat = glGetUniformLocation(prog_phong, "uProjMat");
	GLuint uModelMat = glGetUniformLocation(prog_phong, "uModelMat");

	// 마우스 버튼에 따른 물체 회전
	if (bMoouseLeft) {
		ModelMat = RotateX(g_Time * 30) * CMT;
	}
	if (bMoouseMid)	{

		ModelMat = RotateY(g_Time * 30) * CMT;
}
	if (bMoouseRight)	{

		ModelMat = RotateZ(g_Time * 30) * CMT;
	}

	glUniformMatrix4fv(uProjMat, 1, GL_TRUE, ProjMat);
	glUniformMatrix4fv(uModelMat, 1, GL_TRUE, ViewMat  * ModelMat);

	// 1. define lights : Position, Color(Intensity)
	vec4 lpos = vec4(3, 3, 3, 1);	// in World Coord.
	vec4 lcol = vec4(1, 1, 1, 1);	// white 

	// 2. material properties (phong coeff.)
	vec4 mAmb = vec4(0.2, 0.2, 0.2, 1);
	vec4 mDif = vec4(0.4, 0.2, 0.2, 1);
	vec4 mSpec = vec4(specNum, specNum, specNum, 1);

	GLuint uLPos = glGetUniformLocation(prog_phong, "uLPos");
	GLuint uLCol = glGetUniformLocation(prog_phong, "uLCol");
	GLuint uAmb = glGetUniformLocation(prog_phong, "uAmb");
	GLuint uDif = glGetUniformLocation(prog_phong, "uDif");
	GLuint uSpec = glGetUniformLocation(prog_phong, "uSpec");
	GLuint uShiness = glGetUniformLocation(prog_phong, "uShiness");

	glUniform4f(uLPos, lpos[0], lpos[1], lpos[2], lpos[3]);
	glUniform4f(uLCol, lcol[0], lcol[1], lcol[2], lcol[3]);
	glUniform4f(uAmb, mAmb[0], mAmb[1], mAmb[2], mAmb[3]);
	glUniform4f(uDif, mDif[0], mDif[1], mDif[2], mDif[3]);
	glUniform4f(uSpec, mSpec[0], mSpec[1], mSpec[2], mSpec[3]);
	glUniform1f(uShiness, shiness);

	shape.Draw(prog_phong);

	glutSwapBuffers();
}


void idle()
{
	if (bPlay)
		g_Time += 0.016;
	Sleep(16);					// for vSync
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glutPostRedisplay();
}

void myMouse(int btn, int states, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && states == GLUT_DOWN) {
		g_Time = 0.0f;
		CMT = ModelMat;
		bMoouseLeft = true;
		bMoouseMid = false;
		bMoouseRight = false;

	}
	if (btn == GLUT_MIDDLE_BUTTON && states == GLUT_DOWN) {
		g_Time = 0.0f;
		CMT = ModelMat;
		bMoouseLeft = false;
		bMoouseMid = true;
		bMoouseRight = false;
	}
	if (btn == GLUT_RIGHT_BUTTON && states == GLUT_DOWN) {
		g_Time = 0.0f;
		CMT = ModelMat;
		bMoouseLeft = false;
		bMoouseMid = false;
		bMoouseRight = true;
	}
}
void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case '3':
		printf("Increasing Specular Effect!\n");
		if (specNum >= 1.0f) break;
		specNum += 0.03f;
		break;
	case '4':
		printf("Decreasing Specular Effect!\n");
		if (specNum <= 0.02f) break;
		specNum -= 0.03f;
		break;
	case '5':
		printf("increasing shiness!\n");
		if (shiness >= 50) break;
		shiness += 2;
		break;
	case '6':
		printf("decreasing shiness!\n");
		if (shiness <= 2) break;
		shiness -= 2;
		break;
	case ' ':
		if (bPlay) printf("Stop!\n");
		else printf("Play!\n");
		bPlay = !bPlay;
		break;
	case 'q':
		exit(0);
	case 'Q':
		exit(0);
	default:
		break;
	}

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);

	glutCreateWindow("OpenGL");

	glewExperimental = true;
	glewInit();


	myInit();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(myMouse);
	glutMainLoop();

	return 0;
}
