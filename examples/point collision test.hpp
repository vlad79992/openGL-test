#pragma once

#include "../engine/source/Application.h"

#include <iostream>
#include <math.h>
#include <random>
#include <map>

#define __max(a,b) (((a) > (b)) ? (a) : (b))
#define __min(a,b) (((a) < (b)) ? (a) : (b))
#define __abs(a)   (((a) <= (0)) ? (-a) : (a))

int row_max, column_max;

double max_vel = 0.0065;
bool act = 0;
void update_points(std::vector<Point>& pts, Application& app)
{
	//act = !act;
	double /*max_vel = 0.0065,*/ static_damping = 1 - 1e-5, collision_damping = 0.85;
	double gravity = 0.0001;
	////Point *cur = nullptr;// , * next = nullptr;
	//for (int i = 0; i < pts.size(); ++i)
	//{
	//	auto tmp = pts[i].begin();
	//	for (int j = 0; j < pts[i].size(); ++j)
	//	//for (Point& cur : pts[i])
	//	{
	//		Point& cur = *tmp;
	//		int row = i / column_max, column = i % column_max;
	//		for (int r = row - 1; r <= row + 1; ++r)
	//		{
	//			if (r < 0 || r >= row_max) continue;
	//			for (int c = column - 1; c <= column + 1; ++c)
	//			{
	//				if (c < 0 || c >= column_max) continue;
	//				for (Point& next : pts[r * column_max + c])
	//				{
	//					if (next.ID == cur.ID) continue;
	//					if ((cur.x - next.x) * (cur.x - next.x) + (cur.y - next.y) * (cur.y - next.y) <= (cur.radius + next.radius) * (cur.radius + next.radius))
	//					{
	//						float Dist = sqrt((cur.x - next.x) * (cur.x - next.x) + (cur.y - next.y) * (cur.y - next.y));
	//						Dist = (Dist < cur.radius / 10) ? (cur.radius / 10) : (Dist);
	//						float Overlap = (Dist - cur.radius - next.radius) * 0.5f;
	//						//fixing overlap
	//						cur.x -= Overlap * (cur.x - next.x) / Dist;
	//						cur.y -= Overlap * (cur.y - next.y) / Dist;
	//
	//						next.x -= Overlap * (next.x - cur.x) / Dist;
	//						next.y -= Overlap * (next.y - cur.y) / Dist;
	//
	//						//normal vector of 2 points
	//						double nx = (next.x - cur.x) / (Dist);
	//						double ny = (next.y - cur.y) / (Dist);
	//						//tangent vector of 2 points
	//						double tx = -ny;
	//						double ty = nx;
	//						//dot product
	//						double dp1 = cur.x_vel * tx + cur.y_vel * ty;
	//						double dp2 = next.x_vel * tx + next.y_vel * ty;
	//						double dp3 = cur.x_vel * nx + cur.y_vel * ny;
	//						double dp4 = next.x_vel * nx + next.y_vel * ny;
	//
	//						double m1 = (2.0f * dp4) / 2;
	//						double m2 = (2.0f * dp3) / 2;
	//
	//						cur.x_vel = (tx * dp1 + nx * m1) * collision_damping;
	//						cur.y_vel = (ty * dp1 + ny * m1) * collision_damping;
	//						next.x_vel = (tx * dp2 + nx * m2) * collision_damping;
	//						next.y_vel = (ty * dp2 + ny * m2) * collision_damping;
	//					}
	//				}
	//			}
	//		}
	//		//changing position depending on velocity
	//		if (cur.x_vel > max_vel)
	//			cur.x_vel = max_vel;
	//		if (cur.y_vel > max_vel)
	//			cur.y_vel = max_vel;
	//		if (cur.x_vel < -max_vel)
	//			cur.x_vel = -max_vel;
	//		if (cur.y_vel < -max_vel)
	//			cur.y_vel = -max_vel;
	//		if (cur.state != act)
	//			cur.state = act;
	//		else
	//			continue;
	//		//if (p->y >= -(9.0f / 16.0f) + p->radius * 2)
	//		//	p->y_vel -= gravity;
	//
	//		cur.x_vel *= static_damping, cur.y_vel *= static_damping;
	//		if ((cur.x > 1.0f && cur.x_vel > 0) || (cur.x < -1.0f && cur.x_vel < 0))
	//			cur.x_vel *= -1;
	//		if ((cur.y > (9.0f / 16.0f) && cur.y_vel > 0) || (cur.y < -(9.0f / 16.0f) && cur.y_vel < 0))
	//			cur.y_vel *= -1;
	//		cur.x += cur.x_vel;
	//		cur.y += cur.y_vel;
	//		//add point to render
	//		app.SetVerticesAndColors(cur.Create2DCircle(), cur.CreateColorArray(1.0f, 1.0f, 1.0f), cur.draw_segments * 3 * 3);
	//		//change segment of point
	//		int new_row = (int)((cur.x + 1) / max_vel * 2),
	//			new_column = (int)((cur.y + (double)9 / 16) / max_vel * 2);
	//		if ((row != new_row || new_column != column) && (new_column >= 0 && new_column < column_max) && (new_row >= 0 && new_row < row_max))
	//		{
	//			//std::cout << (new_row >= 0 && new_row < row_max) << ' ' << (new_column >= 0 && new_column < column_max) << std::endl;
	//			Point copy = cur;
	//			pts[new_row * column_max + new_column].push_front(copy);
	//			pts[i].erase(tmp++);
	//		}
	//		else
	//			tmp++;
	//	}
	//}
	for (int i = 0; i < pts.size(); ++i)
	{
		Point& p = pts[i];
		for (int j = i + 1; j < pts.size(); ++j)
		{
			//if (j == i) continue;
			float x1 = p.x, x2 = p.x + p.x_vel, y1 = p.y, y2 = p.y + p.y_vel;
			float x3 = pts[j].x, x4 = pts[j].x + pts[j].x_vel, y3 = pts[j].y, y4 = pts[j].y + pts[j].y_vel;
			if ((x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3) <= (p.radius * 2) * (p.radius * 2))
			{
				float Dist = sqrt((x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3));
				Dist = (Dist < p.radius / 4) ? (p.radius / 4) : (Dist);
				float Overlap = (Dist - p.radius * 2) * 0.5f;
	
				p.x -= Overlap * (p.x - pts[j].x) / (Dist);
				p.y -= Overlap * (p.y - pts[j].y) / (Dist);
				
				pts[j].x -= Overlap * (pts[j].x - p.x) / (Dist);
				pts[j].y -= Overlap * (pts[j].y - p.y) / (Dist);
				//
				//normal vector of 2 points
				double nx = (pts[j].x - p.x) / (Dist);
				double ny = (pts[j].y - p.y) / (Dist);
				//tangent vector of 2 points
				double tx = -ny;
				double ty = nx;
				//dot product
				double dp1 = p.x_vel * tx + p.y_vel * ty;
				double dp2 = pts[j].x_vel * tx + pts[j].y_vel * ty;
				double dp3 = p.x_vel * nx + p.y_vel * ny;
				double dp4 = pts[j].x_vel * nx + pts[j].y_vel * ny;
				
				double m1 = (2.0f * dp4) / 2;
				double m2 = (2.0f * dp3) / 2;
	
				p.x_vel = (tx * dp1 + nx * m1) * collision_damping;
				p.y_vel = (ty * dp1 + ny * m1) * collision_damping;
				pts[j].x_vel = (tx * dp2 + nx * m2) * collision_damping;
				pts[j].y_vel = (ty * dp2 + ny * m2) * collision_damping;
			}
		}
		if (p.x_vel > max_vel)
			p.x_vel = max_vel;
		if (p.y_vel > max_vel)
			p.y_vel = max_vel;
		if (p.x_vel < -max_vel)
			p.x_vel = -max_vel;
		if (p.y_vel < -max_vel)
			p.y_vel = -max_vel;
		p.x += p.x_vel;
		p.y += p.y_vel;
		//if (p->y >= -(9.0f / 16.0f) + p->radius * 2)
		//	p->y_vel -= gravity;
	
		p.x_vel *= static_damping, p.y_vel *= static_damping;
		if ((p.x > 1.0f && p.x_vel > 0) || (p.x < -1.0f && p.x_vel < 0))
			p.x_vel *= -1;
		if ((p.y > (9.0f / 16.0f) && p.y_vel > 0) || (p.y < -(9.0f / 16.0f) && p.y_vel < 0))
			p.y_vel *= -1;
		app.SetVerticesAndColors(p.Create2DCircle(), p.CreateColorArray(1.0f, 1.0f, 1.0f), p.draw_segments * 3 * 3);
	}
}

