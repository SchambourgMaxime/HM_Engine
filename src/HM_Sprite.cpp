/************************************************/
/*												*/
/*	HM_Sprite.cpp								*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-09-07						*/
/*												*/
/*	Last Update : 2016-09-07					*/
/*												*/
/*	Author : Eric Laberge, Maxime Schambourg	*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_Sprite.h"



/*				 IMPLEMENTATION					*/

HM_Sprite::HM_Sprite(std::string const & spriteFile,
					 glm::vec2 const & size) :
	m_animSpeed(1.0f)
{

	if(spriteFile == "")
	{

		createVertices(size);

		m_originalSize = m_size;
		m_originalPivot = m_pivot;
		m_originalUVMin = m_uvMin;
		m_originalUVMax = m_uvMax;

	}
	else
	{

		loadAnimSprite(spriteFile);

		m_currentTimeline = m_timelines.begin()->second;

	}

	m_triangles.push_back(new HM_Triangle(m_vertices[0], m_vertices[1],
		m_vertices[2]));
	m_triangles.push_back(new HM_Triangle(m_vertices[2], m_vertices[3],
		m_vertices[0]));

	m_nbCoords = m_vertices.size() * 3;
	m_nbIndices = m_triangles.size() * 3;
	m_nbTexCoords = m_vertices.size() * 2;

	m_byteSizeCoords = m_nbCoords * sizeof(float);
	m_byteSizeIndices = m_nbIndices * sizeof(unsigned int);
	m_byteSizetexCoords = m_nbTexCoords * sizeof(float);

}

HM_Sprite::HM_Sprite(glm::vec2 const & size, glm::vec2 const & pivot,
					 glm::vec2 const & uvMin, glm::vec2 const & uvMax) :
	m_size(size), m_originalSize(size),
	m_pivot(pivot), m_originalPivot(pivot),
	m_uvMin(uvMin), m_originalUVMin(uvMin),
	m_uvMax(uvMin), m_originalUVMax(uvMin),
	m_animSpeed(1.0f)
{

	createVertices(size, pivot, uvMin, uvMax);

	m_originalSize = m_size;
	m_originalPivot = m_pivot;
	m_originalUVMin = m_uvMin;
	m_originalUVMax = m_uvMax;

	m_triangles.push_back(new HM_Triangle(m_vertices[0], m_vertices[1],
		m_vertices[2]));
	m_triangles.push_back(new HM_Triangle(m_vertices[2], m_vertices[3],
		m_vertices[0]));

	m_nbCoords = m_vertices.size() * 3;
	m_nbIndices = m_triangles.size() * 3;
	m_nbTexCoords = m_vertices.size() * 2;

	m_byteSizeCoords = m_nbCoords * sizeof(float);
	m_byteSizeIndices = m_nbIndices * sizeof(unsigned int);
	m_byteSizetexCoords = m_nbTexCoords * sizeof(float);

}

HM_Sprite::~HM_Sprite()
{

	HM_Mesh::~HM_Mesh();

}

void HM_Sprite::setSize(glm::vec2 const & size)
{

	createVertices(size);

}

void HM_Sprite::createVertices(glm::vec2 const & size, glm::vec2 const & pivot,
	glm::vec2 const & uvMin, glm::vec2 const & uvMax)
{

	glm::vec2 min(-size.x * pivot.x, -size.y * pivot.y);
	glm::vec2 max(size.x * (1.0f - pivot.x), size.y * (1.0f - pivot.y));

	// down left
	m_vertices.push_back(new HM_Vertex(0,
		glm::vec3(min.x, min.y, 0),
		glm::vec2(uvMin.x, uvMin.y)));

	// up left
	m_vertices.push_back(new HM_Vertex(1,
		glm::vec3(min.x, max.y, 0),
		glm::vec2(uvMin.x, uvMax.y)));

	// up right
	m_vertices.push_back(new HM_Vertex(2,
		glm::vec3(max.x, max.y, 0),
		glm::vec2(uvMax.x, uvMax.y)));

	// down right
	m_vertices.push_back(new HM_Vertex(3,
		glm::vec3(max.x, min.y, 0),
		glm::vec2(uvMax.x, uvMin.y)));

	m_boundingBox = HM_Cube(glm::vec3(min, 0.0f), glm::vec3(max, 0.0f));

	m_size = size;
	m_pivot = pivot;
	m_uvMin = uvMin;
	m_uvMax = uvMax;

}

