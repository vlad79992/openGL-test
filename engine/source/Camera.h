#pragma once

class Camera
{
private:
	friend class Application;
	Camera() {};
	void CameraInit(const GLuint programID, GLuint& MatirxID, float aspect);
	glm::mat4 Projection{};
	glm::mat4 View{};
	glm::mat4 Model{};
	glm::mat4 mvp{};
};

inline void Camera::CameraInit(const GLuint programID, GLuint& matrixId, float aspect)
{
	// Get a handle for our "MVP" uniform
	matrixId = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
	// Camera matrix
	View = glm::lookAt(
		glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
}
