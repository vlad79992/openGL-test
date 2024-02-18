#pragma once
#pragma comment (lib, "glew32s.lib")

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <string>
#include <vector>

#include "Vertex.hpp"


class Application
{
public:
	Application();
	~Application()
	{
		// Cleanup VBO and shader
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &colorbuffer);
		glDeleteProgram(programID);
		glDeleteVertexArrays(1, &VertexArrayID);

		// Close OpenGL window and terminate GLFW
		glfwTerminate();
		//delete(window);
	}
	int CreateWindow(int width, int height, std::string name);
	int CreateWindow();
	void EnableDepthTest();
	void SetSwapInterval(int interval); // 1 is display refresh rate

	void Render();
	void BindBuffers();

	void AddShaders(std::string vertexShaderPath, std::string fragmentShaderPath);

	void AddTriangle(Vertex v1, Vertex v2, Vertex v3);

	void SetVerticesAndColors(const float* vert, const float* color, int length);
	void SetBackgroundColor(float r, float g, float b, float a);

	GLFWwindow* GetWindow();
private:
	//window
	float bg_col[4] = { 0.0f, 0.0f, 0.4f, 0.0f };
	GLFWwindow* window;
	int width = 800, height = 600;
	//triangles data
	std::vector<float> verticies;
	std::vector<float> colors;
	GLuint vertexbuffer; //vertex buffer
	GLuint colorbuffer;  //color buffer
	//shaders
	GLuint programID; //shader
	GLuint MatrixID;  //transform matrix
	GLuint VertexArrayID;
	//camera
	friend class Camera;
public:
	void CreateCamera()
	{
		this->camera = Camera(programID, MatrixID, (float)width / (float)height);
	}
	void SetView(glm::mat4 view)
	{
		camera.View = view;
		camera.mvp = camera.Projection * camera.View * camera.Model;
	}
	void SetProjection(glm::mat4 projection)
	{
		camera.Projection = projection;
		camera.mvp = camera.Projection * camera.View * camera.Model;
	}
	
	class Camera
	{
	private:
		friend class Application;
		Camera(const GLuint programID, GLuint& MatirxID, float aspect);
		Camera() 
			:Projection{}, View{}, Model{}, mvp{}
		{}
		glm::mat4 Projection;
		glm::mat4 View;
		glm::mat4 Model;
		glm::mat4 mvp;
	};
private:
	Camera camera;
};

