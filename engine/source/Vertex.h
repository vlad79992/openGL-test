#pragma once

class Vertex
{
public:
	Vertex(float x, float y, float z);
	Vertex(float x, float y, float z,
		float r, float g, float b, float a);
	~Vertex();
	glm::vec3 position;
	glm::vec4 color;
};


inline Vertex::Vertex(float x, float y, float z)
{
	position = { x, y, z };
	color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

inline Vertex::Vertex(float x, float y, float z, float r, float g, float b, float a)
{
	position = { x, y, z };
	color = { r, g, b, a };
}

inline Vertex::~Vertex()
{
}