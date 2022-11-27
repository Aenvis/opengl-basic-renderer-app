#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace renderer
{
	enum CameraMovement
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT
	};

		//default camera setup
		const float c_YAW = -90.0f;
		const float c_PITCH = 0.0f;
		const float c_SPEED = 2.5f;
		const float c_SENSITIVITY = 0.1f;
		const float c_ZOOM = 45.0f;

	class Camera
	{
	private:
		glm::vec3 m_position;
		glm::vec3 m_forward;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;
		float m_yaw;
		float m_pitch;
		float m_movementSpeed;
		float m_mouseSensitivity;
		float m_zoom;

	private:
		void updateCameraVectors();

	public:
		//vec3 constructor
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = c_YAW, float pitch = c_PITCH);
		//float values constructor
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		
		inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_position, m_position + m_forward, m_up); }
		inline const float GetZoom() const { return m_zoom; }

		void ProcessKeyboard(CameraMovement direction, float deltaTime);
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
		void ProcessMouseScroll(float yoffset);
	};
}

