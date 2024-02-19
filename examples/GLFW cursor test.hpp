#pragma once

#include "../engine/source/Application.h"

#include <iostream>
#include <math.h>
struct Point
{
	long long ID;
	int state;//represent nothing can be used in any way
	double r, g, b;
	double x, y;
	double radius;
	double x_vel, y_vel;
	int draw_segments = 24;
	float* Create2DCircle()
	{
		double pi = 3.1415926535;
		float* vert = (float*)malloc(sizeof(float) * draw_segments * 3 * 3);
		for (int i = 0; i < draw_segments && vert != nullptr; ++i)
		{
			//std::cout << i + 1 << ')' << std::endl;
			vert[i * 9 + 0] = x;
			vert[i * 9 + 1] = y;
			vert[i * 9 + 2] = 0.0f;
			//std::cout << vert[i * 9 + 0] << ' ' << vert[i * 9 + 1] << ' ' << vert[i * 9 + 2] << std::endl;

			vert[i * 9 + 3] = x + radius * cos(pi * (float)i * 2 / draw_segments);
			vert[i * 9 + 4] = y + radius * sin(pi * (float)i * 2 / draw_segments);
			vert[i * 9 + 5] = 0.0f;
			//std::cout << vert[i * 9 + 3] << ' ' << vert[i * 9 + 4] << ' ' << vert[i * 9 + 5] << std::endl;
			
			vert[i * 9 + 6] = x + radius * cos(pi * (float)(i + 1) * 2 / draw_segments);
			vert[i * 9 + 7] = y + radius * sin(pi * (float)(i + 1) * 2 / draw_segments);
			vert[i * 9 + 8] = 0.0f;
			//std::cout << vert[i * 9 + 6] << ' ' << vert[i * 9 + 7] << ' ' << vert[i * 9 + 8] << std::endl;
			//std::cout << "------------" << std::endl;
		}
		return vert;
	}
	float* CreateColorArray(float r, float g, float b)
	{
		float* color = (float*)malloc(sizeof(float) * draw_segments * 3 * 3);
		for (int i = 0; i < draw_segments * 9 && color != nullptr; i += 3)
			color[i] = r, color[i + 1] = g, color[i + 2] = b;
		return color;
	}

	struct HashFunction
	{
		size_t operator()(const Point& point) const
		{
			return point.ID;
		}
	};
	Point GetCopy()
	{
		Point p;
		p.ID = this->ID;
		p.x = this->x;
		p.y = this->y;
		p.x_vel = this->x_vel;
		p.y_vel = this->y_vel;
		p.radius = this->radius;
		p.state = this->state;
		return p;
	}
};

inline void cursor_pos_test()
{
	Application app;
	app.CreateWindow(1280, 720, "Test");
	app.SetSwapInterval(1);
	app.EnableDepthTest();


	// Create and compile our GLSL program from the shaders
	app.AddShaders("resources/shaders/SimpleVertexShader.vertexshader", "resources/shaders/SimpleFragmentShader.fragmentshader");
	//create camera
	app.CreateCamera();

	//app.SetVerticesAndColors(g_vertex_buffer_data, g_color_buffer_data, 108);
	app.AddTriangle(Vertex(-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
					Vertex(1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
					Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.1f, 1.0f));


	app.BindBuffers();
	app.SetBackgroundColor(0.1f, 0.2f, 0.7f, 0.0f);

	app.SetView(glm::lookAt(
		glm::vec3(0, 0, -1), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	));
	app.SetProjection(glm::ortho(-1.0f, 1.0f, -1.0f * ((float) 9 / 16), 1.0f * ((float)9 / 16), 0.1f, 100.0f));
	double x = 0, y = 0;
	Point p;
	p.radius = 0.1f;
	glfwSetCursorPos(app.GetWindow(), 640, 360);
	do {
		app.Render();		
		app.GetCursorPosition(x, y);
		//std::cout << x << '\t' << y << std::endl;
		app.ClearVerticies();
		x = x / 640 - 1;
		y = ((float)9 / 16) * ((y) / 360 - 1);
		p.x = -x, p.y = -y;
		app.SetVerticesAndColors(p.Create2DCircle(), p.CreateColorArray(1.0f, 1.0f, 1.0f), p.draw_segments * 3 * 3);
		app.BindBuffers();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(app.GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(app.GetWindow()) == 0);

}