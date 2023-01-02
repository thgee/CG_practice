#version 330

in vec4 color; // in���� �޾ƿ� ���� ��ȭ��ų�� ����
in vec4 position;

out vec4 fcolor;


void main(){

	fcolor = color;

	for(float i = 0; i < 0.2f; i += 0.0001f){

		// z �� ���������� ���� ��Ȳ���� �ȴ�
		if(position.z > i) fcolor = vec4(color.r + i * 5, color.g - i * 1, color.b - i * 3, 1); 
	
		// z �� ���������� ���� �Ķ����� �ȴ�
		if(position.z < -i) fcolor = vec4(color.r - i * 1, color.g - i * 1, color.b + i * 3, 1); 

	}
		
}