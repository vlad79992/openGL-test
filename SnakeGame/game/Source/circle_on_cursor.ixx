export module Examples:circle_on_cursor;

import <Application.h>;

export void circle_on_cursor()
{
	Application app;
	app.CreateWindow(1280, 720, "Test");
	app.SetSwapInterval(1);
	app.EnableDepthTest();


	// Create and compile our GLSL program from the shaders
	app.AddShaders("resources/shaders/SimpleVertexShader.vertexshader", "resources/shaders/SimpleFragmentShader.fragmentshader");
	//create camera
	app.CreateCamera();

	//app.SetVerticesAndColors(g_vertex_buffer_data, g_color_buffer_data, 108);
	app.AddTriangle(Vertex(-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.1f, 1.0f));


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
	p.radius = 0.1f;
	glfwSetCursorPos(app.GetWindow(), 640, 360);
	do {
		app.Render();
		app.GetCursorPosition(x, y);
		//std::cout << x << '\t' << y << std::endl;
		app.ClearVerticies();
		x = x / 640 - 1;
		y = ((float)9 / 16) * ((y) / 360 - 1);
		p.x = -x, p.y = -y;
		app.SetVerticesAndColors(p.Create2DCircle(), p.CreateColorArray(1.0f, 1.0f, 1.0f), p.draw_segments * 3 * 3, 1);
		app.BindBuffers();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(app.GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(app.GetWindow()) == 0);
}