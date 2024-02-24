//#include "Examples.h"

import Examples;

#ifdef _DEBUG
int main()
#else
int WinMain() //no console
#endif // _DEBUG
{
	Example example;
	example.point_repulsion();
	//example.circle_on_cursor();
	//example.point_collision();
	//example.rotating_cube();
	//example.draw_line();
	//example.textured_cube();
}
