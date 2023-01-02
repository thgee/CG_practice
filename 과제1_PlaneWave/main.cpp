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



void KeyDown(unsigned char key, int x, int y) { // 키보드를 입력받으면 실행되는 함수
	static int rot = 0; // rotation 모드			0: 정지					1: 회전
	static int wav = 0; // wave모드					0: not waving		1: waving
	glUseProgram(prog);
	GLuint uDiv = glGetUniformLocation(prog, "uDiv"); 
	
	switch (key) {

	case '2':
		plain.division++; // 2번을 누르면 division이 증가
		plain.init(); // 점들을 재생성한다
		if (wav == 1) // waving
			glUniform1i(uDiv, plain.division);
		glutPostRedisplay();
		printf("Division: %d, Num.of Triangles : %d, Num.of Vertices : %d\n", plain.division, plain.NUM_POINTS / 3, plain.NUM_POINTS);
		break;

	case '1':
		if (plain.division <= 2) return; // division이 2 아래로 내려가지 않도록 예외처리
		plain.division--;
		plain.init();
		if (wav == 1) // waving
			glUniform1i(uDiv, plain.division);
		glutPostRedisplay();
		printf("Division: %d, Num.of Triangles : %d, Num.of Vertices : %d\n", plain.division, plain.NUM_POINTS / 3, plain.NUM_POINTS);
		break;

	case ' ': // spaceBar
		if (rot == 0) { // 정지상태이면
			glutIdleFunc(rotation); // 회전 시작
			glutPostRedisplay();
			rot = 1; // 모드 변경
			break;
		}

		if (rot == 1) { // 회전상태이면
			glutIdleFunc(NULL); // rotation 함수를 빼줌으로써 정지시킴
			glutPostRedisplay();
			rot = 0; // 모드 변경
			break;
		}

	case 'q': // q 누르면 프로그램 종료
		exit(0);

	case 'Q': // 대문자로 입력되었을 경우를 방지
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


	printf("소프트웨어학과 20003325 이태혁\n");
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