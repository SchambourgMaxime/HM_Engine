/************************************************/
/*												*/
/*	HM_Camera.cpp								*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-18						*/
/*												*/
/*	Last Update : 2016-08-18					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_Camera.h"

#include <iostream>
#include <cmath>


/*				 IMPLEMENTATION					*/

	// --- CONSTRUCTOR & DESTRUCTOR ---

/*		Default constructor
*
*		brief : initialize variables to 0 or NULL and vertical axis to Y
*
**/
HM_Camera::HM_Camera() : 
	m_phi(0.0), m_theta(0.0),
	m_orientation(), m_verticalAxis(0, 1, 0),
	m_lateralDisplacement(),
	m_position(), m_targetPoint(),
	m_sensibility(0.0), m_speed(0.0),
	m_isLerping(false), m_lerpPosition(glm::vec3(0.0f)),
	m_lerpSpeed(0.5f)
{
}

/*		Constructor
*
*		brief : initialize variables and set the target point of the camera
*
*		params :
*			- position			(vec3)	: position of the camera
*			- targetPoint		(vec3)	: point targeted by the camera
*			- verticalAxis		(vec3)	: vector representing the vertical 
*										  axis of the coordinate system
*			- sensibility		(float)	: speed at which the camera rotates
*			- speed				(float)	: speed at which the camera moves
*			
**/
HM_Camera::HM_Camera(glm::vec3 position, glm::vec3 targetPoint,
					 glm::vec3 verticalAxis,
					 float sensibility, float speed) :
	m_phi(0.0), m_theta(0.0),
	m_orientation(), m_verticalAxis(verticalAxis),
	m_lateralDisplacement(),
	m_position(position), m_targetPoint(targetPoint),
	m_sensibility(sensibility), m_speed(speed),
	m_speedButtonDown(false),
	m_isLerping(false), m_lerpPosition(glm::vec3(0.0f)),
	m_lerpSpeed(0.5f)
{

	setTargetPoint(targetPoint);

}

/*		Destructor
*
*		brief : 
*
**/
HM_Camera::~HM_Camera()
{
}

// --- ACCESSORS ---

/*		setTargetPoint
*
*		params :
*			- targetPoint	(int)		: point targeted by the camera
*		
**/
void HM_Camera::setTargetPoint(glm::vec3 const & targetPoint)
{

	m_orientation = m_targetPoint - m_position;
	m_orientation = glm::normalize(m_orientation);

	if (m_verticalAxis.x == 1.0)
	{

		m_phi = asin(m_orientation.x);
		m_theta = acos(m_orientation.y / cos(m_phi));

		if (m_orientation.y < 0)
			m_theta *= -1;

	}
	else if (m_verticalAxis.y == 1.0)
	{

		m_phi = glm::asin(m_orientation.y);
		m_theta = glm::acos(m_orientation.z / cos(m_phi));

		if (m_orientation.z < 0)
			m_theta *= -1;

	}
	else if (m_verticalAxis.z == 1.0)
	{

		m_phi = asin(m_orientation.x);
		m_theta = acos(m_orientation.z / cos(m_phi));

		if (m_orientation.z < 0)
			m_theta *= -1;

	}

	m_phi = m_phi * 180 / static_cast<float>(M_PI);
	m_theta = m_theta * 180 / static_cast<float>(M_PI);

}
/*		setPosition
*
*		params :
*			- Position	(int)		: position of the camera
*
**/
void HM_Camera::setPosition(glm::vec3 const & position)
{

	m_position = position;

	m_targetPoint = m_position + m_orientation;

}
/*		setSensibility
*
*		params :
*			- sensibility	(float)		: speed at which the camera rotates
*
**/
void HM_Camera::setSensibility(float sensibility)
{

	m_sensibility = sensibility;

}
/*		setSpeed
*
*		params :
*			- speed	(float)		: speed at which the camera moves
*
**/
void HM_Camera::setSpeed(float speed)
{

	m_speed = speed;

}


	// --- PUBLIC CLASS FUNCTIONS ---

