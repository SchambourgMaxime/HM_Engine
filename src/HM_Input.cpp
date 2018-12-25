#include "HM_Input.h"



HM_Input::HM_Input() : m_x(0), m_y(0), m_xRel(0), m_yRel(0), m_ended(false)
{
	for(int i = 0; i < SDL_NUM_SCANCODES; i++)
		m_keys[i] = false;

	for(int i = 0; i < 8; i++)
		m_mouseButtons[i] = false;
}


HM_Input::~HM_Input()
{
}

void HM_Input::updateEvents()
{
	// To avoid false movements from the mouse, we reset the the relative coordinates
	m_xRel = 0;
	m_yRel = 0;

	// Event loop
	while (SDL_PollEvent(&m_events))
	{
		switch (m_events.type)
		{
			// When a key is pressed
			case SDL_KEYDOWN :

				m_keys[m_events.key.keysym.scancode] = true;

			break;

			// When a key is released
			case SDL_KEYUP :

				m_keys[m_events.key.keysym.scancode] = false;

			break;


			// When a mouse button is pressed
			case SDL_MOUSEBUTTONDOWN:

				m_mouseButtons[m_events.button.button] = true;

			break;

			// When a mouse button is released
			case SDL_MOUSEBUTTONUP :

				m_mouseButtons[m_events.button.button] = false;

			break;

			// When the mouse is moved
			case SDL_MOUSEMOTION :

				m_x = m_events.motion.x;
				m_y = m_events.motion.y;

				m_xRel = m_events.motion.xrel;
				m_yRel = m_events.motion.yrel;

			break;

			case SDL_WINDOWEVENT :
				
				if(m_events.window.event == SDL_WINDOWEVENT_CLOSE)
					m_ended = true;

			break;

			default:
				break;
		}
	}
}

bool HM_Input::ended() const
{
	return m_ended;
}

bool HM_Input::getKey(const SDL_Scancode key) const
{
	return m_keys[key];
}

bool HM_Input::getMouseButton(const Uint8 button) const
{
	return m_mouseButtons[button];
}

bool HM_Input::mouseMovement() const
{
	if(m_xRel == 0 && m_yRel == 0)
		return false;
	else
		return true;
}


// Accessors

int HM_Input::getX() const
{
	return m_x;
}

int HM_Input::getY() const
{
	return m_y;
}

int HM_Input::getXRel() const
{
	return m_xRel;
}

int HM_Input::getYRel() const
{
	return m_yRel;
}

void HM_Input::showCursor(bool toggle) const
{
	if(toggle)
		SDL_ShowCursor(SDL_ENABLE);
	else
		SDL_ShowCursor(SDL_DISABLE);
}

void HM_Input::captureCursor(bool toggle) const
{
	if(toggle)
		SDL_SetRelativeMouseMode(SDL_TRUE);
	else
		SDL_SetRelativeMouseMode(SDL_FALSE);
}
