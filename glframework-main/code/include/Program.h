#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Program
{
public:
	GLuint ID;
	const char* name;

	Program(const char* name) : name{ name } {
		ID = glCreateProgram();
	}

	~Program() {
		glDeleteProgram(ID);
	}

	void compileAndAttachShader(const char* shaderPath, GLenum shaderType, const char* shaderName = "") {
		std::string shaderSource;
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// Read shader from file
		try {
			shaderFile.open(shaderPath);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			shaderSource = shaderStream.str();
		}
		catch (std::ifstream::failure& e) {
			std::cout << "Error reading shader " << shaderName << " in program " << name << " : " << e.what() << std::endl;
		}

		// Compile and attach shader
		GLuint shader = compileShader(shaderSource.c_str(), shaderType, shaderName);
		glAttachShader(ID, shader);
		// Delete shader (as it is already attached to the program)
		glDeleteShader(shader);
	}

	void link() {
		glLinkProgram(ID);
		GLint res;
		glGetProgramiv(ID, GL_LINK_STATUS, &res);
		if (res == GL_FALSE) {
			glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &res);
			char* buff = new char[res];
			glGetProgramInfoLog(ID, res, &res, buff);
			fprintf(stderr, "Error Link: %s", buff);
			delete[] buff;
		}
	}

	GLuint getUniform(const char* name) {
		return glGetUniformLocation(ID, name);
	}

	void bindAttribLocation(GLuint index, const char* varName) {
		glBindAttribLocation(ID, index, varName);
	}

	void use() {
		glUseProgram(ID);
	}

	void unuse() {
		glUseProgram(0);
	}
private:
	GLuint compileShader(const char* shaderStr, GLenum shaderType, const char* name)
	{
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderStr, NULL);
		glCompileShader(shader);
		GLint res;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
		if (res == GL_FALSE) {
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &res);
			char* buff = new char[res];
			glGetShaderInfoLog(shader, res, &res, buff);
			fprintf(stderr, "Error Shader %s: %s", name, buff);
			delete[] buff;
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}
};