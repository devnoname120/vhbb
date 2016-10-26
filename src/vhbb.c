#include <global_include.h>

int main()
{
	vita2d_init();
	vita2d_set_clear_color(COLOR_BLACK);

	initAll();

	GUIViews view = LIST_VIEW;
	while (1) {
		sceKernelPowerTick(0);

		vita2d_start_drawing();
		vita2d_clear_screen();

		Input input;
		getInput(&input);

		handleInput(view, &input);

		displayGUI();

		vita2d_end_drawing();
		vita2d_swap_buffers();
		sceDisplayWaitVblankStart();	
	}

	return 0;
}

int initAll()
{
	initInput();

	initBackground();
	initStatusBar();
	
	initCategoryView();
	initListView();
	//initQueueView();

	return 0;
}

int displayGUI()
{
	displayBackground();
	displayStatusBar();

	displayCategoryView();
	displayListView();
	//displayQueueView();

	return 0;
}