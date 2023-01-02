
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vgl.h>
#include <InitShader.h>
#include "MyCube.h"
#include "MyUtil.h"

#include <vec.h>
#include <mat.h>
#include <list>;

using namespace std;

#define MAZE_FILE	"maze.txt"

typedef struct Pos {
	int x, z;
}pos;

typedef struct Node {
	int x, z; // 인덱스 저장
	int h = 99, f = 99, g = 99;
	pos parent;
}node;
// F = g + h

list<pos> path;

MyCube cube;
GLuint program;

mat4 g_Mat = mat4(1.0f);
GLuint uMat;
GLuint uColor;

float wWidth = 1000;
float wHeight = 500;

vec3 cameraPos = vec3(0, 0, 0);
vec3 viewDirection = vec3(0, 0, -1);
vec3 goalPos = vec3(0, 0, 0);

int MazeSize;
char maze[255][255] = { 0 };

float cameraSpeed = 0.1;

float g_time = 0;

float theta = 0; // 카메라 회전 각도


inline vec3 getPositionFromIndex(int i, int j)
{
	float unit = 1;
	vec3 leftTopPosition = vec3(-MazeSize / 2.0 + unit / 2, 0, -MazeSize / 2.0 + unit / 2);
	vec3 xDir = vec3(1, 0, 0);
	vec3 zDir = vec3(0, 0, 1);
	return leftTopPosition + i * xDir + j * zDir;
}

void LoadMaze()
{
	FILE* file = fopen(MAZE_FILE, "r");
	char buf[255];
	fgets(buf, 255, file);
	sscanf(buf, "%d", &MazeSize);
	for (int j = 0; j < MazeSize; j++)
	{
		fgets(buf, 255, file);
		for (int i = 0; i < MazeSize; i++)
		{
			maze[i][j] = buf[i];
			if (maze[i][j] == 'C')				// Setup Camera Position
				cameraPos = getPositionFromIndex(i, j);
			if (maze[i][j] == 'G')				// Setup Goal Position
				goalPos = getPositionFromIndex(i, j);
		}
	}
	fclose(file);
}


void collision() {
	// cube size : 1
	// camera size : 0.5
	for (int j = 0; j < MazeSize; j++)
		for (int i = 0; i < MazeSize; i++) {
			vec3 wallPos = getPositionFromIndex(i, j);
			if (maze[i][j] == '*') // 벽에 한하여 적용
				// 충돌조건
				if (cameraPos.x + 0.25 >= wallPos.x - 0.5 && cameraPos.x - 0.25 <= wallPos.x + 0.5 && cameraPos.z + 0.25 >= wallPos.z - 0.5 && cameraPos.z - 0.25 <= wallPos.z + 0.5) {

					// 벽의 왼쪽에서 부딪히면
					if (abs((cameraPos.x + 0.25) - (wallPos.x - 0.5)) < 0.1)	cameraPos.x -= 0.1f;
					// 벽의 오른쪽에서 부딪히면
					if (abs((cameraPos.x - 0.25) - (wallPos.x + 0.5)) < 0.1)	cameraPos.x += 0.1f;
					// 벽의 아래쪽에서 부딪히면
					if (abs((cameraPos.z - 0.25) - (wallPos.z + 0.5)) < 0.1)	cameraPos.z += 0.1f;
					// 벽의 위쪽에서 부딪히면
					if (abs((cameraPos.z + 0.25) - (wallPos.z - 0.5)) < 0.1)	cameraPos.z -= 0.1f;
				}
		}
}


void DrawMaze()
{
	for (int j = 0; j < MazeSize; j++)
		for (int i = 0; i < MazeSize; i++)
			if (maze[i][j] == '*')
			{
				vec3 wallPos = getPositionFromIndex(i, j);
				vec3 color;
				// 충돌한 큐브는 빨간색
				if (cameraPos.x + 0.25 >= wallPos.x - 0.5 && cameraPos.x - 0.25 <= wallPos.x + 0.5 && cameraPos.z + 0.25 >= wallPos.z - 0.5 && cameraPos.z - 0.25 <= wallPos.z + 0.5)
					color = vec3(1, 0, 0);
				else
					color = vec3(i / (float)MazeSize, j / (float)MazeSize, 1);
				mat4 ModelMat = Translate(getPositionFromIndex(i, j));

				glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
				glUniform4f(uColor, color.x, color.y, color.z, 1);


				cube.Draw(program);


			}

}


void myInit()
{
	LoadMaze();
	cube.Init();
	program = InitShader("vshader.glsl", "fshader.glsl");

}