/*		turnTowards
*
*		brief : change the position of the target point along a sphere
*
*		params :
*			- xRel	(int)	: the displacement of mouse X since the last frame
*			- yRel	(int)	: the displacement of mouse Y since the last frame
*
**/
void HM_Camera::turnTowards(int xRel, int yRel)
{

	// Changing the value of the angles
	m_phi += -yRel * m_sensibility;
	m_theta += -xRel * m_sensibility;

	// View on the Y axis can't go further than 89 degrees
	if(m_phi > 89.0)
		m_phi = 89.0;
	else if(m_phi < -89.0)
		m_phi = -89.0;

	// Conversion to radians
	float phiRadian = m_phi * static_cast<float>(M_PI) / 180.0f;
	float thetaRadian = m_theta * static_cast<float>(M_PI) / 180.0f;

	// Recalculating the orientation vector
	if (m_verticalAxis.x == 1.0)
	{
		m_orientation.x = sin(phiRadian);
		m_orientation.y = cos(phiRadian) * cos(thetaRadian);
		m_orientation.z = cos(phiRadian) * sin(thetaRadian);
	}

	else if (m_verticalAxis.y == 1.0)
	{
		m_orientation.x = cos(phiRadian) * sin(thetaRadian);
		m_orientation.y = sin(phiRadian);
		m_orientation.z = cos(phiRadian) * cos(thetaRadian);
	}

	else if (m_verticalAxis.z == 1.0)
	{
		m_orientation.x = cos(phiRadian) * cos(thetaRadian);
		m_orientation.y = cos(phiRadian) * sin(thetaRadian);
		m_orientation.z = sin(phiRadian);
	}

	// Calculating the vector of lateral displacment
	m_lateralDisplacement = glm::cross(m_verticalAxis, m_orientation);
	m_lateralDisplacement = glm::normalize(m_lateralDisplacement);

	// setting new target point
	m_targetPoint = m_position + m_orientation;

}

/*		move
*
*		brief : Move the camera forward or laterally
*
*		params :
*			- input	(HM_Input)	: constant reference to the input controller
*
**/
void HM_Camera::move(HM_Input const & input)
{

	// When the mouse moves, the target point is changed
	if(input.mouseMovement())
		turnTowards(input.getXRel(), input.getYRel());

	// When an arrow key is pressed move the character along the forward
	// or the lateral vector
	if(input.getKey(SDL_SCANCODE_UP))
	{

		m_position = m_position + m_orientation * m_speed;
		m_targetPoint = m_position + m_orientation;

	}

	if (input.getKey(SDL_SCANCODE_DOWN))
	{

		m_position = m_position - m_orientation * m_speed;
		m_targetPoint = m_position + m_orientation;

	}

	if (input.getKey(SDL_SCANCODE_LEFT))
	{

		m_position = m_position + m_lateralDisplacement * m_speed;
		m_targetPoint = m_position + m_orientation;

	}

	if (input.getKey(SDL_SCANCODE_RIGHT))
	{

		m_position = m_position - m_lateralDisplacement * m_speed;
		m_targetPoint = m_position + m_orientation;

	}

	if (input.getMouseButton(0) && !m_speedButtonDown)
	{


		m_sensibility *= 3;
		m_speed *= 3;

		m_speedButtonDown = true;

	}
	else if (!input.getMouseButton(0) && m_speedButtonDown)
	{

		m_sensibility /= 3;
		m_speed /= 3;

		m_speedButtonDown = false;

	}

}

/*		lookAt
*
*		brief : set the position, target point and vertical axis of the camera
*
*		params :
*			- modelview	(mat4)	: reference to the modelview matrix
*
**/
void HM_Camera::lookAt(glm::mat4 &modelview)
{

	// Change modelview to be at the position and look at the target point
	modelview = glm::lookAt(m_position, m_targetPoint, m_verticalAxis);

}

void HM_Camera::lerpTo(glm::vec3 target, float speed)
{

	m_lerpPosition = target;

	m_lerpSpeed = speed;

	m_isLerping = true;

}

void HM_Camera::update()
{

	if (m_isLerping && glm::abs(m_lerpPosition.y - m_position.y) > 0.05f)
		setPosition(glm::vec3(getPosition().x,
			glm::mix(m_position.y, m_lerpPosition.y, m_lerpSpeed),
			getPosition().z));
	else if(m_isLerping)
		m_isLerping = false;

}
