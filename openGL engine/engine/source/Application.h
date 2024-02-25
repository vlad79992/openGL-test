#pragma once
#pragma comment (lib, "glew32s.lib")

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <vector>
#include <mutex>

import Vertex;
import Camera;
import Point;
import Timer;
import BMP_loader;

class Application
{
public:
	//float *pos = nullptr, *col = nullptr;
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
		//delete(pos);
		//delete(col);
	}
	int CreateWindow(int width, int height, std::string name);
	int CreateWindow();
	void EnableDepthTest();
	void SetSwapInterval(int interval); // 1 is display refresh rate

	void Render();
	void BindBuffers();
	//void BindRawBuffers(int length);

	void AddShaders(std::string vertexShaderPath, std::string fragmentShaderPath);
	void AddTextures(std::string texturePath); //add BMP texture

	void AddTriangle(Vertex v1, Vertex v2, Vertex v3);
	//get size of vectors
	int getVerticiesSize();
	int getColorsSize();
	int getUVsSize();
	//clear vectors
	void ClearVerticies();
	void ClearColors();
	void ClearUVs();
	//resize vectors
	void ResizeVerticies(int new_size);
	void ResizeColors(int new_size);
	void ResizeUVs(int new_size);
	//set element of vectors
	void VerticiesSetAt(int pos, float value);
	void ColorsSetAt(int pos, float value);
	void UVsSetAt(int pos, float value);
	//
	void SetVerticesAndColors(const float* vert, const float* color, int length, bool free); //if free is true float pointers will be deleted
	void SetVerticesAndColors(const std::vector<float>& vert, const std::vector<float>& color); //if free is true float pointers will be deleted
	void SetUVs(const float* UV, int length);
	//void SetVerticesAndColors(const float* vert, const float* color, int length, std::mutex& mutex);
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
	//TODO рендер в несколько слоев
	GLuint vertexbuffer;  //vertex buffer
	GLuint colorbuffer;  //color buffer
	//textures
	std::vector<float> UVs;
	GLuint uvbuffer;
	GLuint texture;     //texture
	long long vert_size;
	//shaders
	GLuint programID; //shader
	GLuint MatrixID;  //transform matrix
	GLuint VertexArrayID;
	//camera
	friend class Camera;
	Camera camera;// = Camera(programID, MatrixID, (float)width / (float)height);
public:
	
	void CreateCamera();

	void SetView(glm::mat4 view);

	void SetProjection(glm::mat4 projection);
	
private:
};

