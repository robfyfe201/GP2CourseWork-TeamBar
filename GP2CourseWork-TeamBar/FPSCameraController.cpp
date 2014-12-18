#include <iostream>

#include "FPSCameraController.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Timer.h"
#include "Input.h"

#include <glm/glm.hpp>
using glm::vec3;

FPSCameraController::FPSCameraController()
{
	m_AttachedCamera = NULL;
	m_ForwardSpeed = 400;
	m_StrafeSpeed = 400;
	m_LookSpeed = 3;
	m_Type = "FPSCameraController";
	m_Name = m_Type;
}

FPSCameraController::~FPSCameraController()
{

}

void FPSCameraController::update()
{
	if (m_AttachedCamera)
	{
		//lets just use standard FPS Controllers(mouse & keyboard)
		vec3 currentPos = m_AttachedCamera->getParent()->getTransform()->getPosition();
		vec3 currentRot = m_AttachedCamera->getParent()->getTransform()->getRotation();
		vec3 currentLook = m_AttachedCamera->getLookAt();
		//get a forward vector
		vec3 forward = currentLook - currentPos;
		forward.y = 0.0f;
		forward = glm::normalize(forward);

		//calculate a right vector
		vec3 right = glm::cross(m_AttachedCamera->getUp(), forward);
		right = glm::normalize(right);

		//Grab input
		if (Input::getInput().getKeyboard()->isKeyDown(SDLK_w))
		{
			currentPos.z += forward.z*m_ForwardSpeed*Timer::getTimer().getDeltaTime();
			currentLook.z += forward.z*m_ForwardSpeed*Timer::getTimer().getDeltaTime();
		}
		else if (Input::getInput().getKeyboard()->isKeyDown(SDLK_s))
		{
			currentPos.z -= forward.z*m_ForwardSpeed*Timer::getTimer().getDeltaTime();
			currentLook.z -= forward.z*m_ForwardSpeed*Timer::getTimer().getDeltaTime();
		}

		if (Input::getInput().getKeyboard()->isKeyDown(SDLK_a))
		{
			currentPos += right*m_StrafeSpeed*Timer::getTimer().getDeltaTime();
			currentLook += right*m_StrafeSpeed*Timer::getTimer().getDeltaTime();
		}
		else if (Input::getInput().getKeyboard()->isKeyDown(SDLK_d))
		{
			currentPos += right*(m_StrafeSpeed*-1)*Timer::getTimer().getDeltaTime();
			currentLook += right*(m_StrafeSpeed*-1)*Timer::getTimer().getDeltaTime();
		}

		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
				case SDL_MOUSEMOTION:
			{
				//get the mouse values and directly move the yaw & pitch of the camera(the x and y of the camera rotation)
				//use these to caculate the look at
				int mouseX = Input::getInput().getMouse()->getRelativeMouseX();
				int mouseY = Input::getInput().getMouse()->getRelativeMouseY();

				currentRot.y += mouseX*Timer::getTimer().getDeltaTime()*m_LookSpeed;
				currentRot.x += mouseY*Timer::getTimer().getDeltaTime()*m_LookSpeed;

				//adjust values
				m_AttachedCamera->getParent()->getTransform()->setRotation(currentRot.x, currentRot.y, currentRot.z);
				m_AttachedCamera->getParent()->getTransform()->setPosition(currentPos.x, currentPos.y, currentPos.z);
				m_AttachedCamera->setLook(currentLook.x + (cos(currentRot.x)*cos(currentRot.y)),
					currentLook.y,
					currentLook.z + (sin(currentRot.x)*cos(currentRot.y)));
			}
		}
	}
}

void FPSCameraController::setCamera(Camera * cam)
{
	m_AttachedCamera = cam;
}