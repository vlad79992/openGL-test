#pragma once
#include <deque>
#include <set>

#include "Player.h"

class Game
{
public:
	Game();
	~Game();
	void OnUpdate(float angle1, float angle2 = 0);
	void OnRender();
	bool is_2_players = true;
	int player1_shift = 0, player2_shift = 0;
private:
	Player *player1, *player2;

	std::set<std::pair<float, float>> food;
	float cur_food = 0;

	float speed = 0.0025f;
};

