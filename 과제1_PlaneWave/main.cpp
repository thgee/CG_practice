#include <vgl.h>
#include <InitShader.h>
#include <vec.h>
#include "MyPlain.h"

float gTime = 0.0f;
GLuint prog;
MyPlain plain;



void init()
{
	plain.init();
	prog = InitShader("vShader.glsl", "fShader.glsl");
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	plain.draw(prog, gTime);

	glFlush();
}

void rotation()
{
	gTime += 0.001f;
	glutPostRedisplay();
}



void KeyDown(unsigned char key, int x, int y) { // Ű���带 �Է¹����� ����Ǵ� �Լ�
	static int rot = 0; // rotation ���			0: ����					1: ȸ��
	static int wav = 0; // wave���					0: not waving		1: waving
	glUseProgram(prog);
	GLuint uDiv = glGetUniformLocation(prog, "uDiv"); 
	
	switch (key) {

	case '2':
		plain.division++; // 2���� ������ division�� ����
		plain.init(); // ������ ������Ѵ�
		if (wav == 1) // waving
			glUniform1i(uDiv, plain.division);
		glutPostRedisplay();
		printf("Division: %d, Num.of Triangles : %d, Num.of Vertices : %d\n", plain.division, plain.NUM_POINTS / 3, plain.NUM_POINTS);
		break;

	case '1':
		if (plain.division <= 2) return; // division�� 2 �Ʒ��� �������� �ʵ��� ����ó��
		plain.division--;
		plain.init();
		if (wav == 1) // waving
			glUniform1i(uDiv, plain.division);
		glutPostRedisplay();
		printf("Division: %d, Num.of Triangles : %d, Num.of Vertices : %d\n", plain.division, plain.NUM_POINTS / 3, plain.NUM_POINTS);
		break;

	case ' ': // spaceBar
		if (rot == 0) { // ���������̸�
			glutIdleFunc(rotation); // ȸ�� ����
			glutPostRedisplay();
			rot = 1; // ��� ����
			break;
		}

		if (rot == 1) { // ȸ�������̸�
			glutIdleFunc(NULL); // rotation �Լ��� �������ν� ������Ŵ
			glutPostRedisplay();
			rot = 0; // ��� ����
			break;
		}

	case 'q': // q ������ ���α׷� ����
		exit(0);

	case 'Q': // �빮�ڷ� �ԷµǾ��� ��츦 ����
		exit(0);

	case 'w':

		if (wav == 0) { // not waving
			glUniform1i(uDiv, plain.division);
			wav = 1;
			glutPostRedisplay();
			break;
		}

		if (wav == 1) { // waving
			glUniform1i(uDiv, 0);
			wav = 0;
			glutPostRedisplay();
			break;
		}
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Hello GL");

	glewExperimental = true;
	glewInit();


	printf("����Ʈ�����а� 20003325 ������\n");
	printf("A Waving Color Plain\n\n");
	printf("----------------------------------------------------------------\n\n");
	printf("`1' key: Decreaing the Number of Division\n");
	printf("`2' key : Increaing the Number of Division\n");
	printf("`w' key : Showing / hiding the waving pattern\n");
	printf("Spacebar : starting / stoping rotating and waving\n");
	printf("`Q' Key : Exit the program.\n\n");
	printf("----------------------------------------------------------------\n\n");


	init();

	glutKeyboardFunc(KeyDown);
	glutDisplayFunc(display);
	glutPostRedisplay();

	glutMainLoop();

	return 0;
}