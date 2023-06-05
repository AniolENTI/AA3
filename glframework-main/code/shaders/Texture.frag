#version 330

in vec2 texCoord;
out vec4 out_Color;

uniform sampler2D ourTexture;

void main() {
	out_Color = texture(ourTexture, texCoord);
}