void DrawGrid()
{

	float n = 40;
	float w = MazeSize; // MazeSize == 20
	float h = MazeSize;

	for (int i = 0; i < n; i++) // 가로줄
	{
		mat4 m = Translate(0, -0.5, -h / 2 + h / n * i) * Scale(w, 0.02, 0.02);
		glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
		glUniform4f(uColor, 1, 1, 1, 1);
		cube.Draw(program);
	}
	for (int i = 0; i < n; i++) // 세로줄
	{
		mat4 m = Translate(-w / 2 + w / n * i, -0.5, 0) * Scale(0.02, 0.02, h);
		glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
		glUniform4f(uColor, 1, 1, 1, 1);
		cube.Draw(program);
	}
}


void drawCamera()
{
	float cameraSize = 0.5;

	mat4 ModelMat = Translate(cameraPos) * RotateY(theta) * Scale(vec3(cameraSize)); // 카메라 몸통
	// RotateY로 'A' 'D'를 눌렀을 때 회전한 각만큼 카메라를 회전시켜주어야 한다
	glUseProgram(program);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	glUniform4f(uColor, 0, 1, 0, 1);
	cube.Draw(program);

	ModelMat = Translate(cameraPos + viewDirection * cameraSize / 2) * RotateY(theta) * Scale(vec3(cameraSize / 2)); // 카메라 렌즈
	glUseProgram(program);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	glUniform4f(uColor, 0, 1, 0, 1);
	cube.Draw(program);
}

void drawGoal()
{
	glUseProgram(program);
	float GoalSize = 0.7;

	mat4 ModelMat = Translate(goalPos) * RotateY(g_time * 3) * Scale(vec3(GoalSize));
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	glUniform4f(uColor, 0, 0, 0, 0);
	cube.Draw(program);

	ModelMat = Translate(goalPos) * RotateY(g_time * 3 + 45) * Scale(vec3(GoalSize));
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	glUniform4f(uColor, 0, 0, 0, 0);
	cube.Draw(program);
}

void drawPath() {
	pos tmp{100, 100};
	mat4 ModelMat;
	for (pos i : path) {
		vec3 color = vec3(1, cos(0.2 * g_time), sin(0.2 * g_time));
		if (tmp.x == i.x) {
			if (tmp.z > i.z) // 위에서 아래로 이동
				ModelMat = Translate(0, -0.5, -0.5) * Translate(getPositionFromIndex(tmp.x, tmp.z)) * Scale(0.13, 0.13, 1);
			if (tmp.z < i.z) // 아래에서 위로 이동
				ModelMat = Translate(0, -0.5, 0.5) * Translate(getPositionFromIndex(tmp.x, tmp.z)) * Scale(0.13, 0.13, 1);
		}
		if (tmp.z == i.z) {
			if (tmp.x < i.x) // 왼쪽에서 오른쪽으로 이동
				ModelMat = Translate(0.5, -0.5, 0) * Translate(getPositionFromIndex(tmp.x, tmp.z)) * Scale(1, 0.13, 0.13);
			if (tmp.x > i.x) // 오른쪽에서 왼쪽으로 이동
				ModelMat = Translate(-0.5, -0.5, 0) * Translate(getPositionFromIndex(tmp.x, tmp.z)) * Scale(1, 0.13, 0.13);
		}
		if (tmp.x == 100, tmp.z == 100) { tmp = i; continue; }
		glUniform4f(uColor, color.x, color.y, color.z, 1);
		glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
			tmp = i;

		cube.Draw(program);
	}
}

void drawScene(bool bDrawCamera = true)
{
	glUseProgram(program);
	uMat = glGetUniformLocation(program, "uMat");
	uColor = glGetUniformLocation(program, "uColor");

	DrawGrid();
	DrawMaze();

	drawGoal();

	drawPath(); // 경로 그리기

	if (bDrawCamera)
		drawCamera();

}



void cal(node& start, node& cur, node& goal) {
	cur.g = abs(start.x - cur.x) + abs(start.z - cur.z);
	cur.h = abs(cur.x - goal.x) + abs(cur.z - goal.z);
	cur.f = cur.g + cur.h;
}



void storepath(node start, node goal, list<node> closedSet) {

	path.clear();
	pos tmp = { goal.x, goal.z };
	path.push_back(tmp);
	tmp = goal.parent;

	while(1){
		for (node i : closedSet) {
			if (tmp.x == i.x && tmp.z == i.z) { // closedSet에서 부모노드를 찾으면
				tmp.x = i.x;
				tmp.z = i.z;
				path.push_back(tmp);
				if (tmp.x == start.x && tmp.z == start.z) {
					drawPath();
					return; // 시작점까지 리스트에 넣었으면 종료
				}
				tmp = i.parent;
				break;
			}
		}
	}
}




