#include "../examples/rotating cube.hpp"
#include "../examples/GLFW cursor test.hpp"
#include "../examples/point collision test.hpp"
#include "../examples/points follow cursor.hpp"



#ifdef _DEBUG
int main(void)
#else
int WinMain()
#endif // _DEBUG
{
	//rotating_cube();
	//cursor_pos_test();
	//point_collision_test();
	points_follow_cursor();
	return 0;
}
