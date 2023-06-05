#version 330

in vec3 in_Position;
in vec2 in_UV;

uniform mat4 objMat;
uniform mat4 mvpMat;

out vec2 texCoord;

void main() {
	gl_Position = mvpMat * objMat * vec4(in_Position, 1.0);
	texCoord = in_UV;
}