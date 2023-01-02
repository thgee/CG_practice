#version 330

in vec4 vPosition;
in vec4 vColor;

out vec4 color;
out vec4 position;

uniform float uTime;
uniform int uDiv;
uniform float uDist;



void main(){


	float angleX = -45;
	float angleZ = uTime * 50; // �ʴ� �� ���� ���� ����
	float angleWave = uTime* 200;

	float radX = angleX / 180.0f * 3.141592f;
	float radZ = angleZ / 180.0f * 3.141592f;
	float radWave = angleWave / 180.0f * 3.141592f;

	float radius = sqrt(vPosition.x * vPosition.x + vPosition.y * vPosition.y);
	
	




	// -------------------- ȸ����� -------------------------

	mat4 rotX = mat4(1.0f); // ������ķ� �ʱ�ȭ ���ִ� �Լ�
	mat4 rotZ = mat4(1.0f);

	rotX[1][1] = cos(radX);		rotX[2][1] = -sin(radX);
	rotX[1][2] = sin(radX);		rotX[2][2] = cos(radX);

	rotZ[0][0] = cos(radZ);		rotZ[1][0] = -sin(radZ);
	rotZ[0][1] = sin(radZ);		rotZ[1][1] = cos(radZ);




	// -------------------- ���̺� ---------------------------

	mat4 wave = mat4(1.0f);



	for(int i = 0; i < uDiv / 2 ; i += 1){
	
		float dec = 1 - (i / (uDiv / 2.0f)); // i�� �����Ҽ��� 1 ~ 0���� �������� ��

		// ���� ���� �� �ϳ�
		if(radius <= 0.001f) 
			wave[3][2] = 0.2 * sin(2 * radWave); 

		
		if(radius >= uDist * i + 0.001f && radius < uDist * (i + 1) + 0.001f)
			wave[3][2] = 0.2 * dec * sin(2 * radWave - i);
		
	}



	// ---------------- ��ǥ ���� --------------------

	vec4 wPos =  rotX *  rotZ * wave * vPosition;

	wPos.z = -wPos.z;
	gl_Position = wPos;
	
	color = vColor;
	position = wave * vPosition;


}