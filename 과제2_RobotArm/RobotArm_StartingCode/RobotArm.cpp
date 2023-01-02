#include <list>
#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include "MyCube.h"
#include "MyPyramid.h"
#include "MyTarget.h"

MyCube cube;
MyPyramid pyramid;
MyTarget target(&cube);

GLuint program;
GLuint uMat;
std::list<mat4> CTMstack;

mat4 CTM;

bool bPlay = false;
bool bChasingTarget = false;
bool bDrawTarget = false;

float ang1 = 0;
float ang2 = 0;
float ang3 = 0;

void myInit()
{
	cube.Init();
	pyramid.Init();

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
}

float g_time = 0;

void drawRobotArm(float ang1, float ang2, float ang3)
{

	mat4 M(1.0);

	CTM = Translate(0, -0.4, 0) * RotateY(g_time * 30);
	mat4 temp = CTM; // target의 원점 세팅을 위해 저장


	// Base
	{
		// Left Base
		M = Translate(0, 0, 0.2) * Scale(0.4, 0.3, 0.03);
		glUniformMatrix4fv(uMat, 1, true, CTM * M);
		cube.Draw(program);

		// Right Base
		M = Translate(0, 0, -0.2) * Scale(0.4, 0.3, 0.03);
		glUniformMatrix4fv(uMat, 1, true, CTM * M);
		cube.Draw(program);

		// Middle Base
		CTM *= RotateZ(ang1);
		M = Scale(0.4, 0.1, 0.4);
		glUniformMatrix4fv(uMat, 1, true, CTM * M);
		cube.Draw(program);

	}


	// Upper Arm
	{

		// Pillar 1
		{
			CTMstack.push_back(CTM);
			CTM *= Translate(0.1, 0, 0.1);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);

			CTMstack.push_back(CTM);
			CTM *= Translate(0, 0.2, 0);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);
		}

		CTMstack.pop_back();
		CTM = CTMstack.back();

		// Pillar 2
		{
			CTM *= Translate(-0.1, 0, 0.1);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);

			CTMstack.push_back(CTM);
			CTM *= Translate(0, 0.2, 0);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);
		}


		CTMstack.pop_back();
		CTM = CTMstack.back();

		// Pillar 3
		{
			CTM *= Translate(-0.1, 0, -0.1);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);

			CTMstack.push_back(CTM);
			CTM *= Translate(0, 0.2, 0);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);
		}

		CTMstack.pop_back();
		CTM = CTMstack.back();

		// Pillar 4
		{
			CTM *= Translate(0.1, 0, -0.1);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);

			CTMstack.push_back(CTM);
			CTM *= Translate(0, 0.2, 0);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);
		}

		CTMstack.pop_back();
		CTM = CTMstack.back();


		// Main Pillar
		M = Translate(0, 0.2, 0) * Scale(0.1, 0.4, 0.1);
		glUniformMatrix4fv(uMat, 1, true, CTM * M);
		cube.Draw(program);

	}

	// Upper Joint
	{
		{
			// Left Joint
			CTM *= Translate(0, 0.4, 0);
			M = Translate(0.1, 0, 0) * Scale(0.1, 0.1, 0.3);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			cube.Draw(program);

			// Left Board
			M = Translate(0, 0, 0.15) * Scale(0.4, 0.2, 0.03);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			cube.Draw(program);
		}

		CTM = CTMstack.back();

		{
			// Right Joint
			CTM *= Translate(0, 0.4, 0);
			M = Translate(-0.1, 0, 0) * Scale(0.1, 0.1, 0.3);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			cube.Draw(program);

			// Right Board
			M = Translate(0, 0, -0.15) * Scale(0.4, 0.2, 0.03);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			cube.Draw(program);
		}
	}


	// Lower Arm
	{

		// Pillar 1
		{
			CTMstack.push_back(CTM);
			CTM *= Translate(0.1, 0, 0.1) * RotateZ(ang2);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);

			CTMstack.push_back(CTM);
			CTM *= Translate(0, 0.2, 0);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);
		}

		CTMstack.pop_back();
		CTM = CTMstack.back();

		// Pillar 2
		{
			CTM *= Translate(-0.1, 0, 0.1) * RotateZ(ang2);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);

			CTMstack.push_back(CTM);
			CTM *= Translate(0, 0.2, 0);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);
		}


		CTMstack.pop_back();
		CTM = CTMstack.back();

		// Pillar 3
		{
			CTM *= Translate(-0.1, 0, -0.1) * RotateZ(ang2);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);

			CTMstack.push_back(CTM);
			CTM *= Translate(0, 0.2, 0);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);
		}

		CTMstack.pop_back();
		CTM = CTMstack.back();

		// Pillar 4
		{
			CTM *= Translate(0.1, 0, -0.1) * RotateZ(ang2);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);

			CTMstack.push_back(CTM);
			CTM *= Translate(0, 0.2, 0);
			M = Translate(0, 0.1, 0) * Scale(0.05, 0.2, 0.05);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			pyramid.Draw(program);
		}

		CTMstack.pop_back();
		CTM = CTMstack.back();


		// Main Pillar
		CTM *= RotateZ(ang2);
		M = Translate(0, 0.2, 0) * Scale(0.05, 0.4, 0.05);
		glUniformMatrix4fv(uMat, 1, true, CTM * M);
		cube.Draw(program);

	}

	// Lower Joint
	{
		{
			// Left Joint
			CTMstack.push_back(CTM);
			CTM *= Translate(0, 0.4, 0);
			M = Translate(0.1, 0, 0) * Scale(0.1, 0.1, 0.3);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			cube.Draw(program);

			// Left Board
			M = Translate(0, 0, 0.15) * Scale(0.4, 0.2, 0.03);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			cube.Draw(program);
		}

		{
			// Right Joint
			CTM = CTMstack.back();
			CTM *= Translate(0, 0.4, 0);
			M = Translate(-0.1, 0, 0) * Scale(0.1, 0.1, 0.3);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			cube.Draw(program);

			// Right Board
			M = Translate(0, 0, -0.15) * Scale(0.4, 0.2, 0.03);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			cube.Draw(program);
		}
	}

	CTM = CTMstack.back();

	// Pointer
	CTM *= Translate(0, 0.4, 0) * RotateZ(ang3);
	M = Translate(0, 0.2, 0) * Scale(0.08, 0.4, 0.08);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	pyramid.Draw(program);

	CTM = temp;
}

