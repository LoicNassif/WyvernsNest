#pragma once

#include "../engine/core.hpp"

class SettingsMenu : public State {

public:
	SettingsMenu();
	~SettingsMenu();

	void handleEvent(const SDL_Event& e);   // Handles incoming events
	void update(int delta);                 // Handles state logic
	void render();                          // Handles entity rendering

private:
	Sprite background;
	int counter = 0;
	bool render_text = true;
};

