export module Snake;

import <list>;

import <Application.h>;

export struct SnakeSegment : Point
{
	float r_center = 1.0f, g_center = 1.0f, b_center = 1.0f;
	float* CreateColorArray()
	{
		float* color = (float*)malloc(sizeof(float) * draw_segments * 3 * 3);
		for (int i = 0; i < draw_segments * 9 && color != nullptr; i += 3)
		{
			if (i % 9 >= 0 && i % 9 <= 2)
				color[i] = r, color[i + 1] = g, color[i + 2] = b;
			else
				color[i] = r_center, color[i + 1] = g_center, color[i + 2] = b_center;
		}
		return color;
	}
	void AddToApp(Application& app)
	{
		app.SetVerticesAndColors(Create2DCircle(), CreateColorArray(), this->draw_segments * 9, 1);
	}
	void SetAtApp(Application& app, int begin)
	{
		float *positions = Create2DCircle(), *colors = CreateColorArray();
		for (int i = 0; i < this->draw_segments * 9; ++i)
		{
			app.VerticiesSetAt(begin + i, positions[i]);
			app.ColorsSetAt(begin + i, colors[i]);
		}
		delete(positions);
		delete(colors);
	}
};

export class Snake
{
friend class SnakeGame;
public:
	Snake()
	{
		defaultSegment.radius = 0.05f;
		defaultSegment.r = 0.78f;
		defaultSegment.g = 0.75f;
		defaultSegment.b = 0.9f;
	}
	void AddFirst(float xpos, float ypos);
	void RemoveLast();
	void IncreaseRadius(float val);
protected:
	std::list<SnakeSegment> segments;
	SnakeSegment defaultSegment; //specifies scale and color of all segments
	
private:
};

void Snake::AddFirst(float xpos, float ypos)
{
	SnakeSegment segment = defaultSegment;
	segment.x = xpos, segment.y = ypos;
	this->segments.push_front(segment);
}

void Snake::RemoveLast()
{
	this->segments.pop_back();
}

void Snake::IncreaseRadius(float val)
{
	defaultSegment.radius += val;
	for (auto& segment : segments)
	{
		segment.radius = defaultSegment.radius;
	}
}