#pragma comment (lib, "glew32s.lib")
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Renderer.h"

#include "tests/TestClearColor.h"
#include "tests/Game.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1600, 1600, "Test", NULL, NULL);
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

    glfwSwapInterval(1);


    if (glewInit() != GLEW_OK) {
        std::cerr << "error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));

    Renderer renderer;

    //IMGUI
    /*ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark(); 

    bool show_demo_window = true;
    bool show_another_window = false;*/
    //


    /*test::Test* currentTest = nullptr;
    test::TestMenu* menu = new test::TestMenu(currentTest);
    currentTest = menu;*/

    //menu->AddTest<test::TestClearColor>("Clear Color");
    //menu->AddTest<Game>("Game");
    float angle1 = 0.0f;
    float angle2 = 0.0f;
    int key_a = 0, key_d = 0, key_lshift;
    int key_left = 0, key_right = 0, key_rshift;
    Game game;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

        renderer.Clear();
        glClearColor(0, 0, 0, 1);
        
        game.OnRender();

        {
            if (key_a == GLFW_PRESS)
                angle1 += 0.03f;
            if (key_d == GLFW_PRESS)
                angle1 -= 0.03f;
            if (key_left == GLFW_PRESS)
                angle2 += 0.03f;
            if (key_right == GLFW_PRESS)
                angle2 -= 0.03f;
        }



        game.OnUpdate(angle1, angle2);

        /*test.OnUpdate(0.0f);
        test.OnRender();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();*/

        //if (currentTest)
        //{
        //    currentTest->OnUpdate(0.0f);
        //    currentTest->OnRender();
        //    ImGui::Begin("Test");
        //    if (currentTest != menu && ImGui::Button("<-"))
        //    {
        //        delete currentTest;
        //        currentTest = menu;
        //    }
        //    //currentTest->OnImGuiRender();
        //    ImGui::End();
        //}

        //test.OnImGuiRender();

        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        key_a = glfwGetKey(window, GLFW_KEY_A);
        key_d = glfwGetKey(window, GLFW_KEY_D);
        game.player1_shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
        key_left = glfwGetKey(window, GLFW_KEY_LEFT);
        key_right = glfwGetKey(window, GLFW_KEY_RIGHT);
        game.player2_shift = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);
        //move_x = 0, move_y = 0;
    }

    }

    //IMGUI
    /*ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();*/
    //

    glfwTerminate();
    return 0;
}