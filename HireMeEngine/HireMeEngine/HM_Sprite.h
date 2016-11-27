/************************************************/
/*												*/
/*	HM_Sprite.h									*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-09-06						*/
/*												*/
/*	Last Update : 2016-09-06					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "HM_Mesh.h"



/*				     HEADER						*/

struct HM_SpriteFrame
{

	glm::vec2 m_position;
	glm::vec2 m_size;
	glm::vec2 m_pivot;

	unsigned int m_vboBytesOffsetCoords;
	unsigned int m_vboBytesOffsetTexCoords;

};

class HM_SpriteTimeline
{

public:
	/** Public functions **/

	// Constructor
	HM_SpriteTimeline();

	// Destructor
	~HM_SpriteTimeline();

	std::string getCurrentFrame();

	void addTimelineStep(unsigned int time, std::string frame);

	void update(float animSpeed);

	void start();
	void stop();

private:
	/** Private variables **/
	
	std::vector<std::pair<unsigned int, std::string>> m_timelineSteps;

	unsigned char m_currentStep;
	std::string m_currentFrame;

	int m_startTime;

	bool isPlaying;

};

/*		Class HM_Sprite
*
*		brief : Sprite which consists of a square mesh and a spriteSheet
*
**/
class HM_Sprite : public HM_Mesh
{

public:
	/** Public functions **/

	// Constructor
	HM_Sprite(std::string const & spriteFile = "",
			  glm::vec2 const & size = glm::vec2(1.0f, 1.0f));

	HM_Sprite(glm::vec2 const & pivotPoint = glm::vec2(0),
		 	  glm::vec2 const & size = glm::vec2(1.0f, 1.0f),
			  glm::vec2 const & uvMin = glm::vec2(0.0f, 0.0f),
			  glm::vec2 const & uvMax = glm::vec2(1.0f, 1.0f));

	// Destructor
	~HM_Sprite();

	void setSize(glm::vec2 const & size);

	void playAnim(std::string animName);

	void updateSprite();

	void crop(float percentage, Direction direction);

	float getAnimSpeed();

	void setAnimSpeed(float animSpeed);

private:
	/** Private functions **/

	void createVertices(glm::vec2 const & size, 
		glm::vec2 const & pivot = glm::vec2(0.0f, 0.0f),
		glm::vec2 const & uvMin = glm::vec2(0.0f, 0.0f),
		glm::vec2 const & uvMax = glm::vec2(1.0f, 1.0f)
		);

	void loadAnimSprite(std::string const & spriteFile);

	void loadFrames(std::list<std::string> const & descr, 
		HM_Texture* const texture);
	void loadTimelines(std::list<std::string> const & descr);

	void addFrameToMesh(HM_SpriteFrame const & frame, 
		glm::vec2 const & textureSize);

	/** Private variables **/
	
	glm::vec2 m_size;
	glm::vec2 m_originalSize;

	glm::vec2 m_pivot;
	glm::vec2 m_originalPivot;

	glm::vec2 m_uvMin;
	glm::vec2 m_originalUVMin;

	glm::vec2 m_uvMax;
	glm::vec2 m_originalUVMax;

	std::map<std::string, HM_SpriteFrame> m_frames;
	std::map<std::string, HM_SpriteTimeline> m_timelines;

	HM_SpriteTimeline m_currentTimeline;
	std::string m_currentTimelineName;
	HM_SpriteFrame m_currentFrame;
	std::string m_currentFrameName;

	float m_animSpeed;

};