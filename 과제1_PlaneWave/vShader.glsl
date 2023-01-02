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
	float angleZ = uTime * 50; // 초당 몇 도씩 돌게 만듬
	float angleWave = uTime* 200;

	float radX = angleX / 180.0f * 3.141592f;
	float radZ = angleZ / 180.0f * 3.141592f;
	float radWave = angleWave / 180.0f * 3.141592f;

	float radius = sqrt(vPosition.x * vPosition.x + vPosition.y * vPosition.y);
	
	




	// -------------------- 회전행렬 -------------------------

	mat4 rotX = mat4(1.0f); // 단위행렬로 초기화 해주는 함수
	mat4 rotZ = mat4(1.0f);

	rotX[1][1] = cos(radX);		rotX[2][1] = -sin(radX);
	rotX[1][2] = sin(radX);		rotX[2][2] = cos(radX);

	rotZ[0][0] = cos(radZ);		rotZ[1][0] = -sin(radZ);
	rotZ[0][1] = sin(radZ);		rotZ[1][1] = cos(radZ);




	// -------------------- 웨이브 ---------------------------

	mat4 wave = mat4(1.0f);



	for(int i = 0; i < uDiv / 2 ; i += 1){
	
		float dec = 1 - (i / (uDiv / 2.0f)); // i가 증가할수록 1 ~ 0으로 낮아지는 수

		// 가장 안쪽 점 하나
		if(radius <= 0.001f) 
			wave[3][2] = 0.2 * sin(2 * radWave); 

		
		if(radius >= uDist * i + 0.001f && radius < uDist * (i + 1) + 0.001f)
			wave[3][2] = 0.2 * dec * sin(2 * radWave - i);
		
	}



	// ---------------- 좌표 적용 --------------------

	vec4 wPos =  rotX *  rotZ * wave * vPosition;

	wPos.z = -wPos.z;
	gl_Position = wPos;
	
	color = vColor;
	position = wave * vPosition;


}