export module SnakeGame:init;

import <Application.h>;

import SnakeGame;

void SnakeGame::CreateWindow(int resx, int resy, std::string name)
{
	this->app.CreateWindow(resx, resy, name); //create window
	this->app.SetSwapInterval(1); //enable vsync
	this->app.EnableDepthTest();

	this->app.AddShaders("resources/shaders/SimpleVertexShader.vertexshader", "resources/shaders/SimpleFragmentShader.fragmentshader");
	this->app.CreateCamera();

	this->app.BindBuffers();
	this->app.SetBackgroundColor(0.4f, 0.4f, 0.4f, 0.0f);

	this->app.SetView(glm::lookAt(
		glm::vec3(0, 0, -1), // Camera is at (0, 0, -1), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
	));
	this->app.SetProjection(glm::ortho(-1.0f, 1.0f, -1.0f * ((float)resy / resx), 1.0f * ((float)resy / resx), 0.1f, 100.0f)); //set projection to screen aspect ratio
}

void SnakeGame::createEntities()
{
	Snake snake;
	snake.AddFirst(0.0f, 0.0f);
	this->snakes.push_back(snake);
}