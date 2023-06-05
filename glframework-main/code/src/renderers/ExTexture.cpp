#include "renderers/ExTexture.h"


#define STB_IMAGE_IMPLEMENTATION
#include "imgui/stb_image.h"

ExTexture::ExTexture(glm::vec3 topRight, glm::vec3 topLeft, glm::vec3 botLeft, glm::vec3 botRight, char* path)
{
	float vertices[]{
		//Positions								//Texture coords
		 topRight.x,  topRight.y, topRight.z,	 1.0f,  1.0f,//top right
		 topLeft.x,   topLeft.y,  topLeft.z,	-1.0f,  1.0f,//top left
		 botRight.x,  botRight.y, botRight.z,	-1.0f, -1.0f,//bottom left
		 botLeft.x,   botLeft.y,  botLeft.z,	 1.0f, -1.0f,//bottom right
	};
	unsigned int indices[]{
		0, 1, 3, //upper triangle
		1, 2, 3 //bottom triangle
	};

	//Generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Vertex Array Object
	glBindVertexArray(VAO);

	//Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Define the position attribute
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		5 * sizeof(float),
		(void*)0
	);
	glEnableVertexAttribArray(0);

	//Define the texture coord attribute
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		5 * sizeof(float),		   //Stride
		(void*)(3 * sizeof(float)) //Starting byte
	);
	glEnableVertexAttribArray(1);

	//Load image in CPU
	int imgWidth, imgHeight, nrCannels;
	unsigned char* data = stbi_load(path,
		&imgWidth,
		&imgHeight,
		&nrCannels,
		0);
	if (data)
	{
		//Generate texture
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D, 	 //Texture target
			0,				  //Mipmap level
			GL_RGB,			  //Store the texture with RGB values
			imgWidth,		  //Image width
			imgHeight,		  //Image height
			0,				  //Always zero (legacy stuff)
			GL_RGB,			  //Format of the source
			GL_UNSIGNED_BYTE, //Datatype of the source image
			data			  //Image data
		);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		//Handle error
		std::cerr << "Error loading the texture." << std::endl;
	}
	stbi_image_free(data);

	//Initialize program and shaders
	program = new Program("square");
	program->compileAndAttachShader("shaders/Texture.vert", GL_VERTEX_SHADER);
	program->compileAndAttachShader("shaders/Texture.frag", GL_FRAGMENT_SHADER);

	//Bind attribute locations
	program->bindAttribLocation(0, "in_Position");
	program->bindAttribLocation(1, "in_UV");

	//Link program
	program->link();

	//Unbind
	glBindVertexArray(0);
	program->unuse();
}

ExTexture::~ExTexture()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete program;
}

void ExTexture::draw()
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	program->use();

	glm::vec4 color = glm::vec4(1.0f, 0.3f, 0.2f, 1.0f);
	glm::mat4 objMat = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -5.0f));

	glUniformMatrix4fv(
		program->getUniform("objMat"),
		1, GL_FALSE, glm::value_ptr(objMat)
	);

	glUniformMatrix4fv(
		program->getUniform("mvpMat"),
		1, GL_FALSE, glm::value_ptr(cam._MVP)
	);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	program->unuse();
	glBindVertexArray(0);
}

void ExTexture::setTransforms(CameraTransforms cam)
{
	this->cam = cam;
}