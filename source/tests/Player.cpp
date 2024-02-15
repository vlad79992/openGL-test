#include "Player.h"
#include "../Renderer.h"

#include <cmath>
#include <set>

Player::Player(float col[5])
{
	m_Trail.push_front(new float[](col[0], col[1], col[2], col[3], col[4]));
	m_Color[0] = col[2], m_Color[1] = col[3], m_Color[2] = col[4];
}

Player::~Player()
{
}

void Player::AddSegment(float x, float y)
{
	m_Trail.push_front(new float[](x, y, m_Color[0], m_Color[1], m_Color[2]));
}

void Player::AddSegment(float x, float y, int col[3])
{
	m_Trail.push_front(new float[](x, y, col[0], col[1], col[2]));
}

void Player::RemoveLastSegment()
{
	if (m_Trail.empty())
		return;
	m_Trail.pop_back();
}

void Player::GetFirstPosition(float& x, float& y)
{
	if (m_Trail.empty())
		return;
	x = m_Trail.front()[0];
	y = m_Trail.front()[1];
}

void Player::AddFood(int amount)
{
	m_FoodAmount += amount;
}

void Player::DrawPlayer()
{
	float PI = 3.14159265359;
	glColor3ub(m_Color[0], m_Color[1], m_Color[2]);
	for (auto& segment : m_Trail)
	{
		glBegin(GL_POLYGON);								// Middle circle
		double ori_x = segment[0];                      // the origin or center of circle
		double ori_y = segment[1];
		for (int i = 0; i <= 30; i++) {
			double angle = 2 * PI * i / 30;
			double x = cos(angle) * m_Radius;
			double y = sin(angle) * m_Radius;
			glVertex2d(ori_x + x, ori_y + y);
		}
		glEnd();
	}
}

void Player::SetRadius(float r)
{
	if (r < 0)
		return;
	m_Radius = r;
}

int Player::GetFoodAmount()
{
	return m_FoodAmount;
}

void Player::Kill(std::set<std::pair<float, float>>& food)
{
	int cnt = 0;
	for (auto& segment : m_Trail) {
		if (cnt % 25 != 0) {
			cnt++;
			continue;
		}
		food.insert({ (((double)(rand() % 2000) - 1000) / 25000) + segment[0]
			, (((double)(rand() % 2000) - 1000) / 25000) + segment[1] });
		cnt++;
	}

	m_Trail.clear();
	this->AddSegment(((double)(rand() % 2000) - 1000) / 1000, ((double)(rand() % 2000) - 1000) / 1000);
}

float Player::MinDistToTrail(float x, float y)
{
	float minDist = 1;
	for (auto& segment : m_Trail)
	{
		float tmp = sqrtf((segment[0] - x) * (segment[0] - x) + (segment[1] - y) * (segment[1] - y));
		if (tmp < minDist)
			minDist = tmp;
	}
	return minDist;
}