void HM_Sprite::loadAnimSprite(std::string const & spriteFile)
{

	HM_FileHandler file("Animation/" + spriteFile, HM_FILE_READ);

	if (!file)
	{

		GRAPHICS_LOG_LINE("Error : can't open sprite file \'" +
			spriteFile + "\'.", __LINE__ - 6);
		return;

	}

	std::string textureString = file.getLine(0);
	HM_Texture* spriteSheet = NULL;
	if(textureString.substr(0U, 7U) == "texture")
		spriteSheet = HM_GameMaster::instance()->getGraphicsManager()->
						loadTexture(textureString.substr(8U));

	std::list<std::string> framesStringList;
		framesStringList = file.getSectionWithTitle("Frames");

	loadFrames(framesStringList, spriteSheet);

	framesStringList = file.getSectionWithTitle("Timelines");

	loadTimelines(framesStringList);

}

void HM_Sprite::playAnim(std::string animName)
{

	if(animName != m_currentTimelineName)
	{

		std::map<std::string, HM_SpriteTimeline>::const_iterator iter;
		iter = m_timelines.find(animName);

		if (iter != m_timelines.end())
		{

			m_currentTimeline.stop();
			m_currentTimelineName = (*iter).first;
			m_currentTimeline = (*iter).second;

		}

	}

}

void HM_Sprite::updateSprite()
{

	if(m_timelines.size() != 0)
	{
	
		m_currentTimeline.update(m_animSpeed);
	
		if (m_currentFrameName != m_currentTimeline.getCurrentFrame())
		{

			std::string currentFrameName = m_currentTimeline.getCurrentFrame();

			std::map<std::string, HM_SpriteFrame>::const_iterator iter;
			iter = m_frames.find(currentFrameName);

			if (iter != m_frames.end())
			{

				m_currentFrame = m_frames[currentFrameName];

				setVBOBytesOffsetCoords(
					m_originalVBOBytesOffsetCoords +
					m_currentFrame.m_vboBytesOffsetCoords);
				setVBOBytesOffsetTexCoords(
					m_originalVBOBytesOffsetTexCoords +
					m_currentFrame.m_vboBytesOffsetTexCoords);

				m_currentFrameName = currentFrameName;

			}

		}

	}

}

void HM_Sprite::loadFrames(std::list<std::string> const & descr,
						   HM_Texture* const texture)
{

	std::list<std::string>::const_iterator iter;
	int byteSizeCursor = 0;
	glm::vec2 textureSize;

	if(texture)
		textureSize = texture->getSize();
	else
		textureSize = glm::vec2(1.0f, 1.0f);

	for (iter = descr.begin(); iter != descr.end(); iter++)
	{

		std::string current = *iter;

		if (current.size() != 0)
		{

			if (current.substr(1U, 6U) == "sprite")
			{

				std::string frameName = current.substr(8U,
											current.find(" {", 8U) - 8U);

				HM_SpriteFrame frame;

				frame.m_vboBytesOffsetCoords = byteSizeCursor * 12 * sizeof(float);
				frame.m_vboBytesOffsetTexCoords = byteSizeCursor * 8 * sizeof(float);

				while (current.find('}') == -1)
				{

					int keyEnd = current.find_first_of('=');

					if (keyEnd != -1)
					{

						if (current.substr(0U, keyEnd) == "position")
							frame.m_position = hmu::getDataFromVoid<glm::vec2>(
								hmu::getDataFromString(
									current.substr(keyEnd + 1)), true);
						else if (current.substr(0U, keyEnd) == "size")
							frame.m_size = hmu::getDataFromVoid<glm::vec2>(
								hmu::getDataFromString(
									current.substr(keyEnd + 1)), true);
						else if (current.substr(0U, keyEnd) == "pivot")
							frame.m_pivot = hmu::getDataFromVoid<glm::vec2>(
								hmu::getDataFromString(
									current.substr(keyEnd + 1)), true);

					}

					iter++;
					current = (*iter);

				}

				byteSizeCursor++;
				addFrameToMesh(frame, textureSize);
				m_frames[frameName] = frame;

			}

		}

	}

}

