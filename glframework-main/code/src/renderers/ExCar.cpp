#include "renderers/ExCar.h"

ExCar::ExCar(int width, int height) : Renderer(width, height)
{
	//GENERACIO COTXE
	{

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
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

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

	//GENERACIO PLA
	texture = new ExTexture(topRight, topLeft, botLeft, botRight, path);

	//GENERACIO MIRALL
	mirror = new ExTexture(topRightMirror, topLeftMirror, botLeftMirror, botRightMirror, path);
	setupFBO();
	this->width = width;
	this->height = height;
}

ExCar::~ExCar()
{
	//DESTRUCTOR COTXE
	delete program;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//DESTRUCTOR PLA
	delete texture;

	//DESTRUCTOR MIRALL
	delete mirror;
}

void ExCar::render(float dt)
{
	//RENDER COTXE
	{
		program->use();
		glBindVertexArray(VAO);

		// Get location of uniforms
		GLuint colorLoc = program->getUniform("aCol");
		GLuint objMatLoc = program->getUniform("objMat");
		GLuint mvpMatLoc = program->getUniform("mvpMat");

		// Declare all the matrices that we will use
		glm::mat4 model, view, objMat;

		// Draw model
		glUniform4f(colorLoc, 0.9f, 0.1f, 0.1f, 1.0f);
		model = glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(glm::mat4(), glm::vec3(0.0f, -10.0f, -20.0f));
		objMat = view * model;
		glUniformMatrix4fv(objMatLoc, 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, glm::value_ptr(cam._MVP));
		glDrawArrays(GL_TRIANGLES, 0, 678424528);
	}

	//RENDER PLA
	texture->setTransforms(cam);
	texture->draw();

	//RENDER MIRALL
	/*{
		// Setup fbo camera
		CameraTransforms fbCam;
		fbCam._modelView = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 3.f));
		fbCam._projection = glm::perspective(FOV, 1.f, zNear, zFar);
		fbCam._MVP = fbCam._projection * fbCam._modelView;

		// RENDER IN THE NEW FRAMEBUFFER
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(0, 0, 800, 800);
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// Restore the state
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);  // Remember to store the width and the height in Renderer

		// Draw the textured plane with the FBO
		// You must refactor the textures example that we did in
		// class into a class that follows this interface
		mirror->setTransforms(cam);
		mirror->forceChangeTexture(fbo_tex);
		mirror->draw();
	}*/
	
}

void ExCar::setupFBO()
{
	glGenFramebuffers(1, &fbo);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800);  // 24 bits for depth, 8 bits for stencil
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenTextures(1, &fbo_tex);
	glBindTexture(GL_TEXTURE_2D, fbo_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,  // En quin buffer ho guardem (color, depth, stencil)
		GL_TEXTURE_2D,
		fbo_tex,
		0
	);
	// WARNING: If this part is commented we will not see the dragon. Why?
	// glFramebufferRenderbuffer(
	// 	GL_FRAMEBUFFER,
	// 	GL_DEPTH_STENCIL_ATTACHMENT,
	// 	GL_RENDERBUFFER,
	// 	rbo
	// );
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}