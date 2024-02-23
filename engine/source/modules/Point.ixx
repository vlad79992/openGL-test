export module Point;

import <cmath>;

export struct Point
{
	long long ID = 0;
	int state = 0;
	double r = 0, g = 0, b = 0;
	double x = 0, y = 0;
	double radius = 0;
	double x_vel = 0, y_vel = 0;
	int draw_segments = 24;
	float* Create2DCircle()
	{
		double pi = 3.1415926535;
		float* vert = (float*)malloc(sizeof(float) * draw_segments * 3 * 3);
		for (int i = 0; i < draw_segments && vert != nullptr; ++i)
		{
			//std::cout << i + 1 << ')' << std::endl;
			vert[i * 9 + 0] = x;
			vert[i * 9 + 1] = y;
			vert[i * 9 + 2] = 0.0f;
			//std::cout << vert[i * 9 + 0] << ' ' << vert[i * 9 + 1] << ' ' << vert[i * 9 + 2] << std::endl;

			vert[i * 9 + 3] = x + radius * cos(pi * (float)i * 2 / draw_segments);
			vert[i * 9 + 4] = y + radius * sin(pi * (float)i * 2 / draw_segments);
			vert[i * 9 + 5] = 0.0f;
			//std::cout << vert[i * 9 + 3] << ' ' << vert[i * 9 + 4] << ' ' << vert[i * 9 + 5] << std::endl;

			vert[i * 9 + 6] = x + radius * cos(pi * (float)(i + 1) * 2 / draw_segments);
			vert[i * 9 + 7] = y + radius * sin(pi * (float)(i + 1) * 2 / draw_segments);
			vert[i * 9 + 8] = 0.0f;
			//std::cout << vert[i * 9 + 6] << ' ' << vert[i * 9 + 7] << ' ' << vert[i * 9 + 8] << std::endl;
			//std::cout << "------------" << std::endl;
		}
		return vert;
	}
	float* CreateColorArray(float r, float g, float b)
	{
		float* color = (float*)malloc(sizeof(float) * draw_segments * 3 * 3);
		for (int i = 0; i < draw_segments * 9 && color != nullptr; i += 3)
			color[i] = r, color[i + 1] = g, color[i + 2] = b;
		return color;
	}

	struct HashFunction
	{
		size_t operator()(const Point& point) const
		{
			return point.ID;
		}
	};
	Point GetCopy()
	{
		Point p;
		p.ID = this->ID;
		p.x = this->x;
		p.y = this->y;
		p.x_vel = this->x_vel;
		p.y_vel = this->y_vel;
		p.radius = this->radius;
		p.state = this->state;
		return p;
	}
};