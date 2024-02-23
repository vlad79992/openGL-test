#include "../engine/source/Application.h"
//#include <Application.h>

#include <vector>
#include <random>
#include <thread>
#include <mutex>


void Create2DCircle_Direct(Point p, Application& app, int begin)
{
	double pi = 3.1415926535;
	for (int i = 0; i < p.draw_segments; ++i)
	{
		app.pos[begin + i * 9 + 0] = (float)p.x;
		app.pos[begin + i * 9 + 1] = (float)p.y;
		app.pos[begin + i * 9 + 2] = 0.0f;

		app.pos[begin + i * 9 + 3] = (float)p.x + p.radius * cos(pi * (float)i * 2 / p.draw_segments);
		app.pos[begin + i * 9 + 4] = (float)p.y + p.radius * sin(pi * (float)i * 2 / p.draw_segments);
		app.pos[begin + i * 9 + 5] = 0.0f;

		app.pos[begin + i * 9 + 6] = (float)p.x + p.radius * cos(pi * (float)(i + 1) * 2 / p.draw_segments);
		app.pos[begin + i * 9 + 7] = (float)p.y + p.radius * sin(pi * (float)(i + 1) * 2 / p.draw_segments);
		app.pos[begin + i * 9 + 8] = 0.0f;
	}
}

void Create2DCircleColor_Direct(Point p, Application& app, int begin)
{
	for (int i = 0; i < p.draw_segments * 9; i += 3)
		app.col[begin + i] = (float)p.r, app.col[begin + i + 1] = (float)p.g, app.col[begin + i + 2] = (float)p.b;
}

double c = 0.01;
int defaultx = 1280, defaulty = 720;
bool fullscreen = false;
int resx = 1600, resy = 900;
int thread_count = std::thread::hardware_concurrency(), points_count = 100000;

void points_and_cursor(std::vector<Point>& points, double xpos, double ypos, Application& app)
{
	std::mutex my_mutex;
	double max_vel = 0.065, static_damping = 1 - 1e-2;
	auto f = [&](int begin, int end)
	{
		for (int i = begin; i <= end; ++i)
		{
			Point& p = points[i];
			double dist = ((p.x - xpos) * (p.x - xpos) + (p.y - ypos) * (p.y - ypos));
			if (dist < (c / 150) * (c / 150))
			{
				double to_cursor_x = (1) * (c) / dist * (p.x - xpos) * 1e-5;
				double to_cursor_y = (1) * (c) / dist * (p.y - ypos) * 1e-5;

				p.x_vel += to_cursor_x;
				p.y_vel += to_cursor_y;
				//add rondomness
				p.x_vel += c * 0.75e-4 * ((double)rand() / (RAND_MAX)) - (c * 0.75e-4) / 2;
				p.y_vel += c * 0.75e-4 * ((double)rand() / (RAND_MAX)) - (c * 0.75e-4) / 2;
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

			//app.SetVerticesAndColors(p.Create2DCircle(), p.CreateColorArray(p.r, p.g, p.b), p.draw_segments * 3 * 3, my_mutex);
			Create2DCircle_Direct(p, app, i * 5 * 3 * 3);
			Create2DCircleColor_Direct(p, app, i * 5 * 3 * 3);
			//app.SetVerticesAndColors(p.Create2DCircle(), p.CreateColorArray(p.r, p.g, p.b), p.draw_segments * 3 * 3);
			//if (p->y >= -(9.0f / 16.0f) + p->radius * 2)
			//	p->y_vel -= gravity;

			p.x_vel *= static_damping, p.y_vel *= static_damping;
			if ((p.x > (1.0f - 0.025) && p.x_vel > 0) || (p.x < (-1.0f + 0.025) && p.x_vel < 0))
				p.x_vel *= -0.75;
			if ((p.y > (((float)resy / resx) - 0.025) && p.y_vel > 0) || ((p.y < -((float)resy / resx) + 0.025) && p.y_vel < 0))
				p.y_vel *= -0.75;
		}
	};
	std::vector<std::thread> threads;
	for (int i = 0; i < thread_count; ++i)
	{
		int begin = i * (points_count / thread_count);
		int end = ((i + 1) * (points_count / thread_count) - 1 < points_count - 1) ? ((i + 1) * (points_count / thread_count) - 1) : (points_count - 1);
		if (i == thread_count - 1)
			end = points_count - 1;
		threads.push_back(std::thread(f, begin, end));
	}
	for (int i = 0; i < thread_count; ++i)
		threads[i].join();
}

inline void points_follow_cursor()
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
	auto f = [&]()
		{
		points.clear();
		//points.resize(points_count);
		delete(app.pos);
		delete(app.col);
		app.pos = (float*)malloc(sizeof(float) * 5 * 3 * 3 * points_count);
		app.col = (float*)malloc(sizeof(float) * 5 * 3 * 3 * points_count); //every point hav 5 triangle every triangle is 3 points every point is 3 positions
		for (int i = 0; i < points_count; ++i) //generate 100 random points
		{
			Point p;
			p.radius = 0.00125f;
			p.draw_segments = 5;
			p.x = dist_x(gen);
			p.y = dist_y(gen);
			p.r = (p.x + 1) / 2, p.b = (p.y + 1) / 2, p.g = p.x * p.y;
			//app.SetVerticesAndColors(p.Create2DCircle(), p.CreateColorArray(p.r, p.g, p.b), p.draw_segments * 3 * 3);
			Create2DCircle_Direct(p, app, i * 5 * 3 * 3);
			Create2DCircleColor_Direct(p, app, i * 5 * 3 * 3);
			p.x_vel = dist_x(gen) / 500;
			p.y_vel = dist_x(gen) / 500;
			p.ID = i;
			p.state = 0;
			points.push_back(p);
		}
	};
	f();
	app.BindRawBuffers(5 * 3 * 3 * points_count);
	app.SetBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);



	app.SetView(glm::lookAt(
		glm::vec3(0, 0, -1), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	));
	double x, y;
	app.SetProjection(glm::ortho(-1.0f, 1.0f, -1.0f * ((float)resy / resx), 1.0f * ((float)resy / resx), 0.1f, 100.0f));
	Timer tmr;
	do {
		tmr.start();
		app.Render();
		//app.ClearVerticies();
		app.GetCursorPosition(x, y);
		if (glfwGetMouseButton(app.GetWindow(), 0) == GLFW_PRESS)
			c = 25;
		else if (glfwGetMouseButton(app.GetWindow(), 1) == GLFW_PRESS)
			c = 0;
		else
			c = 7.5;
		if (glfwGetMouseButton(app.GetWindow(), 2) == GLFW_PRESS)
			c *= -1;
		if (glfwGetKey(app.GetWindow(), GLFW_KEY_R) == GLFW_PRESS)
			f();
		//if (x > resx || x < 0) x = resx / 2;
		//if (y > resy  || y < 0) y = resy / 2;
		x = (x / (resx / 2) - 1) * -1;
		y = (((float)resy / resx) * ((y) / (resy / 2) - 1)) * -1;
		points_and_cursor(points, x, y, app);
		app.BindRawBuffers(5 * 3 * 3 * points_count);
		//app.BindBuffers();
		//set fps in window title
		tmr.stop();
		double fps = 1000.0 / tmr.elapsedMilliseconds();
		std::stringstream ss;
		ss << "window" << " " << " [" << fps << " FPS]";

		glfwSetWindowTitle(app.GetWindow(), ss.str().c_str());
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(app.GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(app.GetWindow()) == 0);
}