void findpath() {

	list<node> closedSet;
	list<node> openSet;

	// ----------- 카메라 좌표로부터 start 노드 인덱스 계산 ----------
	node start;
	for (int j = 0; j < MazeSize; j++)
		for (int i = 0; i < MazeSize; i++)
			if (cameraPos.x > -MazeSize / 2.0 + i && cameraPos.x <= -MazeSize / 2.0 + 1 + i && cameraPos.z > -MazeSize / 2.0 + j && cameraPos.z <= -MazeSize / 2.0 + 1 + j) {
				start.x = i;
				start.z = j;
			}

	// --------------- 도착지점 노드 저장 ---------------
	node goal;
	for (int j = 0; j < MazeSize; j++)
		for (int i = 0; i < MazeSize; i++)
			if (maze[i][j] == 'G') { // 도착 지점
				goal.x = i; goal.z = j;
			}


	cal(start, start, goal);
	openSet.push_back(start); // 열린목록에 start노드 추가
	// 부모노드를 따라 올라가면서 결과노드 리스트를 만들어 그곳에 저장
	// 노드마다 인덱스 -> 좌표 변환 후 해당 좌표에 큐브로 점 찍기

	while (1) {

		node cur;

		int minF = openSet.front().f;
		for (node n : openSet)
			if (n.f <= minF) {
				minF = n.f;
				cur = n;
			}

		cur = openSet.back();
		openSet.pop_back();
		closedSet.push_back(cur);

		for (int j = cur.z - 1; j <= cur.z + 1; j++) // 현재 노드를 기준으로 위아래, 양옆 노트 탐색
			for (int i = cur.x - 1; i <= cur.x + 1; i++) {
				if (j == cur.z - 1 && i == cur.x - 1 ||
						j == cur.z - 1 && i == cur.x + 1 ||
						j == cur.z + 1 && i == cur.x - 1 ||
						j == cur.z + 1 && i == cur.x + 1 ||
						j == cur.z && i == cur.x) 
						continue;


				node neighbor;
				neighbor.x = i; neighbor.z = j;
				cal(start, neighbor, goal);
				if (maze[i][j] == '*') 
					continue; // 해당 노드가 벽이면 무시



				bool flag = false;
				for (node n : closedSet)
					if (n.x == i && n.z == j) flag = true; // 닫힌목록에 들어있는 노드이면 무시
				if (flag == true) continue;

				flag = false;
				for (node n : openSet)
					if (n.x == i && n.z == j) {  // 탐색중인 노드가 열린목록에 있다면
						flag = true;
					}
				if (flag == true) continue;


				// 탐색중인 노드가 열린목록에 없다면

				neighbor.parent.x = cur.x; // 부모노드 설정
				neighbor.parent.z = cur.z; // 부모노드 설정
				openSet.push_back(neighbor); // 이 노드를 열린목록에 추가
				for (node n : openSet)
					if (n.x == goal.x && n.z == goal.z) { // openSet에 목적 노드가 들어오면
						storepath(start, n, closedSet);
						return;
					}
				
			}

	}
}






void display()
{
	glEnable(GL_DEPTH_TEST);

	float vWidth = wWidth / 2;
	float vHeight = wHeight;

	// LEFT SCREEN : View From Camera (Perspective Projection)
	glViewport(0, 0, vWidth, vHeight);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	float h = 4;
	float aspectRatio = vWidth / vHeight;
	float w = aspectRatio * h;
	mat4 ViewMat = myLookAt(cameraPos, cameraPos + viewDirection, vec3(0, 1, 0));
	mat4 ProjMat = myPerspective(45, aspectRatio, 0.01, 20);

	g_Mat = ProjMat * ViewMat;
	drawScene(false);							// drawing scene except the camera


	// RIGHT SCREEN : View from above (Orthographic parallel projection)
	glViewport(vWidth, 0, vWidth, vHeight);
	h = MazeSize;
	w = aspectRatio * h;
	ViewMat = myLookAt(vec3(0, 5, 0), vec3(0, 0, 0), vec3(0, 0, -1)); // y = 5 높이에서 미로를 내려다보게 함
	ProjMat = myOrtho(-w / 2, w / 2, -h / 2, h / 2, 0, 20);

	g_Mat = ProjMat * ViewMat;
	drawScene(true);


	glutSwapBuffers();
}



