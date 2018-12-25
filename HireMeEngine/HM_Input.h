#ifndef DEF_INPUT
#define DEF_INPUT

#include <SDL2/SDL.h>

class HM_Input
{
	public :

		HM_Input();
		~HM_Input();

		void updateEvents();

		bool ended() const;

		bool getKey(const SDL_Scancode key) const;
		bool getMouseButton(const Uint8 button) const;
		bool mouseMovement() const;

		int getX() const;
		int getY() const;

		int getXRel() const;
		int getYRel() const;

		void showCursor(bool toggle) const;

		void captureCursor(bool toggle) const;

	private :
		SDL_Event m_events;
		bool m_keys[SDL_NUM_SCANCODES];
		bool m_mouseButtons[8];
		int m_x, m_y;
		int m_xRel, m_yRel;
		bool m_ended;
};

#endif

