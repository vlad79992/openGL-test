#define GLM_ENABLE_EXPERIMENTAL

#include "Application.h"

#include "shader compilation.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Application::Application()
	:colorbuffer{}, vertexbuffer{}, MatrixID{}, programID{}, window{}, camera{}
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
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
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
	glDrawArrays(GL_TRIANGLES, 0, verticies.size()); // 12*3 indices starting at 0 -> 12 triangles

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticies.size(), verticies.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), colors.data(), GL_STATIC_DRAW);
}

void Application::AddShaders(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	this->programID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
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

void Application::SetVerticesAndColors(const float* vert, const float* color, int length)
{
	for (int i = 0; i < length; ++i)
	{
		verticies.push_back(vert[i]);
		colors.push_back(color[i]);
	}
}

void Application::SetBackgroundColor(float r, float g, float b, float a)
{
	bg_col[0] = r, bg_col[1] = g, bg_col[2] = b, bg_col[3] = a;
}

GLFWwindow* Application::GetWindow()
{
	return this->window;
}

Application::Camera::Camera(const GLuint programID, GLuint &matrixId, float aspect)
	:Projection{}, View{}, Model{}, mvp{}
{	
	// Get a handle for our "MVP" uniform
	matrixId = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
	// Camera matrix
	View = glm::lookAt(
		glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
}
