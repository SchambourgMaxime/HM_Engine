/************************************************/
/*												*/
/*	HM_Camera.h									*/
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

#pragma once

// Includes GLM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "HM_Input.h"



/*				     HEADER						*/

/*		Class HM_Camera
*
*		brief : Handle the camera
*
**/
class HM_Camera
{
public :
	/** Public functions **/

	// Default constuctor
	HM_Camera();
	// Constructor
	HM_Camera(glm::vec3 position, glm::vec3 targetPoint,
	glm::vec3 verticalAxis, float sensibility, float speed);
	// Destructor
	~HM_Camera();


		// --- Accessors ---
	
	inline glm::vec3 const & getPosition() const;
	inline glm::vec3 const & getOrientation() const;

	inline float getSensibility() const;
	inline float getSpeed() const;

	void setSensibility(float sensibility);
	void setSpeed(float speed);

	void setTargetPoint(glm::vec3 const & targetPoint);
	void setPosition(glm::vec3 const & position);


	// Change the position of the target point along a sphere
	void turnTowards(int xRel, int yRel);
	// Move the camera forward or laterally
	void move(HM_Input const &input);
	// Set the position, target point and vertical axis of the camera
	void lookAt(glm::mat4 &modelview);

	//
	void lerpTo(glm::vec3 target, float speed = 0.5f);

	// Update
	void update();


private :
	/** Private Variables **/

	// Angles position to target point along trigonometric sphere
	float m_phi;
	float m_theta;
	// Vector that starts at position and go toward the target point
	glm::vec3 m_orientation;

	// Vector representing the vertical axis of the coordinate system
	glm::vec3 m_verticalAxis;
	// Vector representing the side of the camera
	glm::vec3 m_lateralDisplacement;

	// Position of the camera in modelview
	glm::vec3 m_position;
	// Point looked at by the camera
	glm::vec3 m_targetPoint;

	// is the camera lerping
	bool m_isLerping;
	// Position of the camera to lerp to
	glm::vec3 m_lerpPosition;
	// Speed of lerp
	float m_lerpSpeed;


	// Speed at which the camera rotates
	float m_sensibility;
	// Speed at which the camera moves
	float m_speed;

	bool m_speedButtonDown;

};

inline glm::vec3 const & HM_Camera::getPosition() const
{

	return m_position;

}

inline glm::vec3 const & HM_Camera::getOrientation() const
{

	return m_orientation;

}

/*		getSensibility
*
*		return : float -> speed at which the camera rotates
*
**/
inline float HM_Camera::getSensibility() const
{

	return m_sensibility;

}
/*		getSpeed
*
*		return : float -> speed at which the camera moves
*
**/
inline float HM_Camera::getSpeed() const
{

	return m_speed;

}