void computeAngle()
{
	vec3 target_pos = target.GetPosition(g_time);
	vec3 robot_pos;
	float dist = 0.0f;

	for (int i = 0; i < 360; i += 2) {
		for (int j = 0, k = 0; j < 120; j += 2, k += 2) {
			robot_pos.x = 0.4 * -sin(3.141592 / 180 * i) + 0.4 * -sin(3.141592 / 180 * (i + j)) + 0.5 * -sin(3.141592 / 180 * (i + j + k));
			robot_pos.y = 0.4 * cos(3.141592 / 180 * i) + 0.4 * cos(3.141592 / 180 * (i + j)) + 0.5 * cos(3.141592 / 180 * (i + j + k));
			dist = sqrt((robot_pos.x - target_pos.x) * (robot_pos.x - target_pos.x) + (robot_pos.y - target_pos.y) * (robot_pos.y - target_pos.y));
			if (dist < 0.1)
			{
				ang1 = i;
				ang2 = j;
				ang3 = k;
				break;
			}
		}
	}
}


void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint uColor = glGetUniformLocation(program, "uColor");
	glUniform4f(uColor, -1, -1, -1, -1);


	uMat = glGetUniformLocation(program, "uMat");
	drawRobotArm(ang1, ang2, ang3);


	glUniform4f(uColor, 1, 0, 0, 1);
	if (bDrawTarget == true)
		target.Draw(program, CTM, g_time);

	glutSwapBuffers();
}

void myIdle()
{
	if (bPlay)
	{
		g_time += 1 / 60.0f;
		Sleep(1 / 60.0f * 1000);

		if (bChasingTarget == false)
		{
			ang1 = 45 * sin(g_time * 3.141592);
			ang2 = 60 * sin(g_time * 2 * 3.141592);
			ang3 = 30 * sin(g_time * 3.141592);
		}
		else
			computeAngle();

		glutPostRedisplay();
	}
}

void myKeyboard(unsigned char c, int x, int y)
{

	switch (c)
	{
	case '1':
		bChasingTarget = !bChasingTarget;
		break;
	case '2':
		bDrawTarget = !bDrawTarget;
		break;
	case '3':
		target.toggleRandom();
		break;
	case ' ':
		bPlay = !bPlay;
		break;
	default:
		break;
	}
}


int main(int argc, char** argv)
{
	std::cout << "소프트웨어학과 20003325 이태혁" << std::endl;
	std::cout << "SIMPLE INTELLIGENT ROBOT ARM" << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "`1' key: Toggle Between Chasing the target or not" << std::endl;
	std::cout << "`2' key : Toggle Between Showing the target position or not" << std::endl;
	std::cout << "`3' key : Toggle Between Randomly Positioning the target or not" << std::endl << std::endl;

	std::cout << "Spacebar: start / stop Playing" << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Simple Robot Arm");

	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);

	glutMainLoop();





	return 0;
}