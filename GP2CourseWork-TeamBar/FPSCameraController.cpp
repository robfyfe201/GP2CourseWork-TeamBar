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
	m_ForwardSpeed = 350;
	m_StrafeSpeed = 350;
	m_LookSpeed = 2.0f;
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
        vec3 currentPos=m_AttachedCamera->getParent()->getTransform()->getPosition();
        vec3 currentRot=m_AttachedCamera->getParent()->getTransform()->getRotation();
        vec3 currentLook=m_AttachedCamera->getLookAt();

		vec3 direction(cos(currentRot.x) * sin(currentRot.y),
			sin(currentRot.x),
			cos(currentRot.x) * cos(currentRot.y));

		vec3 right = vec3(sin(currentRot.y - 3.14f / 2.0f), 0, cos(currentRot.y - 3.14 / 2.0f));

		glm::vec3 up = glm::cross(right, direction);
        
        //get the mouse values and directly move the yaw & pitch of the camera(the x and y of the camera rotation)
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			 //use these to caculate the look at
			int mouseX=Input::getInput().getMouse()->getRelativeMouseX();
			int mouseY=Input::getInput().getMouse()->getRelativeMouseY();       

			switch (event.type)
			case SDL_MOUSEMOTION:
			{
					currentRot.x += mouseY*Timer::getTimer().getDeltaTime()*m_LookSpeed;
					currentRot.y += mouseX*Timer::getTimer().getDeltaTime()*m_LookSpeed;				
			}
		}

		//Grab input
		if (Input::getInput().getKeyboard()->isKeyDown(SDLK_w))
			{
				currentPos.z += direction.z*m_ForwardSpeed*Timer::getTimer().getDeltaTime();
				currentPos.y += direction.y*m_ForwardSpeed*Timer::getTimer().getDeltaTime();
				currentPos.x += direction.x*m_ForwardSpeed*Timer::getTimer().getDeltaTime();
			}
		else if (Input::getInput().getKeyboard()->isKeyDown(SDLK_s))
			{
				currentPos.z -= direction.z*m_ForwardSpeed*Timer::getTimer().getDeltaTime();
				currentPos.y -= direction.y*m_ForwardSpeed*Timer::getTimer().getDeltaTime();
				currentPos.x -= direction.x*m_ForwardSpeed*Timer::getTimer().getDeltaTime();
			}

		if (Input::getInput().getKeyboard()->isKeyDown(SDLK_a))
			{
				currentPos -= right*m_StrafeSpeed*Timer::getTimer().getDeltaTime();
			}
		else if (Input::getInput().getKeyboard()->isKeyDown(SDLK_d))
			{
				currentPos += right*m_StrafeSpeed*Timer::getTimer().getDeltaTime();
			}

		m_AttachedCamera->getParent()->getTransform()->setPosition(currentPos.x, currentPos.y, currentPos.z);
		m_AttachedCamera->getParent()->getTransform()->setRotation(currentRot.x, currentRot.y, currentRot.z);
		m_AttachedCamera->setUp(up.x, up.y, up.z);
		m_AttachedCamera->setLook(currentPos.x + direction.x, currentPos.y + direction.y, currentPos.z + direction.z);
		}       
	}

void FPSCameraController::setCamera(Camera * cam)
{
    m_AttachedCamera=cam;
}