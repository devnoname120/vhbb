#include "input.h"


typedef enum x{
	MAIN_VIEW,
	QUEUE_VIEW
} GUIViews;

int main()
{
	vita2d_init();
	vita2d_set_clear_color(COLOUR_BLACK);

	initAll();

	GUIViews view = MAIN_VIEW;
	while (1) {
		sceKernelPowerTick(0);

		Input input;
		getInput(&input);

		handleInput(view, &input);

		displayGUI();
		
	}

}

int initAll()
{
	initInput();

	initBackground();
	initStatusBar();
	
	initCategoryView();
	initMainView();
	initQueueView();

	return 0;
}

int displayGUI()
{
	displayBackground();
	displayStatusBar();

	displayCategoryView();
	displayMainView();
	displayQueueView();

	return 0;
}