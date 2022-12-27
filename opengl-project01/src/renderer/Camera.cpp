#include "Camera.h"
#include <iostream>

namespace renderer
{
	//
	// private
	//
	void Camera::updateCameraVectors()
	{
		glm::vec3 forward;
		forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		forward.y = sin(glm::radians(m_pitch));
		forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_forward = glm::normalize(forward);

		m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_forward));
	}

	//
	// public
	//
	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
		: m_forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(c_SPEED), m_mouseSensitivity(c_SENSITIVITY), m_zoom(c_ZOOM)
	{
		m_position = position;
		m_worldUp = up;
		m_yaw = yaw;
		m_pitch = pitch;
		updateCameraVectors();
	}
	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: m_forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(c_SPEED), m_mouseSensitivity(c_SENSITIVITY), m_zoom(c_ZOOM)
	{
		m_position = glm::vec3(posX, posY, posZ);
		m_worldUp = glm::vec3(upX, upY, upZ);
		m_yaw = yaw;
		m_pitch = pitch;
		updateCameraVectors();
	}
	void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
	{
		float velocity = m_movementSpeed * deltaTime;

		switch (direction)
		{
		case FORWARD:	m_position += m_forward * velocity; break;
		case BACKWARD:	m_position -= m_forward * velocity; break;
		case LEFT:		m_position -= m_right * velocity; break;
		case RIGHT:		m_position += m_right * velocity; break;
		default:		std::cerr << "Invalid direction in Camera::ProcessKeyboard" << std::endl; break;
		}
	}
	void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
	{
		xoffset *= m_mouseSensitivity;
		yoffset *= m_mouseSensitivity;

		m_yaw += xoffset;
		m_pitch += yoffset;

		if (constrainPitch)
		{
			if (m_pitch > 89.0f) m_pitch = 89.0f;
			if (m_pitch < -89.0f) m_pitch = -89.0f;
		}

		updateCameraVectors();
	}
	void Camera::ProcessMouseScroll(float yoffset)
	{
		m_zoom -= (float)yoffset;
		if (m_zoom < 1.0f) m_zoom = 1.0f;
		if (m_zoom > 55.0f) m_zoom = 55.0f;
	}
}