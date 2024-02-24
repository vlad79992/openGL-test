export module SnakeGame:on_update;

import <iostream>;
import <Application.h>;

import SnakeGame;

void SnakeGame::onUpdate(float deltaTime)
{
	drawSnakes();
	this->app.BindBuffers();
	this->app.Render();
	getInput();
}

void SnakeGame::Run()
{
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

void SnakeGame::drawSnakes()
{
	int cnt = 0;
	for (auto& snake : snakes)
	{
		for (auto& segment : snake.segments)
		{
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
}

void SnakeGame::getInput()
{
	float x_incr = 0, y_incr = 0;
	if (glfwGetKey(this->app.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		x_incr -= 0.01;
	}
	if (glfwGetKey(this->app.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		x_incr += 0.01;
	}
	if (glfwGetKey(this->app.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		y_incr += 0.01;
	}
	if (glfwGetKey(this->app.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		y_incr -= 0.01;
	}
	this->snakes.front().AddFirst(snakes.front().segments.front().x + x_incr, snakes.front().segments.front().y + y_incr);
	this->snakes.front().RemoveLast();
}