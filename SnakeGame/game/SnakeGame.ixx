export module SnakeGame;

//export import :cube;
import <Application.h>;

import Snake;

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
	void drawSnakes();
	void createEntities();
	void getInput();
};
