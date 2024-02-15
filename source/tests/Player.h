#pragma once

#include <deque>
#include <set>

class Player
{
public:
	Player(float col[5]);
	~Player();
	void AddSegment(float x, float y);
	void AddSegment(float x, float y, int col[3]);
	void RemoveLastSegment();
	void GetFirstPosition(float& x, float& y);
	void AddFood(int amount);
	void DrawPlayer();
	void SetRadius(float r);
	int GetFoodAmount();
	void Kill(std::set<std::pair<float, float>>& food);
	float MinDistToTrail(float x, float y);
private:
	int m_FoodAmount;
	int m_Color[3];
	float m_Radius = 0.03;
	/* 
	[0] & [1] pos
	[2], [3], [4] color
	*/
	std::deque<float*> m_Trail; 
};

