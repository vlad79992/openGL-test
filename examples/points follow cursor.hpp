#pragma once

#include "../engine/source/Application.h"

#include <vector>
#include <random>

double c = 0.01;
int resx = 1600, resy = 900;

void points_and_cursor(std::vector<Point>& points, double xpos, double ypos, Application& app)
{
	auto y = [](double d)
		{
			if (d < 0) return -1;
			if (d > 0) return 1;
			if (d == 0) return 0;
		};
	double max_vel = 0.065, static_damping = 1 - 1e-2;
	for (Point& p : points)
	{
		double dist = ((p.x - xpos) * (p.x - xpos) + (p.y - ypos) * (p.y - ypos));
		if (dist < (c / 150) * (c / 150))
		{
			double to_cursor_x = (1) * (c) / dist * (p.x - xpos) * 1e-5;
			double to_cursor_y = (1) * (c) / dist * (p.y - ypos) * 1e-5;

			p.x_vel += to_cursor_x;
			p.y_vel += to_cursor_y;
		}

		if (p.x_vel > max_vel)
			p.x_vel = max_vel;
		if (p.y_vel > max_vel)
			p.y_vel = max_vel;
		if (p.x_vel < -max_vel)
			p.x_vel = -max_vel;
		if (p.y_vel < -max_vel)
			p.y_vel = -max_vel;
		p.x += p.x_vel;
		p.y += p.y_vel;
		app.SetVerticesAndColors(p.Create2DCircle(), p.CreateColorArray(p.r, p.g, p.b), p.draw_segments * 3 * 3);
		//if (p->y >= -(9.0f / 16.0f) + p->radius * 2)
		//	p->y_vel -= gravity;

		p.x_vel *= static_damping, p.y_vel *= static_damping;
		if ((p.x > (1.0f - 0.025) && p.x_vel > 0) || (p.x < (-1.0f + 0.025) && p.x_vel < 0))
			p.x_vel *= -0.75;
		if ((p.y > (((float)resy / resx) - 0.025) && p.y_vel > 0) || ((p.y < -((float)resy / resx) + 0.025) && p.y_vel < 0))
			p.y_vel *= -0.75;
	}
}

void points_follow_cursor()
{
	//random
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist_x(-1.0f + 0.025, 1.0f - 0.025);
	std::uniform_real_distribution<> dist_y(-((float)resy / resx) + 0.025, ((float)resy / resx) - 0.025);
	//window creation
	Application app;
	app.CreateWindow(resx, resy, "Test");
	app.SetSwapInterval(1);
	app.EnableDepthTest();



	// Create and compile our GLSL program from the shaders
	app.AddShaders("resources/shaders/SimpleVertexShader.vertexshader", "resources/shaders/SimpleFragmentShader.fragmentshader");
	//create camera
	app.CreateCamera();


	std::vector<Point> points;
	for (int i = 0; i < 20000; ++i) //generate 100 random points
	{
		Point p;
		p.radius = 0.0025f;
		p.draw_segments = 8;
		p.x = dist_x(gen);
		p.y = dist_y(gen);
		p.r = (p.x + 1) / 2, p.b = (p.y + 1) / 2, p.g = p.x * p.y;
		app.SetVerticesAndColors(p.Create2DCircle(), p.CreateColorArray(p.r, p.g, p.b), p.draw_segments * 3 * 3);
		p.x_vel = dist_x(gen) / 500;
		p.y_vel = dist_x(gen) / 500;
		p.ID = i;
		p.state = 0;
		points.push_back(p);
	}

	app.BindBuffers();
	app.SetBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);



	app.SetView(glm::lookAt(
		glm::vec3(0, 0, -1), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	));
	double x, y;
	app.SetProjection(glm::ortho(-1.0f, 1.0f, -1.0f * ((float)resy / resx), 1.0f * ((float)resy / resx), 0.1f, 100.0f));
	do {
		app.Render();
		app.ClearVerticies();
		app.GetCursorPosition(x, y);
		if (glfwGetMouseButton(app.GetWindow(), 0) == GLFW_PRESS)
			c = 25;
		else if (glfwGetMouseButton(app.GetWindow(), 1) == GLFW_PRESS)
			c = 0;
		else
			c = 7.5;
		if (glfwGetMouseButton(app.GetWindow(), 2) == GLFW_PRESS)
			c *= -1;
		//if (x > resx || x < 0) x = resx / 2;
		//if (y > resy  || y < 0) y = resy / 2;
		x = (x / (resx / 2) - 1) * -1;
		y = (((float)resy / resx) * ((y) / (resy / 2) - 1)) * -1;
		points_and_cursor(points, x, y, app);
		app.BindBuffers();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(app.GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(app.GetWindow()) == 0);
}