export module SnakeGame;

import <map>;
import <set>;

import <Application.h>;

import Snake;
import Food;

export class SnakeGame
{
public:
	void CreateWindow(int resx, int resy, std::string name); //creates window
	void Run(); //enter main loop of game
protected:
	Application app;
	void onUpdate(float deltaTime);
private:
	std::vector<Snake> snakes;
	std::list<Food> food;
	std::unordered_map<std::string, int> keys;
	int resx = 800, resy = 600;
	void createEntities();
	void setInput();
	void drawEntities();
	void getInput(float deltaTime);
	void moveFood(float deltaTime);
	void checkSnakeCollisions();

	int max_food_size = 25;
};
