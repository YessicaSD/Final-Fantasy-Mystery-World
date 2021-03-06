#ifndef __m1SCENE_H__
#define __m1SCENE_H__

#include "m1Module.h"
#include "p2ChangeControls.h"

enum class StatesMenu {
	NO_MENU,
	PAUSE_MENU,
	OPTIONS_MENU,
	CONTROLS_MENU,

	NONE
};


struct SDL_Texture;
struct SDL_Rect;

class u1GUI;
class u1Image;
class u1Label;
class u1Button;

class u1CheckBox;
class u1Slider;

class e1Player;



class m1Scene : public m1Module
{
public:

	m1Scene();

	// Destructor
	virtual ~m1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void CreateEntities();

	void CreatePauseMenu();
	void DestroyPauseMenu();
	bool Interact(u1GUI* interaction);

	void CreateOptionsMenu();
	void DestroyOptionsMenu();

	void CreateControlsMenu();
	void DestroyControlsMenu();

public:

	//u1GUI* mock_image_ui = nullptr;
	//SDL_Rect mock_image_rect = { 0, 0, 61, 76 };
	u1GUI* background = nullptr;
	std::list<u1Label*> labels_control;
	std::list<u1Label*> Clabels_control;

	u1Slider* slider_general_volume = nullptr;
	u1Slider* slider_music_volume = nullptr;
	u1Slider* slider_fx_volume = nullptr;

	e1Player* player = nullptr;


private:

	ChangeControls * control_to_change = nullptr;
	StatesMenu menu_state = StatesMenu::NO_MENU;

	//pause
	u1Image* pause_panel = nullptr;
	u1Button* button_resume = nullptr;
	u1Label* label_resume = nullptr;
	u1Button* button_main_menu = nullptr;
	u1Label* label_main_menu = nullptr;
	u1Button* button_abort_quest = nullptr;
	u1Label* label_abort_quest = nullptr;
	u1Button* button_options = nullptr;
	u1Label* label_options = nullptr;
	
	//options
	u1Image* options_panel = nullptr;

	u1Label* label_general_volume = nullptr;
	u1Button* button_general_volume = nullptr;
	
	u1Label* label_music_volume = nullptr;
	u1Button* button_music_volume = nullptr;
	//u1Button* music_slider_btn = nullptr;

	u1Label* label_fx_volume = nullptr;
	u1Button* button_fx_volume = nullptr;
	//u1Button* fx_slider_btn = nullptr;

	u1Label* label_fps = nullptr;
	u1CheckBox* checkbox_fps = nullptr;
	u1Label* label_fullscreen = nullptr;
	u1CheckBox* checkbox_fullscreen = nullptr;
	u1Button* button_controls = nullptr;
	u1Label* label_controls = nullptr;

	u1Button* button_retun = nullptr;
	u1Label* label_return = nullptr;


	// Menu Controls

	

	u1Image* controls_panel = nullptr;
	

	u1Button* button_retun_to_options = nullptr;
	u1Label* label_return_to_options = nullptr;

	// KEYBOARD
	u1Label* keyboard = nullptr;

	u1Button* button_basic_attack = nullptr;
	u1Label* label_basic_attack = nullptr;
	u1Label* label_to_show_how_basic_attack = nullptr;

	u1Button* button_up = nullptr;
	u1Label* label_up = nullptr;
	u1Label* label_to_show_how_up = nullptr;

	u1Button* button_right = nullptr;
	u1Label* label_right= nullptr;
	u1Label* label_to_show_how_right = nullptr;

	u1Button* button_left = nullptr;
	u1Label* label_left = nullptr;
	u1Label* label_to_show_how_left = nullptr;

	u1Button* button_down = nullptr;
	u1Label* label_down = nullptr;
	u1Label* label_to_show_how_down = nullptr;

	u1Button* button_diagonals = nullptr;
	u1Label* label_diagonals = nullptr;
	u1Label* label_to_show_how_diagonals = nullptr;

	u1Button* button_direction_up = nullptr;
	u1Label* label_direction_up = nullptr;
	u1Label* label_to_show_how_direction_up = nullptr;

	u1Button* button_direction_right = nullptr;
	u1Label* label_direction_right = nullptr;
	u1Label* label_to_show_how_direction_right = nullptr;

	u1Button* button_direction_left = nullptr;
	u1Label* label_direction_left = nullptr;
	u1Label* label_to_show_how_direction_left = nullptr;

	u1Button* button_direction_down = nullptr;
	u1Label* label_direction_down = nullptr;
	u1Label* label_to_show_how_direction_down = nullptr;

	// CONTROLLER

	u1Label* controller = nullptr;

	u1Button* Cbutton_direction_up = nullptr;
	u1Label* Clabel_to_show_how_direction_up = nullptr;

	u1Button* Cbutton_direction_right = nullptr;
	u1Label* Clabel_to_show_how_direction_right = nullptr;

	u1Button* Cbutton_direction_left = nullptr;
	u1Label* Clabel_to_show_how_direction_left = nullptr;

	u1Button* Cbutton_direction_down = nullptr;
	u1Label* Clabel_to_show_how_direction_down = nullptr;

	u1Button* Cbutton_diagonals = nullptr;
	u1Label* Clabel_to_show_how_diagonals = nullptr;

	u1Button* Cbutton_basic_attack = nullptr;
	u1Label* Clabel_to_show_how_basic_attack = nullptr;

	u1Button* Cbutton_up = nullptr;
	u1Label* Clabel_to_show_how_up = nullptr;

	u1Button* Cbutton_right = nullptr;
	u1Label* Clabel_to_show_how_right = nullptr;

	u1Button* Cbutton_left = nullptr;
	u1Label* Clabel_to_show_how_left = nullptr;

	u1Button* Cbutton_down = nullptr;
	u1Label* Clabel_to_show_how_down = nullptr;

};

#endif // __j1SCENE_H__