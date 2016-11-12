/************************************************/
/*												*/
/*	HM_TransformComponent.h						*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-30						*/
/*												*/
/*	Last Update : 2016-08-30					*/
/*												*/
/*	Author : Eric Laberge, Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_Component.h"



/*				     HEADER						*/

/*		Class HM_TransformComponent
*
*		brief : Component that holds position, rotation and scale of an object
*
**/
class HM_TransformComponent : public HM_Component
{

public:
	
	// Constructor
	HM_TransformComponent(HM_SceneObject* owner) : HM_Component(owner) {}

	// Create an instance of the transform component for the factory
	HM_TransformComponent* create(HM_SceneObject* owner) const override;
	// Initialize the position, rotation and scale of the component
	bool setup(std::map<std::string, void*> descr) override;

	// No display for the transform component
	virtual void display() override {};
	// No update for the transform component
	virtual void update() override {};


		// --- Accessors ---

	glm::vec3 getLocalPosition() const;
	glm::vec3 getLocalRotation() const;
	glm::vec3 getLocalScale() const;

	glm::vec3 getWorldPosition() const;
	glm::vec3 getWorldRotation() const;
	glm::vec3 getWorldScale() const;


	glm::vec3 getPreviousLocalPosition() const;
	glm::vec3 getPreviousLocalRotation() const;
	glm::vec3 getPreviousLocalScale() const;

	glm::vec3 getPreviousWorldPosition() const;
	glm::vec3 getPreviousWorldRotation() const;
	glm::vec3 getPreviousWorldScale() const;

	void translate(glm::vec3 const & offset);
	void rotate(glm::vec3 const & rotation);
	void resize(glm::vec3 const & scale);

	void setLocalPosition(glm::vec3 const & localPosition);
	void setLocalRotation(glm::vec3 const & localRotation);
	void setLocalScale(glm::vec3 const & localScale);

	void setLocalPositionX(float localPositionX);
	void setLocalPositionY(float localPositionY);
	void setLocalPositionZ(float localPositionZ);

	void setLocalRotationX(float localRotationX);
	void setLocalRotationY(float localRotationY);
	void setLocalRotationZ(float localRotationZ);

	void setLocalScaleX(float localScaleX);
	void setLocalScaleY(float localScaleY);
	void setLocalScaleZ(float localScaleZ);

	// Reset the position, rotation and scale to their original values
	void reset();

private:
	


	// Position of transform
	glm::vec3 m_position;
	// Previous position of transform
	glm::vec3 m_previousPosition;
	// Position of transform when the component was setup
	glm::vec3 m_originalPosition;

	// Rotation of transform
	glm::vec3 m_rotation;
	// Previous rotation of transform
	glm::vec3 m_previousRotation;
	// Rotation of transform when the component was setup
	glm::vec3 m_originalRotation;

	// Scale of transform
	glm::vec3 m_scale;
	// Previous scale of transform
	glm::vec3 m_previousScale;
	// Scale of transform when the component was setup
	glm::vec3 m_originalScale;

	// Number of copies
	unsigned int m_linearCopyNumber;

	// Offset of copies
	glm::vec3 m_linearCopyOffset;

};