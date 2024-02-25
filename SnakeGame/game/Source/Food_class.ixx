export module Food;

import <Application.h>;

export struct Food : Point
{
	friend class SnakeGame;
	float food_value = 0;
	void AddToApp(Application& app)
	{
		app.SetVerticesAndColors(Create2DCircle(), CreateColorArray(r, g, b), this->draw_segments * 9, 1);
	}
	void SetAtApp(Application& app, int begin)
	{
		float* positions = Create2DCircle(), * colors = CreateColorArray(r, g, b);
		for (int i = 0; i < this->draw_segments * 9; ++i)
		{
			app.VerticiesSetAt(begin + i, positions[i]);
			app.ColorsSetAt(begin + i, colors[i]);
		}
		delete(positions);
		delete(colors);
	}
	float DistTo(float x, float y)
	{
		return sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y));
	}
};