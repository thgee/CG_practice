#ifndef _MY_PLAIN_H_
#define _MY_PLAIN_H_

#include <vgl.h>
#include <vec.h>

struct MyPlainVertex {
	vec4 position;
	vec4 color;
};

class MyPlain {
public:

	bool bInitialized;
	int division = 30;
	MyPlainVertex* vertices;
	GLuint vao;
	GLuint vbo;
	int NUM_POINTS;
	float dist;
	int dx = 0, dy = 0;

	MyPlain();
	void setRectangle(int pos1, int pos2, int pos3, int pos4, int color);
	void init();
	void setAttributePointers(GLuint prog, float gTime);
	void draw(GLuint prog, float gTime);
	void setNumPoints(int division);
	void setDist(int division);
};

MyPlain::MyPlain(){
	bInitialized = false;
}

void MyPlain::setNumPoints(int division) {
	NUM_POINTS = 6 * division * division;
}

void MyPlain::setDist(int division){
	dist = 1.4f / division;
}

void MyPlain::setRectangle(int pos1, int pos2, int pos3, int pos4, int color) {
	static int index = 0;
	static float cnt = 0.0f;

	vec4 vertex_pos[4] = { // plain�� �� ������
		vec4(-0.7 + dist * dx, -0.7 + dist * dy, 0, 1.0),
		vec4(-0.7 + dist * (dx + 1) , -0.7 + dist * dy, 0, 1.0),				// 3   2
		vec4(-0.7 + dist * (dx + 1) , -0.7 + dist * (dy + 1), 0, 1.0),
		vec4(-0.7 + dist * dx, -0.7 + dist * (dy + 1), 0, 1.0),					// 0   1
	};

	vec4 vertex_color[2] = {
	vec4(0.7, 0.7, 0.7, 1.0),
	vec4(0.5, 0.5, 0.5, 1.0)

	};

	vertices[index].position = vertex_pos[pos1];		vertices[index].color = vertex_color[color];		index++;
	vertices[index].position = vertex_pos[pos2];		vertices[index].color = vertex_color[color];		index++;
	vertices[index].position = vertex_pos[pos3];		vertices[index].color = vertex_color[color];		index++;
	vertices[index].position = vertex_pos[pos3];		vertices[index].color = vertex_color[color];		index++;
	vertices[index].position = vertex_pos[pos4];		vertices[index].color = vertex_color[color];		index++;
	vertices[index].position = vertex_pos[pos1];		vertices[index].color = vertex_color[color];		index++;

	if (dy == division - 1 && dx == division - 1) { // ��� ���� �� �׷�����
		dx = 0;
		dy = 0;
		index = 0;
		return; // �ʱ�ȭ �� Ż��
	}

	if (dx == division - 1) { // ���� �� �׷�����
		dy++; // �� ����
		dx = 0; // �� �ʱ�ȭ
		return;
	}
	dx++;
}

void MyPlain::init() {
	setNumPoints(division);
	setDist(division);
	vertices = new MyPlainVertex[NUM_POINTS];


	for (int i = 0, k = 0; i < division; i++) {
		for (int j = 0; j < division; j++) {
			if (j % 2 == k) setRectangle(0, 1, 2, 3, 0); // color�� 0 or 1�� �����Ƽ� �־��ֱ� ����
			else setRectangle(0, 1, 2, 3, 1);
		}
		if (i % 2 == 0) k = 1;
		else k = 0;
	}

		if (bInitialized == false) glGenVertexArrays(1, &vao); // vao�� �������� ���� �� �ѹ��� �����Ѵ�.
		glBindVertexArray(vao);

		if (bInitialized == false) glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyPlainVertex) * NUM_POINTS, vertices, GL_STATIC_DRAW);

		delete[] vertices;

}

void MyPlain::setAttributePointers(GLuint prog, float gTime) { // �����͵��� ��� �����о�� �ϴ��� shader�� �˷���

	glUseProgram(prog);

	GLuint vPosition = glGetAttribLocation(prog, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, FALSE, sizeof(MyPlainVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(prog, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, FALSE, sizeof(MyPlainVertex), BUFFER_OFFSET(sizeof(vec4)));
	
	GLuint uTime = glGetUniformLocation(prog, "uTime");
	glUniform1f(uTime, gTime);
																	
	GLuint uDist = glGetUniformLocation(prog, "uDist");
	glUniform1f(uDist, dist);
}

void MyPlain::draw(GLuint prog, float gTime) { 
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	setAttributePointers(prog, gTime); // �����͵��� ��� �����о�� �ϴ��� shader�� �˷���
	glDrawArrays(GL_TRIANGLES, 0, NUM_POINTS); // ������ ���� ������� �׸��� ����
}

#endif
