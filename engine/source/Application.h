#pragma once
#pragma comment (lib, "glew32s.lib")

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <string>
#include <vector>
#include <mutex>
#include <chrono>

#include "Vertex.hpp"


class Application
{
public:
	float *pos = nullptr, *col = nullptr;
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
		delete(pos);
		delete(col);
	}
	int CreateWindow(int width, int height, std::string name);
	int CreateWindow();
	void EnableDepthTest();
	void SetSwapInterval(int interval); // 1 is display refresh rate

	void Render();
	void BindBuffers();
	void BindRawBuffers(int length);

	void AddShaders(std::string vertexShaderPath, std::string fragmentShaderPath);

	void AddTriangle(Vertex v1, Vertex v2, Vertex v3);
	void ClearVerticies();

	void SetVerticesAndColors(const float* vert, const float* color, int length);
	void SetVerticesAndColors(const float* vert, const float* color, int length, std::mutex& mutex);
	void SetBackgroundColor(float r, float g, float b, float a);

	void GetCursorPosition(double& xpos, double& ypos);

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
	long long vert_size;
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

class Timer
{
public:
	void start()
	{
		m_StartTime = std::chrono::system_clock::now();
		m_bRunning = true;
	}

	void stop()
	{
		m_EndTime = std::chrono::system_clock::now();
		m_bRunning = false;
	}

	double elapsedMilliseconds()
	{
		std::chrono::time_point<std::chrono::system_clock> endTime;

		if (m_bRunning)
		{
			endTime = std::chrono::system_clock::now();
		}
		else
		{
			endTime = m_EndTime;
		}

		return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
	}

	double elapsedSeconds()
	{
		return elapsedMilliseconds() / 1000.0;
	}

private:
	std::chrono::time_point<std::chrono::system_clock> m_StartTime;
	std::chrono::time_point<std::chrono::system_clock> m_EndTime;
	bool                                               m_bRunning = false;
};