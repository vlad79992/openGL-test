export module Examples:draw_line;

import Examples;

import <Application.h>;
import <vector>;

struct simplePoint
{
	float x, y;
};

export void Example::draw_line()
{
	Application app;
	app.CreateWindow(1280, 720, "Test");
	app.SetSwapInterval(1);
	app.EnableDepthTest();


	// Create and compile our GLSL program from the shaders
	app.AddShaders("resources/shaders/SimpleVertexShader.vertexshader", "resources/shaders/SimpleFragmentShader.fragmentshader");
	//create camera
	app.CreateCamera();

	app.BindBuffers();
	app.SetBackgroundColor(0.1f, 0.2f, 0.7f, 0.0f);

	app.SetView(glm::lookAt(
		glm::vec3(0, 0, -1), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	));
	app.SetProjection(glm::ortho(-1.0f, 1.0f, -1.0f * ((float)9 / 16), 1.0f * ((float)9 / 16), 0.1f, 100.0f));
	double x = 0, y = 0;
	Point p;
	p.radius = 0.005f;
	std::vector<Point> points;
	glfwSetCursorPos(app.GetWindow(), 640, 360);
	do {
		app.Render();
		app.GetCursorPosition(x, y);
		//std::cout << x << '\t' << y << std::endl;
		app.ClearVerticies();
		x = x / 640 - 1;
		y = ((float)9 / 16) * ((y) / 360 - 1);
		p.x = -x, p.y = -y;
		///
		if (glfwGetMouseButton(app.GetWindow(), 0) == GLFW_PRESS)
		{
			Point tmp;
			tmp = p.GetCopy();
			if (!points.empty() && tmp.x == points.back().x && tmp.x == points.back().x);
			else
				points.push_back(p);
		}
		///
		if (points.size() > 0)
		{
			Point prev = points[0];
			for (int i = 1; i < points.size(); ++i)
			{
				float xdiff = points[i].x - points[i - 1].x;
				float ydiff = points[i].y - points[i - 1].y;
				app.SetVerticesAndColors(points[i].Create2DCircle(), points[i].CreateColorArray(1.0f, 1.0f, 1.0f), points[i].draw_segments * 3 * 3, 1);
				Point p1 = points[i].GetCopy(), p2 = points[i].GetCopy();
				float d = points[i - 1].radius / sqrt(xdiff * xdiff + ydiff * ydiff);
				p1.y += -d * xdiff, p1.x +=  d * ydiff;
				p2.y +=  d * xdiff, p2.x += -d * ydiff;
				//app.SetVerticesAndColors(p1.Create2DCircle(), p1.CreateColorArray(1.0f, 0.0f, 0.0f), p1.draw_segments * 3 * 3, 1);
				//app.SetVerticesAndColors(p2.Create2DCircle(), p2.CreateColorArray(1.0f, 0.0f, 0.0f), p2.draw_segments * 3 * 3, 1);
				float col[18];
				for (int i = 0; i < 18; ++i)
					col[i] = 1.0f;
				float pos[] =
				{
					(float)p2.x,			   (float)p2.y,	               0.0f,
					(float)p1.x,			   (float)p1.y,	               0.0f,
					(float)prev.x - d * ydiff, (float)prev.y + d * xdiff,  0.0f,
					(float)p1.x,			   (float)p1.y,	               0.0f,
					(float)prev.x - d * ydiff, (float)prev.y + d * xdiff,  0.0f,
					(float)prev.x + d * ydiff, (float)prev.y - d * xdiff,  0.0f
				};
				app.SetVerticesAndColors(pos, col, 18, 0);
				prev = points[i];
			}
			float xdiff = p.x - points.back().x;
			float ydiff = p.y - points.back().y;
			app.SetVerticesAndColors(points.back().Create2DCircle(), points.back().CreateColorArray(1.0f, 1.0f, 1.0f), points.back().draw_segments * 3 * 3, 1);
			Point p1 = points.back().GetCopy(), p2 = points.back().GetCopy();
			float d = p.radius / sqrt(xdiff * xdiff + ydiff * ydiff);
			p1.y += -d * xdiff, p1.x +=  d * ydiff;
			p2.y +=  d * xdiff, p2.x += -d * ydiff;
			app.SetVerticesAndColors(p1.Create2DCircle(), p1.CreateColorArray(1.0f, 0.0f, 0.0f), p1.draw_segments * 3 * 3, 1);
			app.SetVerticesAndColors(p2.Create2DCircle(), p2.CreateColorArray(1.0f, 0.0f, 0.0f), p2.draw_segments * 3 * 3, 1);
		}
		app.SetVerticesAndColors(p.Create2DCircle(), p.CreateColorArray(1.0f, 1.0f, 1.0f), p.draw_segments * 3 * 3, 1);
		app.BindBuffers();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(app.GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(app.GetWindow()) == 0);
}