#pragma once

#include <vgl.h>
#include <vec.h>
#include <mat.h>

struct MyVertex
{
	vec4 points;
	vec4 colors;
	vec3 normals;
};

char path[100];

int NumVertices = 0;
int planeNum = 0;

vec4* vertex;
struct MyVertex* myVertices; // ����ü �����迭, ������ ���� ������ ���� (planeNum * 3)

class MyShape
{
public:
	MyShape(void);
	~MyShape(void);

	void readV(char input[], vec4 arr[], int index);
	void readF(char input[], struct MyVertex vertices[], int* vertices_index);

	GLuint Init();
	void SetPositionAndColorAttribute(GLuint program);

	int NumCurVertices;

	GLuint vao;
	GLuint buffer;
	bool bInitialized;

	void Draw(GLuint program);
};

void MyShape::readV(char input[], vec4 arr[], int index) {

	for (int i = 2, start = 2, mode = 1; i < strlen(input); i++) {

		char tmp[100] = { 0 };
		arr[index].w = 1.0f;

		if (input[i] == ' ' && mode == 1) {
			strncpy(tmp, &input[start], (i - start));
			arr[index].x = atof(tmp);
			start = i + 1;
			mode = 2;
			continue;
		}
		if (input[i] == ' ' && mode == 2) {
			strncpy(tmp, &input[start], (i - start));
			arr[index].y = atof(tmp);
			start = i + 1;
			mode = 3;
			continue;
		}
		if (input[i] == '\n' && mode == 3) {
			strncpy(tmp, &input[start], (i - start));
			arr[index].z = atof(tmp);
			return;
		}
	}
}

void MyShape::readF(char input[], struct MyVertex vertices[], int* vertices_index) {
	for (int i = 2, start = 2, mode = 1; i < strlen(input); i++) {

		char tmp[100] = { 0 };
		if (input[i] == ' ' && mode == 1) {
			strncpy(tmp, &input[start], (i - start));
			vertices[*vertices_index].points.x = vertex[atoi(tmp) - 1].x;
			vertices[*vertices_index].points.y = vertex[atoi(tmp) - 1].y;
			vertices[*vertices_index].points.z = vertex[atoi(tmp) - 1].z;
			vertices[*vertices_index].points.w = 1.0f;

			(*vertices_index)++;
			start = i + 1;
			mode = 2;
			continue;
		}
		if (input[i] == ' ' && mode == 2) {
			strncpy(tmp, &input[start], (i - start));
			vertices[*vertices_index].points.x = vertex[atoi(tmp) - 1].x;
			vertices[*vertices_index].points.y = vertex[atoi(tmp) - 1].y;
			vertices[*vertices_index].points.z = vertex[atoi(tmp) - 1].z;
			vertices[*vertices_index].points.w = 1.0f;
			(*vertices_index)++;
			start = i + 1;
			mode = 3;
			continue;
		}
		if (input[i] == '\n' && mode == 3) {
			strncpy(tmp, &input[start], (i - start));
			vertices[*vertices_index].points.x = vertex[atoi(tmp) - 1].x;
			vertices[*vertices_index].points.y = vertex[atoi(tmp) - 1].y;
			vertices[*vertices_index].points.z = vertex[atoi(tmp) - 1].z;
			vertices[*vertices_index].points.w = 1.0f;
			(*vertices_index)++;
			return;
		}

	}
}


MyShape::MyShape(void)
{
	bInitialized = false;
	NumCurVertices = 0;
}

MyShape::~MyShape(void)
{
}




