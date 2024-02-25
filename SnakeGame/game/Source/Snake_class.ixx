export module Snake;

import <list>;

import <Application.h>;

export struct SnakeSegment : Point
{
	bool inUse = true;
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
	Snake(int resx, int resy)
	{
		aspect_ratio = (float)resy / resx;
		defaultSegment.radius = 0.05f;
		defaultSegment.r = 0.78f;
		defaultSegment.g = 0.75f;
		defaultSegment.b = 0.9f;
	}
	void AddFirst(float xpos, float ypos);
	void AddFirst(float distance);
	void RemoveLast();
	void IncreaseRadius(float val);
	void AddDirection(float val);
	void AddFood(float val);
	float GetVelocity();
	std::list<SnakeSegment> segments;
	SnakeSegment defaultSegment; //specifies scale and color of all segments
protected:
	bool isShifting = false;
	float foodCount = 0.0f;
	float direction = 0.0f;
	float velocity = 0.1f;
	float rotationSpeed = 1;
private:
	float aspect_ratio;
	float updateTime = 0.013, currentTime = 0.0f;
};

void Snake::AddFirst(float xpos, float ypos)
{
	SnakeSegment segment = defaultSegment;
	segment.x = xpos, segment.y = ypos;
	this->segments.push_front(segment);
}



void Snake::AddFirst(float distance)
{
	SnakeSegment segment = segments.front();
	segment.radius = defaultSegment.radius;
	segment.x += distance * cos(direction), segment.y += distance * sin(direction);
	if (segment.x > 1 + segment.radius)
		segment.x = -1 - segment.radius;
	if (segment.x < -1 - segment.radius)
		segment.x = 1 + segment.radius;
	if (segment.y > aspect_ratio + segment.radius)
		segment.y = -aspect_ratio - segment.radius;
	if (segment.y < - aspect_ratio - segment.radius)
		segment.y = aspect_ratio + segment.radius;


	if (foodCount > 1)
	{
		foodCount -= 1;
		IncreaseRadius(1e-4);
	}
	else
	{
		segments.pop_back();
	}

	if (foodCount < -1 && segments.size() > 1)
	{
		IncreaseRadius(-1e-4);
		segments.pop_back();
		foodCount += 1;
	}
	this->segments.push_front(segment);

	if (isShifting && segments.size() > 3)
	{
		isShifting = false;
		SnakeSegment segment = segments.front();
		segment.radius = defaultSegment.radius;
		segment.x += distance * cos(direction), segment.y += distance * sin(direction);
		if (segment.x > 1 + segment.radius)
			segment.x = -1 - segment.radius;
		if (segment.x < -1 - segment.radius)
			segment.x = 1 + segment.radius;
		if (segment.y > aspect_ratio + segment.radius)
			segment.y = -aspect_ratio - segment.radius;
		if (segment.y < -aspect_ratio - segment.radius)
			segment.y = aspect_ratio + segment.radius;
		foodCount -= 0.15;
		this->segments.push_front(segment);
		segments.pop_back();
	}
}

void Snake::RemoveLast()
{
	this->segments.pop_back();
}

void Snake::IncreaseRadius(float val)
{
	defaultSegment.radius += val;
	//for (auto& segment : segments)
	//{
	//	segment.radius = defaultSegment.radius;
	//}
	rotationSpeed -= val * (1 - val * 10);
}

void Snake::AddDirection(float val)
{
	this->direction += val * rotationSpeed;
}

void Snake::AddFood(float val)
{
	foodCount += val * 3;
	//this->IncreaseRadius(val / 10000);
}

float Snake::GetVelocity()
{
	return this->velocity;
}
