#ifndef _GUI_LABEL_H__ 
#define _GUI_LABEL_H__

#include "GUI.h"
#include "j1Fonts.h"
#include <string>

class GUI_Label : public GUI
{
private:
	SDL_Rect img_normal;

public:
	GUI_Label() : GUI(LABEL, 0, 0, nullptr, { 0,0,0,0 }) {	}
	GUI_Label(const int & pos_x, const int & pos_y, const char* txt, const Color &c, const char* path_font, const uint &size = 12, GUI* parent = nullptr, bool interactable = false, bool draggable = false, uint32 wrap_length = 0);
	~GUI_Label();

	void InnerDraw();

	void SetText(const char* txt);

	void SetColor(Color c);

	void SetColor(const SDL_Color &c);

	std::string GetText();

	void ChangeFont(const char* f, const int &size);

protected:

	FontType		id_font;
	uint			size = 0;
	SDL_Texture*	texture = nullptr;
	SDL_Color		color = { 255,255,255,255 };
	std::string		text;
};
#endif //_GUI_LABEL_H_