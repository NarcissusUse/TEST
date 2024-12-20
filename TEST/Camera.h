#pragma once
#include "Renderer.h"
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT

};
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		:Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}
	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;
		if (direction==FORWARD)
		{
			Position += Front * (velocity*3);
		}
		if (direction==BACKWARD)
		{
			Position -= Front * (velocity*3);
		}
		if (direction==LEFT)
		{
			Position -= Right * (velocity*3);
		}
		if (direction==RIGHT)
		{
			Position += Right * (velocity*3);
		}
	}
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constarinPitch = true) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		Yaw += xoffset;
		Pitch += yoffset;
		if (constarinPitch)
		{
			if (Pitch > 89.0f) {
				Pitch = 89.0f;
			}
			if (PITCH<-89.0f)
			{
				Pitch = -89.0f;
			}
		}
		updateCameraVectors();//update front,up,right
	}
	void ProcessMouseScroll(float yoffset) {
		Zoom -= (float)yoffset;
		if (Zoom<1.0f)
		{
			Zoom = 1.0f;
		}
		if (Zoom>45.0f)
		{
			Zoom = 45.0f;
		}

	}
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
private:
	void updateCameraVectors() {
		glm::vec3 dircetion;
		dircetion.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		dircetion.y = sin(glm::radians(Pitch));
		dircetion.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(dircetion);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
		
	}
	

};
