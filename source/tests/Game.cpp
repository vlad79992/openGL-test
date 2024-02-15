#include "Game.h"

#include "../Renderer.h"

#include <cmath>

#include <iostream>

Game::Game()
{
    player1 = new Player( new float[](0.0f, -0.5f, 200, 191, 231));
    if (is_2_players)
        player2 = new Player(new float[](0.0f, 0.5f, 255, 201, 14));
    else
        player2 = nullptr;
    for (int i = 0; i < 10; ++i)
        food.insert({ ((double) (rand() % 2000) - 1000) / 1000, ((double)(rand() % 2000) - 1000) / 1000 });
}

Game::~Game()
{
}

void Game::OnUpdate(float angle1, float angle2)
{
    {
        float x1 = 0, y1 = 0;
        player1->GetFirstPosition(x1, y1);
        x1 += speed * (1 + player1_shift) * cos(angle1);
        y1 += speed * (1 + player1_shift) * sin(angle1);
        //std::cout << x << '\t' << y << std::endl;
        {
            if (y1 > 1)
                y1 -= 2;
            if (x1 > 1)
                x1 -= 2;
            if (y1 < -1)
                y1 += 2;
            if (x1 < -1)
                x1 += 2;
        }
        std::set<std::pair<float, float>> to_delete;
        for (auto& f : food)
        {
            if ((f.first - x1) * (f.first - x1) + (f.second - y1) * (f.second - y1) < 0.001f)
            {
                to_delete.insert(f);
                food.insert({ ((double)(rand() % 2000) - 1000) / 1000, ((double)(rand() % 2000) - 1000) / 1000 });
                player1->AddFood(10);
            }
        }
        for (auto& x : to_delete)
            food.erase(x);
        to_delete.clear();

        if (player1->GetFoodAmount() != 0)
            player1->AddFood(-1);
        else
            //trail.pop_back();
            player1->RemoveLastSegment();
        if (player1_shift)
            player1->RemoveLastSegment();
        //trail.push_front({ x, y });
        player1->AddSegment(x1, y1);
        if (player2->MinDistToTrail(x1, y1) < 0.05f)
            player1->Kill(food);
    }
    {
        float x1 = 0, y1 = 0;
        player2->GetFirstPosition(x1, y1);
        x1 += speed * (1 + player2_shift) * cos(angle2);
        y1 += speed * (1 + player2_shift) * sin(angle2);
        //std::cout << x << '\t' << y << std::endl;
        {
            if (y1 > 1)
                y1 -= 2;
            if (x1 > 1)
                x1 -= 2;
            if (y1 < -1)
                y1 += 2;
            if (x1 < -1)
                x1 += 2;
        }
        std::set<std::pair<float, float>> to_delete;
        for (auto& f : food)
        {
            if ((f.first - x1) * (f.first - x1) + (f.second - y1) * (f.second - y1) < 0.001f)
            {
                to_delete.insert(f);
                food.insert({ ((double)(rand() % 2000) - 1000) / 1000, ((double)(rand() % 2000) - 1000) / 1000 });
                player2->AddFood(10);
            }
        }
        for (auto& x : to_delete)
            food.erase(x);
        to_delete.clear();

        if (player2->GetFoodAmount() != 0)
             player2->AddFood(-1);
        else
            //trail.pop_back();
            player2->RemoveLastSegment();
        if (player2_shift)
            player2->RemoveLastSegment();
        //trail.push_front({ x, y });
        player2->AddSegment(x1, y1);
        if (player1->MinDistToTrail(x1, y1) < 0.005f)
            player2->Kill(food);
    }
}

void DrawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
        float x = r * cosf(theta);//calculate the x component 
        float y = r * sinf(theta);//calculate the y component 
        glVertex2f(x + cx, y + cy);//output vertex 
    }
    glEnd();
}

void drawFilledSun(float radius, float x, float y, int col[3]) {

    glLoadIdentity();
    int i;
    glColor3ub(col[0], col[1], col[2]);
    double twicePi = 2.0 * 3.142;
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= 20; i++) {
        glVertex2f(
            (x + (radius * cos(i * twicePi / 20))), (y + (radius * sin(i * twicePi / 20)))
        );
    }
    glEnd(); //END
}

void Game::OnRender()
{
    player1->DrawPlayer();
    if (is_2_players)
        player2->DrawPlayer();

    int col[3]{};
    col[0] = 255, col[1] = 128, col[2] = 25;
    for (auto& x : food)
    {
        drawFilledSun(0.025, x.first, x.second, col);
    }
    //DrawCircle(x, y, 1, 24);
}