void HM_Sprite::loadTimelines(std::list<std::string> const & descr)
{

	std::list<std::string>::const_iterator iter;

	for (iter = descr.begin(); iter != descr.end(); iter++)
	{

		std::string current = *iter;

		if (current.size() != 0)
		{

			if (current.substr(1U, 2U) == "tl")
			{

				std::string timelineName = current.substr(4U,
					current.find(" {", 4U) - 4U);

				HM_SpriteTimeline timeline;

				while (current.find('}') == -1)
				{

					int keyEnd = current.find_first_of('=');

					if (keyEnd != -1)
					{

						unsigned int time = stoi(current.substr(0U, keyEnd));
						std::string frame = current.substr(keyEnd + 1);

						timeline.addTimelineStep(time, frame);

					}

					iter++;
					current = (*iter);

				}

				m_timelines[timelineName] = timeline;

			}

		}

	}

}

void HM_Sprite::addFrameToMesh(HM_SpriteFrame const & frame,
							   glm::vec2 const & textureSize)
{

	glm::vec2 uvMax, uvMin;

	uvMax.x = (frame.m_position.x + frame.m_size.x) / textureSize.x;
	uvMax.y = (frame.m_position.y + frame.m_size.y) / textureSize.y;
	uvMin.x = (frame.m_position.x) / textureSize.x;
	uvMin.y = (frame.m_position.y) / textureSize.y;

	createVertices(frame.m_size, frame.m_pivot, uvMin, uvMax);

}


void HM_Sprite::crop(float percentage, Direction direction)
{

	m_vertices.clear();

	if(direction == RIGHT)
		createVertices(glm::vec2(m_originalSize.x * percentage, m_size.y),
		m_originalPivot,
		glm::vec2(m_originalUVMin.x * percentage, m_uvMin.y),
		glm::vec2(m_originalUVMax.x * percentage, m_uvMax.y));
	else if(direction == LEFT)
		createVertices(glm::vec2(m_originalSize.x * percentage, m_size.y),
		m_originalPivot,
		glm::vec2(m_originalUVMin.x * percentage, m_uvMin.y),
		glm::vec2(m_originalUVMax.x * percentage, m_uvMax.y));


		HM_GameMaster::instance()->getGraphicsManager()->refreshVBOData(this,
			HM_VBO_STATIC, HM_VBO_STATIC);

}

HM_SpriteTimeline::HM_SpriteTimeline() :
	m_currentStep(0), m_currentFrame(""), m_startTime(-1), isPlaying(false)
{
}

HM_SpriteTimeline::~HM_SpriteTimeline()
{

}

std::string HM_SpriteTimeline::getCurrentFrame()
{

	return m_currentFrame;

}

void HM_SpriteTimeline::addTimelineStep(unsigned int time, std::string frame)
{

	std::pair<unsigned int, std::string> pairToAdd(time, frame);

	m_timelineSteps.push_back(pairToAdd);

}

void HM_SpriteTimeline::update(float animSpeed)
{

	if(!isPlaying)
		start();

	if (m_timelineSteps.size() > 1u)
	{

		unsigned int timeFromStart = SDL_GetTicks() - m_startTime;


		if (timeFromStart > m_timelineSteps[m_currentStep + 1].first * animSpeed)
		{

			m_currentStep++;
			m_currentFrame = m_timelineSteps[m_currentStep].second;

		}

		if (m_currentStep == m_timelineSteps.size() - 1)
			start();

	}

}

void HM_SpriteTimeline::start()
{

	m_startTime = SDL_GetTicks();

	m_currentStep = 0;
	m_currentFrame = m_timelineSteps[m_currentStep].second;

	isPlaying = true;

}

void HM_SpriteTimeline::stop()
{

	m_startTime = -1;
	isPlaying = false;

}

float HM_Sprite::getAnimSpeed()
{

	return m_animSpeed;

}

void HM_Sprite::setAnimSpeed(float animSpeed)
{

	m_animSpeed = animSpeed;

}
