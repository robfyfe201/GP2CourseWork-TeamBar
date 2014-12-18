#ifndef FPSCameraController_h
#define FPSCameraController_h

#include "Component.h"

class Camera;

class FPSCameraController :public Component
{
	float camPitch();
	float camYaw();
public:
	FPSCameraController();
	~FPSCameraController();

	void update();
	void moveCamera();
	void lockCamera();

	void setCamera(Camera * cam);
	void setSpeed(float speed);
private:
	float m_ForwardSpeed;
	float m_StrafeSpeed;
	float m_LookSpeed;
	Camera * m_AttachedCamera;

};


#endif