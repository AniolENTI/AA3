#include <objects/Axis.h>

Axis::Axis()
{
	// Define vertexs and colours
	float verts[] = {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0
	};
	float colors[] = {
		1.0, 0.0, 0.0, 1.0,
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0,
		0.0, 0.0, 1.0, 1.0
	};
	GLubyte indexs[] = {
		0, 1,
		2, 3,
		4, 5
	};

	// Initialize buffers
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, indexs, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Initialize program
	program = new Program("Axis");
	program->compileAndAttachShader("shaders/Axis.vert", GL_VERTEX_SHADER, "vertex");
	program->compileAndAttachShader("shaders/Axis.frag", GL_FRAGMENT_SHADER, "fragment");

	// Bind Attrib locations
	program->bindAttribLocation(0, "in_Position");
	program->bindAttribLocation(1, "in_Color");

	// Link program
	program->link();
}

Axis::~Axis()
{
	glDeleteBuffers(3, VBO);
	glDeleteVertexArrays(1, &VAO);
	delete program;
}

void Axis::setTransforms(glm::mat4 MVP)
{
	this->MVP = MVP;
}

void Axis::draw()
{
	if (isActive) {
		glBindVertexArray(VAO);
		program->use();
		glUniformMatrix4fv(
			program->getUniform("mvpMat"),
			1, GL_FALSE, glm::value_ptr(MVP)
		);
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, 0);

		program->unuse();
		glBindVertexArray(0);
	}
}

void Axis::toggle()
{
	isActive = !isActive;
}