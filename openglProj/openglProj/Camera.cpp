﻿#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup) {
	Position = position;
	WorldUp = worldup;
	Forward = glm::normalize(target - position);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Forward, Right));
	Pitch = glm::radians(15.0f);
	Yaw = glm::radians(180.0f);
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup) {
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

Camera::~Camera() {
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Forward, WorldUp);
}

void Camera::UpdateCameraVectors() {
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY) {
	Pitch -= deltaY * SenseX;
	if (Pitch > glm::radians(89.0f)) {
		Pitch = glm::radians(89.0f);
	}
	if (Pitch < glm::radians(- 89.0f)) {
		Pitch = glm::radians(- 89.0f);
	}
	std::cout << "Pitch: " << Pitch << std::endl;
	Yaw -= deltaX * SenseY;
	UpdateCameraVectors();
}

void Camera::UpdateCameraPos() {
	Position += Forward * speedZ * 0.1f + Right * speedX * 0.1f + glm::vec3(0.0f, 1.0f, 0.0f) * speedY * 0.1f;
}