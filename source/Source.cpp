#pragma comment (lib, "glew32s.lib")
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <iostream>

#include "Renderer.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(2);

    if (glewInit() != GLEW_OK) {
        std::cerr << "error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[]{
        -0.5f, -0.5f, 0.0f, 0.0f, //0
         0.5f, -0.5f, 1.0f, 0.0f, //1
         0.5f,  0.5f, 1.0f, 1.0f, //2
        -0.5f,  0.5f, 0.0f, 1.0f  //3
    };

    unsigned int indicies[]{
        0, 1, 2,
        2, 3, 0
    };
    {
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));

    VertexArray va;
    VertexBuffer vb(positions, sizeof(float) * 4 * 4);

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indicies, 6);

    Shader shader("resources/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8, 0.3, 0.8, 1.0);

    Texture texture ("resources/textures/Fly-PNG-7.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);


    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    float r = 0.0f;
    float incr = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3, 0.8, 1.0);

        renderer.Clear();
        renderer.Draw(va, ib, shader);

        {
            if (r > 1.0f)
                incr *= -1;
            else if (r < 0.0f)
                incr *= -1;
            r += incr;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    }
    glfwTerminate();
    return 0;
}