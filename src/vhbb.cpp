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
	
	Background background;
	CategoryView categoryView;
	StatusBar statusBar;
	// Init queue view too

	GUIViews curView = LIST_VIEW;
	while (1) {
		//sceKernelPowerTick(0);
		vita2d_start_drawing();
		vita2d_clear_screen();

		input.Get();
		//input.Propagate(curView); // TODO: Rework function
		categoryView.HandleInput(1, input);	

		background.Display();
		categoryView.Display();
		statusBar.Display();

		vita2d_end_drawing();
		vita2d_swap_buffers();
		sceDisplayWaitVblankStart();	
	}

	#ifdef PSP2SHELL
	psp2shell_exit();
	#endif

	return 0;
}

