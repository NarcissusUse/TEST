#version 330 core  
layout (location = 0) in vec3 aPos;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
out vec3 Pos;
void main()
{  
	gl_Position = projection*view*model* vec4(1.0);
	Pos=aPos;
}