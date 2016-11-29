#include <global_include.h>

int main()
{
	dbg_init();

	#ifdef PSP2SHELL
	psp2shell_init(3333, 0);
	dbg_printf(DBG_INFO, "PSPSHELL started on port 3333");
	#endif

	vita2d_init();
	vita2d_set_clear_color(COLOR_BLACK);

	Input input;

	StatusBar statusBar;
	CategoryView categoryView;
	ListView listView;
	// Init queue view too

	GUIViews curView = LIST_VIEW;
	while (1) {
		//sceKernelPowerTick(0);

		vita2d_start_drawing();
		vita2d_clear_screen();

		input.Get();
		input.Propagate(curView);
		displayGUI();

		vita2d_end_drawing();
		vita2d_swap_buffers();
		sceDisplayWaitVblankStart();	
	}

	#ifdef PSP2SHELL
	psp2shell_exit();
	#endif

	return 0;
}

// TODO : get rid of this function
int displayGUI()
{
	displayBackground();

	displayListView();
	displayCategoryView();
	//displayQueueView();

	displayStatusBar();
	return 0;
}
