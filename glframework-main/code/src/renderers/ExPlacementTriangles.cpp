#include <renderers/ExPlacementTriangles.h>

ExPlacementTriangles::ExPlacementTriangles(int width, int height) : Renderer(width, height)
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	const char* vertShaderFile = "shaders/triangle.vert";
	const char* fragShaderFile = "shaders/triangle.frag";

	program = new Program("Triangle");
	program->compileAndAttachShader(vertShaderFile, GL_VERTEX_SHADER, "vertex");
	program->compileAndAttachShader(fragShaderFile, GL_FRAGMENT_SHADER, "fragment");
	program->link();

	// Create the vertex array object
	// This object maintains the state related to the input of the OpenGL pipeline
	// In this example, the VAO defined will contain all the state related to the
	// below operations for vertex attributes definition.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the vertex buffer object
	// It contains arbitrary data for the vertices. In our case, its coordinates.
	glGenBuffers(1, &VBO);

	// From this point until we bind another buffer, calls related to the
	// array buffer will use VBO.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// For example, copy the data to the array buffer...
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ...and specify the layout of the arbitrary data setting the attributes of the vertex buffer
	glVertexAttribPointer(
		0, // Set the location for this attribute to 0 (same that we specified in the shader)
		3, // Size of the vertex attribute. In this case, 3 coordinates x, y, z
		GL_FLOAT,
		GL_FALSE, // Don't normalize the data.
		3 * sizeof(float),  // stride (space between consecutive vertex attributes)
		(void*)0 // offset of where the position data begins in the buffer (in this case 0)
	);

	// Once the attribute is specified, we enable it. The parameter is the location of the attribute
	glEnableVertexAttribArray(0);

	// Not necessary, but recomendable as it ensures that we will not use the VAO accidentally.
	glBindVertexArray(0);
}

ExPlacementTriangles::~ExPlacementTriangles()
{
	delete program;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void ExPlacementTriangles::render(float dt)
{
	program->use();
	glBindVertexArray(VAO);

	// Get location of uniforms
	GLuint colorLoc = program->getUniform("aCol");
	GLuint objMatLoc = program->getUniform("objMat");
	GLuint mvpMatLoc = program->getUniform("mvpMat");

	// Declare all the matrices that we will use
	glm::mat4 model, view, objMat;

	// Draw front triangle (red)
	glUniform4f(colorLoc, 0.9f, 0.1f, 0.1f, 1.0f);
	model = glm::rotate(glm::mat4(), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -10.0f));
	objMat = view * model;
	glUniformMatrix4fv(objMatLoc, 1, GL_FALSE, glm::value_ptr(objMat));
	glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, glm::value_ptr(cam._MVP));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Draw right triangle (blue)
	glUniform4f(colorLoc, 0.1f, 0.1f, 0.9f, 1.0f);
	model = glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, -10.0f));
	objMat = view * model;
	glUniformMatrix4fv(objMatLoc, 1, GL_FALSE, glm::value_ptr(objMat));
	glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, glm::value_ptr(cam._MVP));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Draw left triangle (green)
	glUniform4f(colorLoc, 0.1f, 0.9f, 0.1f, 1.0f);
	model = glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
	view = glm::translate(glm::mat4(), glm::vec3(-2.0f, 0.0f, -10.0f));
	objMat = view * model;
	glUniformMatrix4fv(objMatLoc, 1, GL_FALSE, glm::value_ptr(objMat));
	glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, glm::value_ptr(cam._MVP));
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
