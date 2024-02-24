#include "Application.h"
#define GLM_ENABLE_EXPERIMENTAL

#include "Application.h"

import shader_compilation;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Application::Application()
	:colorbuffer{}, vertexbuffer{}, MatrixID{}, VertexArrayID{}, programID{}, window{}, vert_size{}
{
}

int Application::CreateWindow(int width, int height, std::string name)
{
	this->width = width, this->height = height;
	
	//camera = Camera((float)width / (float)height);

	glewExperimental = true;
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW\n";
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	//GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
	window = glfwCreateWindow(width, height, "Window", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to open GLFW window.\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW\n";
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
	
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &colorbuffer);
}

int Application::CreateWindow()
{
	return (this->CreateWindow(this->width, this->height, "Window"));
}

void Application::EnableDepthTest()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
}

void Application::SetSwapInterval(int interval)
{
	glfwSwapInterval(1);
}

void Application::Render()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bg_col[0], bg_col[1], bg_col[2], bg_col[3]);

	// Use our shader
	glUseProgram(programID);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &camera.mvp[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, vert_size); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Application::BindBuffers()
{
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticies.size(), verticies.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), colors.data(), GL_DYNAMIC_DRAW);
	vert_size = verticies.size();
}

//void Application::BindRawBuffers(int length)
//{
//	vert_size = length;
//	// The following commands will talk about our 'vertexbuffer' buffer
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//	// Give our vertices to OpenGL.
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * length, this->pos, GL_DYNAMIC_DRAW);
//
//	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * length, this->col, GL_DYNAMIC_DRAW);
//}

void Application::AddShaders(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	this->programID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
}

void Application::AddTextures(std::string texturePath)
{
	this->texture = loadBMP_custom(texturePath);
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * UVs.size(), UVs.data(), GL_DYNAMIC_DRAW);
}

void Application::AddTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	//push verticies
	verticies.push_back(v1.position.x);
	verticies.push_back(v1.position.y);
	verticies.push_back(v1.position.z);
	verticies.push_back(v2.position.x);
	verticies.push_back(v2.position.y);
	verticies.push_back(v2.position.z);
	verticies.push_back(v3.position.x);
	verticies.push_back(v3.position.y);
	verticies.push_back(v3.position.z);
	//push vertex colors
	colors.push_back(v1.color.r);
	colors.push_back(v1.color.g);
	colors.push_back(v1.color.b);
	//colors.push_back(v1.color.a);
	colors.push_back(v2.color.r);
	colors.push_back(v2.color.g);
	colors.push_back(v2.color.b);
	//colors.push_back(v2.color.a);
	colors.push_back(v3.color.r);
	colors.push_back(v3.color.g);
	colors.push_back(v3.color.b);
	//colors.push_back(v3.color.a);

}

int Application::getVerticiesSize()
{
	return verticies.size();
}

int Application::getColorsSize()
{
	return colors.size();
}

int Application::getUVsSize()
{
	return UVs.size();
}

void Application::ClearVerticies()
{
	verticies.clear();
}

void Application::ClearColors()
{
	colors.clear();
}

void Application::ClearUVs()
{
	UVs.clear();
}

void Application::ResizeVerticies(int new_size)
{
	verticies.resize(new_size);
}

void Application::ResizeColors(int new_size)
{
	colors.resize(new_size);
}

void Application::ResizeUVs(int new_size)
{
	UVs.resize(new_size);
}

void Application::VerticiesSetAt(int pos, float value)
{
	//if (pos < 0 || pos > verticies.size()) //SLOOOOOOOOOW
	//{
	//	std::runtime_error("invalide index\n");
	//}
	verticies[pos] = value;
}

void Application::ColorsSetAt(int pos, float value)
{
	//if (pos < 0 || pos > colors.size()) //SLOOOOOOOOOW
	//{
	//	std::runtime_error("invalide index\n");
	//}
	colors[pos] = value;
}

void Application::UVsSetAt(int pos, float value)
{
	if (pos < 0 || pos > UVs.size())
	{
		std::runtime_error("invalide index\n");
	}
	UVs[pos] = value;
}

void Application::SetVerticesAndColors(const float* vert, const float* color, int length, bool free)
{
	for (int i = 0; i < length; ++i)
	{
		verticies.push_back(vert[i]);
		colors.push_back(color[i]);
	}
	if (free)
	{
		delete(vert);
		delete(color);
	}
}

void Application::SetVerticesAndColors(const std::vector<float>& vert, const std::vector<float>& col)
{
	for (int i = 0; i < vert.size(); ++i)
		verticies.push_back(vert[i]);
	for (int j = 0; j < col.size(); ++j)
		colors.push_back(col[j]);
}

//void Application::SetVerticesAndColors(const float* vert, const float* color, int length, std::mutex& mutex)
//{
//	mutex.lock();
//	//std::cout << "locked" << std::endl;
//	for (int i = 0; i < length; ++i)
//	{
//		verticies.push_back(vert[i]);
//		colors.push_back(color[i]);
//	}
//	delete(vert);
//	delete(color);
//	//std::cout << "unlocked" << std::endl;
//	mutex.unlock();
//}

void Application::SetUVs(const float* UV, int length)
{
	for (int i = 0; i < length; ++i)
	{
		verticies.push_back(UV[i]);
	}
}

void Application::SetBackgroundColor(float r, float g, float b, float a)
{
	bg_col[0] = r, bg_col[1] = g, bg_col[2] = b, bg_col[3] = a;
}

void Application::GetCursorPosition(double& xpos, double& ypos)
{
	glfwGetCursorPos(window, &xpos, &ypos);
}

GLFWwindow* Application::GetWindow()
{
	return this->window;
}

void Application::CreateCamera()
{
	camera.CameraInit(programID, MatrixID, (float)width / (float)height);
}
void Application::SetView(glm::mat4 view)
{
	camera.View = view;
	camera.mvp = camera.Projection * camera.View * camera.Model;
}
void Application::SetProjection(glm::mat4 projection)
{
	camera.Projection = projection;
	camera.mvp = camera.Projection * camera.View * camera.Model;
}