inline void point_collision_test()
{
	//random
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist_x(-1.0f, 1.0f);
	std::uniform_real_distribution<> dist_y(-(9.0f / 16.0f), (9.0f / 16.0f));
	//window creation
	Application app;
	app.CreateWindow(1280, 720, "Test");
	app.SetSwapInterval(1);
	app.EnableDepthTest();



	// Create and compile our GLSL program from the shaders
	app.AddShaders("resources/shaders/SimpleVertexShader.vertexshader", "resources/shaders/SimpleFragmentShader.fragmentshader");
	//create camera
	app.CreateCamera();

	//app.SetVerticesAndColors(g_vertex_buffer_data, g_color_buffer_data, 108);
	//app.AddTriangle(Vertex(-1.0f, -(9.0f / 16.0f), 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
					//Vertex( 1.0f, -(9.0f / 16.0f), 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
					//Vertex( 0.0f,  (9.0f / 16.0f), 0.0f, 0.0f, 0.0f, 0.1f, 1.0f));
	row_max = (int)ceil((1 * 2) / max_vel * 2), column_max = ceil(((float)9 / 16 * 2) / max_vel * 2);
	std::vector/*<std::vector<Point>>*/<Point> points/*((row_max * column_max), std::vector<Point>())*/;
	//std::vector<Point> points;
	for (int i = 0; i < 2500; ++i) //generate 100 random points
	{
		Point p;
		p.radius = 0.0065f;
		p.draw_segments = 10;
		p.x = dist_x(gen);
		p.y = dist_y(gen);
		//std::cout << p.x << ' ' << p.y << std::endl;
		app.SetVerticesAndColors(p.Create2DCircle(), p.CreateColorArray(1.0f, 1.0f, 1.0f), p.draw_segments * 3 * 3);
		p.x_vel = dist_x(gen) / 5000;
		p.y_vel = dist_x(gen) / 5000;
		p.ID = i;
		p.state = 0;
		//TODO проверить правильность этого говна
		//видимо не удалось
		//int row = (int)((p.x + 1) / max_vel * 2),
		//	column = (int)((p.y + (double)9 / 16) / max_vel * 2);
		//if (row < 0 || column < 0 || row * column_max + column >= points.size())
		//	std::cout << row << ' ' << column << std::endl;
		//points[row * column_max + column].emplace_back(p);
		points.push_back(p);
	}

	app.BindBuffers();
	app.SetBackgroundColor(0.1f, 0.2f, 0.7f, 0.0f);



	app.SetView(glm::lookAt(
		glm::vec3(0, 0, -1), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0,  0), // and looks at the origin
		glm::vec3(0, 1,  0)  // Head is up (set to 0,-1,0 to look upside-down)
	));
	app.SetProjection(glm::ortho(-1.0f, 1.0f, -1.0f * ((float)9 / 16), 1.0f * ((float)9 / 16), 0.1f, 100.0f));
	do {
		app.Render();
		app.ClearVerticies();
		update_points(points, app);
		app.BindBuffers();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(app.GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(app.GetWindow()) == 0);

}