void idle()
{

	g_time += 1;

	collision();


	if ((GetAsyncKeyState('A') & 0x8000) == 0x8000) {		// if "A" key is pressed	: Go Left
		theta += 3.0f;
		float rad = 3.141592 / 180 * theta;
		viewDirection = vec3(-sin(rad), 0, -cos(rad)); // 카메라 transform에서 카메라가 바라보는 focus 지점이 카메라를 중심으로 원형으로 돌아가도록 한다
	}
	if ((GetAsyncKeyState('D') & 0x8000) == 0x8000) {		// if "D" key is pressed	: Go Right
		theta -= 3.0f;
		float rad = 3.141592 / 180 * theta;
		viewDirection = vec3(-sin(rad), 0, -cos(rad));
	}
	if ((GetAsyncKeyState('W') & 0x8000) == 0x8000)		// if "W" key is pressed	: Go Forward
		cameraPos += cameraSpeed * viewDirection;

	if ((GetAsyncKeyState('S') & 0x8000) == 0x8000)		// if "S" key is pressed	: Go Backward
		cameraPos += cameraSpeed * -viewDirection;

	if ((GetAsyncKeyState('Q') & 0x8000) == 0x8000)		// Q 누르면 길찾기
		findpath();


	if ((GetAsyncKeyState(' ') & 0x8000) == 0x8000) {
				list<pos> copy = path;
			while (copy.size() != 0) {
				int pre_turn = 0, next_turn = 0;
				cameraPos = getPositionFromIndex(copy.back().x, copy.back().z);

				vec3 tmp = vec3(100, 0, 100);
				while (copy.size() >= 1) {
					vec3 i = getPositionFromIndex(copy.back().x, copy.back().z);
					copy.pop_back();

					if (tmp.x == i.x) {
						if (tmp.z > i.z) {  // 아래에서 위로 이동
							viewDirection = vec3(0, 0, -1);
							theta = 0;
							for (int i = 0; i < 10; i++) {
								cameraPos += vec3(0, 0, -0.1);
								display();
								Sleep(10);
							}
							next_turn = 1;
						}

						if (tmp.z < i.z) { // 위에서 아래로 이동
							viewDirection = vec3(0, 0, 1);
							theta = 180;

							for (int i = 0; i < 10; i++) {
								cameraPos += vec3(0, 0, 0.1);
								display();
								Sleep(10);
							}
							next_turn = 2;
						}
					}
					if (tmp.z == i.z) {
						if (tmp.x < i.x) { // 왼쪽에서 오른쪽으로 이동
							viewDirection = vec3(1, 0, 0);
							theta = -90;

							for (int i = 0; i < 10; i++) {
								cameraPos += vec3(0.1, 0, 0);
								display();
								Sleep(10);
							}
							next_turn = 3;
						}
						if (tmp.x > i.x) {
							for (int i = 0; i < 10; i++) { // 오른쪽에서 왼쪽으로 이동
								viewDirection = vec3(-1, 0, 0);
								theta = 90;

								cameraPos += vec3(-0.1, 0, 0);
								display();
								Sleep(10);
							}
							next_turn = 4;

						}
					}

					// ------------- 방향이 바뀌면 카메라 시점 변환 ------------

						if (pre_turn == 1 && next_turn == 3) 
								viewDirection = vec3(1, 0, 0);
							
						if (pre_turn == 1 && next_turn == 4) 
							viewDirection = vec3(-1, 0, 0);
						 
						if (pre_turn == 2 && next_turn == 3)  							
							for (int i = 0; i < 10; i++) 
								viewDirection = vec3(-1, 0, 0);
						
						if (pre_turn == 2 && next_turn == 4)  
							viewDirection = vec3(1, 0, 0);
		
						if (pre_turn == 3 && next_turn == 1) 
							viewDirection = vec3(0, 0, -1);

						
						if (pre_turn == 3 && next_turn == 2)
							viewDirection = vec3(0, 0, 1);

						if (pre_turn == 4 && next_turn == 1)  
							viewDirection = vec3(0, 0, -1);

						if (pre_turn == 4 && next_turn == 2)
							viewDirection = vec3(0, 0, 1);

					pre_turn = next_turn;
					if (tmp.x == 100, tmp.z == 100) { tmp = i; continue; }
					tmp = i;
				}
			}


	}

	Sleep(16);											// for vSync
	glutPostRedisplay();
}

void reshape(int wx, int wy)
{
	printf("%d %d \n", wx, wy);
	wWidth = wx;
	wHeight = wy;
	glutPostRedisplay();
}


int main(int argc, char** argv)
{

	cout << "소프트웨어학과 20003325 이태혁" << endl;
	cout << "A Maze Navigator" << endl;
		cout << "Programming Assignment #3 for Computer Graphics.Department of Software, Sejong University" << endl << endl;

		cout << "----------------------------------------------------------------" << endl;
		cout << "`W' key: Go Forward" << endl;
		cout << "`S' key : Go Backward" << endl;
		cout << "`A' key : Turn Left" << endl;
		cout << "`D' key : Turn Right" << endl;
		cout << "`Q' key : Find Shortest Path to the Goal" << endl;
		cout << "`Spacebar' : Start following the Path" << endl << endl;

		cout << "----------------------------------------------------------------" << endl;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(wWidth, wHeight);

	glutCreateWindow("Homework3 (Maze Navigator)");

	glewExperimental = true;
	glewInit();

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	myInit();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}