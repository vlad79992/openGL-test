//#include "Examples.h"

import SnakeGame;

#ifdef _DEBUG
int main()
#else
int WinMain() //no console
#endif // _DEBUG
{
	SnakeGame game;
	game.CreateWindow(1280, 720, "Snake");
	game.Run();
	//example.circle_on_cursor();
	//example.point_collision();
	//example.rotating_cube();
	//example.draw_line();
	//example.textured_cube();
}
