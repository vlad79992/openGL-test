export module SnakeGame:on_update;

import <iostream>;
import <Application.h>;
import <random>;

import SnakeGame;
import Snake;

float aspect_ratio = 0.0f;

void moveSnake(Snake& snake, float deltaTime);

void SnakeGame::onUpdate(float deltaTime)
{

	drawEntities();
	this->app.BindBuffers();
	this->app.Render();
	getInput(deltaTime);

	for (Snake& snake : snakes)
		moveSnake(snake, deltaTime);
	moveFood(deltaTime);
	this->checkSnakeCollisions();
}

void SnakeGame::Run()
{
	aspect_ratio = (float)(this->resy) / (this->resx);

	Timer timer;
	float deltaTime = 0.16f;
	createEntities();
	do 
	{
		timer.start();
		this->onUpdate(deltaTime);
		timer.stop();
		deltaTime = timer.elapsedSeconds();
	} while (glfwGetKey(app.GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && //if escape is pressed window closes
		glfwWindowShouldClose(app.GetWindow()) == 0);                     //checks if window closes
}

auto _pow2 = [](double a) { return a * a; };

void SnakeGame::drawEntities()
{
	int cnt = 0;
	
	
	for (Snake& snake : snakes)
	{
		SnakeSegment* prevSegment = nullptr;
		double dist_incr = 0;
		double prev_x = 0, prev_y = 0;
		for (SnakeSegment& segment : snake.segments)
		{
			if (prevSegment == nullptr)
			{
				segment.inUse = 1;
				prevSegment = &segment;
				prev_x = segment.x, prev_y = segment.y;
			}
			else
			{

				//TODO что-то сломалось
				double tmp = sqrt(_pow2(prev_x - segment.x) + _pow2(prev_y - segment.y));
				dist_incr += tmp;
				//std::cout << sqrt(_pow2(prevSegment->x - segment.x) + _pow2(prevSegment->y - segment.y)) << '\t' << prevSegment->radius * 0.5 << std::endl;
				if (dist_incr > prevSegment->radius * 0.8)
				{
					segment.inUse = 1;
					dist_incr -= prevSegment->radius;
					prevSegment = &segment;
					prev_x = segment.x, prev_y = segment.y;
				}
				else
				{
					segment.inUse = 0;
					continue;
				}
			}

			cnt += segment.draw_segments * 9;
			if (cnt > app.getVerticiesSize())
			{
				if (app.getVerticiesSize() != cnt - segment.draw_segments * 9 && cnt - segment.draw_segments * 9 > 0)
				{
					app.ResizeVerticies(cnt - segment.draw_segments * 9);
					app.ResizeColors(cnt - segment.draw_segments * 9);
				}
				segment.AddToApp(this->app);
			}
			else
			{
				segment.SetAtApp(this->app, cnt - segment.draw_segments * 9);
			}
		}
	}

	for (auto& food : food)
	{
		cnt += food.draw_segments * 9;
		if (cnt > app.getVerticiesSize())
		{
			if (app.getVerticiesSize() != cnt - food.draw_segments * 9 && cnt - food.draw_segments * 9 > 0)
			{
				app.ResizeVerticies(cnt - food.draw_segments * 9);
				app.ResizeColors(cnt - food.draw_segments * 9);
			}
			food.AddToApp(this->app);
		}
		else
		{
			food.SetAtApp(this->app, cnt - food.draw_segments * 9);
		}
	}
}

void SnakeGame::getInput(float deltaTime)
{
	float player1_rotate = 0.0f;
	float player2_rotate = 0.0f;
	float rotation_speed = 4.0f;
	if (glfwGetKey(this->app.GetWindow(), this->keys.at("player1_right")) == GLFW_PRESS)
		player1_rotate += rotation_speed;
	if (glfwGetKey(this->app.GetWindow(), this->keys.at("player1_left")) == GLFW_PRESS)
		player1_rotate -= rotation_speed;
	if (glfwGetKey(this->app.GetWindow(), this->keys.at("player2_right")) == GLFW_PRESS)
		player2_rotate += rotation_speed;
	if (glfwGetKey(this->app.GetWindow(), this->keys.at("player2_left")) == GLFW_PRESS)
		player2_rotate -= rotation_speed;
	snakes[0].AddDirection(player1_rotate * deltaTime);
	snakes[1].AddDirection(player2_rotate * deltaTime);
	
	if (glfwGetKey(this->app.GetWindow(), this->keys.at("player1_boost")) == GLFW_PRESS)
		snakes[0].isShifting = true;
	if (glfwGetKey(this->app.GetWindow(), this->keys.at("player2_boost")) == GLFW_PRESS)
		snakes[1].isShifting = true;

	//this->snakes.front().AddFirst(snakes.front().segments.front().x + x_incr, snakes.front().segments.front().y + y_incr);
	//this->snakes.front().RemoveLast();
}

void moveSnake(Snake& snake, float deltaTime)
{
	snake.AddFirst(snake.GetVelocity() * deltaTime);
}

void SnakeGame::moveFood(float deltaTime)
{
	auto addRandomFood = [&]()
	{
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
		std::uniform_real_distribution<> dist_y(-(aspect_ratio + 0.025), (aspect_ratio - 0.025));
		std::uniform_real_distribution<> food_dist(0.005f, 0.02f);
		std::uniform_int_distribution<>  color_dist(0, sizeof(colors) / sizeof(float) / 3 - 1);

		Food f;
		f.x = dist_x(gen);
		f.y = dist_y(gen);
		f.food_value = food_dist(gen) * 100;
		f.radius = f.food_value / 100;
		int index = color_dist(gen);
		f.r = colors[index * 3 + 0];
		f.g = colors[index * 3 + 1];
		f.b = colors[index * 3 + 2];
		food.push_back(f);
	};

	float	attractionDist = 0.15,
			attractionScale = 2.5f;

	std::list<Food>::iterator i = this->food.begin();
	while (i != this->food.end())
	{
		for (Snake& s : this->snakes)
		{
			float dist = (i->DistTo(s.segments.front().x, s.segments.front().y));
			if (dist < (s.defaultSegment.radius + i->radius) * 1.5)
			{
				bool first = i == this->food.begin();
				s.AddFood(i->food_value/* * 5*/);
				this->food.erase(i++);
				if (food.size() <= max_food_size)
					addRandomFood();
				if (!first)
					i--;
				break;
			}
			else if (dist < attractionDist)
			{
				i->x += (attractionDist - dist) * (s.segments.front().x - i->x) * deltaTime * attractionScale;
																							  
				i->y += (attractionDist - dist) * (s.segments.front().y - i->y) * deltaTime * attractionScale;
				
				if (i->x >  1) i->x -= 2;
				if (i->x < -1) i->x += 2;
				if (i->y >  resy / resx) i->y -= resy / resx * 2;
				if (i->y < -resy / resx) i->y += resy / resx * 2;
			}
		}
		++i;
	}
}

void Kill(Snake& snake, std::list<Food>& food);

void SnakeGame::checkSnakeCollisions()
{
	for (Snake& snake : this->snakes)
	{
		bool killed = false;
		for (Snake& other : this->snakes)
		{
			if (killed)
				break;
			if (&snake == &other)
				continue;

			for (SnakeSegment& segment : other.segments)
			{
				if (segment.inUse == false)
					continue;

				if (sqrt(_pow2(snake.segments.front().x - segment.x) + _pow2(snake.segments.front().y - segment.y)) <
					snake.segments.front().radius + segment.radius)
				{
					Kill(snake, this->food);
					killed = true;
					break;
				}
			}
		}
	}
}

void Kill(Snake& snake, std::list<Food>& food)
{
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
	std::uniform_real_distribution<> dist_y(-aspect_ratio + 0.025, (aspect_ratio - 0.025));
	std::uniform_real_distribution<> food_dist(0.005f, 0.01f);
	std::uniform_int_distribution<>  color_dist(0, sizeof(colors) / sizeof(float) / 3 - 1);


	for (SnakeSegment& segment : snake.segments)
	{
		if (segment.inUse == false)
			continue;
		Food f;
		f.x = segment.x + dist_x(gen) / 100;
		f.y = segment.y + dist_y(gen) / 100;
		f.food_value = food_dist(gen) * 100;
		f.radius = f.food_value / 100;
		int index = color_dist(gen);
		f.r = colors[index * 3 + 0];
		f.g = colors[index * 3 + 1];
		f.b = colors[index * 3 + 2];
		food.push_back(f);
	}

	snake.segments.clear();
	snake.defaultSegment.radius = 0.01f;
	snake.AddFirst(dist_x(gen), dist_y(gen));
}