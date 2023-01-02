#version 330

in vec4 color; // in으로 받아온 값은 변화시킬수 없다
in vec4 position;

out vec4 fcolor;


void main(){

	fcolor = color;

	for(float i = 0; i < 0.2f; i += 0.0001f){

		// z 가 높아질수록 점점 주황색이 된다
		if(position.z > i) fcolor = vec4(color.r + i * 5, color.g - i * 1, color.b - i * 3, 1); 
	
		// z 가 낮아질수록 점점 파란색이 된다
		if(position.z < -i) fcolor = vec4(color.r - i * 1, color.g - i * 1, color.b + i * 3, 1); 

	}
		
}