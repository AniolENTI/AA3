#version 330
layout (location = 0) in vec3 aPos;
uniform mat4 objMat;
uniform mat4 mvpMat;

void main() {
	gl_Position = mvpMat * objMat * vec4(aPos, 1.0);
}