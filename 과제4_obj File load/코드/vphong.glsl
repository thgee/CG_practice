#version 330

in  vec4 vPosition;
in  vec4 vColor;
in  vec3 vNormal;

//out vec4 color;
out vec3 N3;
out vec3 L3;
out vec3 V3;

uniform mat4 uProjMat;
uniform mat4 uModelMat;
uniform vec4 uLPos;
uniform vec4 uLCol;
uniform vec4 uAmb;
uniform vec4 uDif;
uniform vec4 uSpec;
uniform float uShiness;


void main()
{
	gl_Position  = uProjMat * (uModelMat * vPosition);
	gl_Position *= vec4(1,1,-1,1);	// z�� ������ �ݴ���

	vec4 lPos = uLPos;
	vec4 vPos = uModelMat * vPosition;

	vec4 N = uModelMat*vec4(vNormal,0);
	vec4 L = lPos - vPos;
	N3 = normalize(N.xyz);
	L3 = normalize(L.xyz);
	V3 = normalize(vec3(0,0,0) - vPos.xyz);

}