GLuint MyShape::Init()
{
	printf("-----------------------------------------------------------------------------------\n");
	printf("Spacebar: starting / stoping rotation\n");
	printf("\n");
	printf("Left Mouse Button : rotating around x - axis\n");
	printf("Middle Mouse Button : rotating around y - axis\n");
	printf("Right Mouse Button : rotating around z - axis\n");
	printf("\n");
	printf("`1' key : Using Vertex Normal for shading\n");
	printf("`2' key : Using Surface Normal for shading\n");
	printf("`3' key : Increasing Specular effect(ks)\n");
	printf("`4' key : Decreasing Specular effect(ks)\n");
	printf("`5' key : Increasing Shininess(n)\n");
	printf("`6' key : Decreasing Shininess(n)\n");
	printf("`Q' Key : Exit the program.\n");
	printf("-----------------------------------------------------------------------------------\n");
	printf("\n");
	printf("\n");

	printf("-----------------------------------------------------------------------------------\n");
	printf("!!! Read me !!!\n");
	printf("ũ�� ������ ��ġ ������ �����Ͽ��� 'cube.obj' �� 'bunny.obj' �� �� ������ �ʽ��ϴ�.\n");
	printf(" 'dragon.obj' �� 'buddha.obj' ������ �̿��Ͽ� �������ּ���!\n");
	printf("-----------------------------------------------------------------------------------\n");
	printf("\n");

	FILE* s = NULL;
	while (true) {
		printf("Input File Path : ");
		scanf("%s", path);

		s = fopen(path, "rt");
		if (s == NULL)
			printf("File not Found!\n");
		else
			break;
	}

	char input[100];


	// v, f ���� ī��Ʈ
	while (!feof(s)) {
		fgets(input, 100, s);
		if (input[0] == 'v')
			NumVertices += 1;

		else if (input[0] == 'f')
			planeNum += 1;
		memset(input, '\0', sizeof(input));
	}

	// -------------- �����Ҵ� ------------------

	myVertices = (struct MyVertex*)malloc(sizeof(struct MyVertex) * planeNum * 3);
	vertex = (vec4*)malloc(sizeof(vec4) * NumVertices);

	// color ����
	for (int i = 0; i < planeNum * 3; i++) {
		myVertices[i].colors = vec4(1, 1, 1, 1);
	}

	int vertIndex = 0;
	int vertices_index = 0;

	s = fopen(path, "rt");
	while (!feof(s)) { // ������ ���� �� ���� �ݺ�

		fgets(input, 100, s); // file�� �� �پ� input�� ����

		if (input[0] == 'v') {
			readV(input, vertex, vertIndex);
			vertIndex++;
		}

		if (input[0] == 'f') {
			readF(input, myVertices, &vertices_index);
		}
		memset(input, '\0', sizeof(input));
	}

	// compute normal
	for (int i = 0; i < planeNum * 3; i += 3) {
		// 3���� ��� normal�� �־���
		// normal�� ���� ���� �����ϰ� �ִٸ� ��� ���� ����
		// 3���� ���� ���� ����
		vec3 p0(myVertices[i].points.x, myVertices[i].points.y, myVertices[i].points.z);
		vec3 p1(myVertices[i + 1].points.x, myVertices[i + 1].points.y, myVertices[i + 1].points.z);
		vec3 p2(myVertices[i + 2].points.x, myVertices[i + 2].points.y, myVertices[i + 2].points.z);

		vec3 p = p1 - p0;
		vec3 q = p2 - p0;
		vec3 n = cross(p, q);
		n = normalize(n);

		myVertices[i].normals = n;
		myVertices[i + 1].normals = n;
		myVertices[i + 2].normals = n;



	}


	// The Cube should be initialized only once;
	if (bInitialized == true) return vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * planeNum * 3, myVertices, GL_STATIC_DRAW);
	// �ѹ��� �������� ����ü�� �־ ������ �Ѵ�.

	bInitialized = true;
	return vao;
}

void MyShape::SetPositionAndColorAttribute(GLuint program)
{
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(sizeof(vec4)));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(sizeof(vec4) + sizeof(vec4)));

}


void MyShape::Draw(GLuint program)
{
	if (!bInitialized) return;			// check whether it is initiazed or not. 

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	SetPositionAndColorAttribute(program);

	glDrawArrays(GL_TRIANGLES, 0, planeNum * 3);
}