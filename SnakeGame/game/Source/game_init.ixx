export module SnakeGame:init;

import <fstream>;
import <sstream>;
import <string>;

import <random>;

import <Application.h>;

import SnakeGame;

void SnakeGame::CreateWindow(int resx, int resy, std::string name)
{
	this->resx = resx, this->resy = resy;
	this->setInput();

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
	int playerCount = 2;

	float colors[]
	{
		0.518f, 0.0f, 1.0f,
		0.282f, 0.98f, 0.592f,
		1.0f, 0.569f, 0.0f, 
		0.0f, 1.0f, 0.969f, 
		1.0f, 0.541f, 0.741f
	};
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist_x(-1.0f + 0.025, 1.0f - 0.025);
	std::uniform_real_distribution<> dist_y(-((float)resy / resx) + 0.025, ((float)resy / resx) - 0.025);
	std::uniform_real_distribution<> food_dist(0.005f, 0.02f);
	std::uniform_int_distribution<>  color_dist(0, sizeof(colors) / sizeof(float) / 3 - 1);
	for (int i = 0; i < playerCount; ++i)
	{
		Snake snake(resx, resy);
		snake.defaultSegment.radius = 0.01f;
		snake.AddFirst(dist_x(gen), dist_y(gen));
		this->snakes.push_back(snake);
	}
	Food f;
	for (int i = 0; i < 20; ++i)
	{
		f.x = dist_x(gen);
		f.y = dist_y(gen);
		f.food_value = food_dist(gen) * 100;
		f.radius = f.food_value / 100;
		int index = color_dist(gen);
		f.r = colors[index * 3 + 0];
		f.g = colors[index * 3 + 1];
		f.b = colors[index * 3 + 2];
		food.push_back(f);
	}
}

void SnakeGame::setInput()
{
	std::fstream fs("resources/keys.txt");
	std::stringstream sstr;
	std::string line;
	while (std::getline(fs, line))
	{
		sstr << line;
		std::string action;
		int key;
		sstr >> action >> key;
		this->keys.insert({ action, key });
		sstr = {};
	}
}
