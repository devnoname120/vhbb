typedef struct {
	SceCtrlData pad;
	SceTouchData touch;
} Input;

int initInput()
{
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, 1);
	return 0;	
}

int getInput(Input *input)
{
	sceCtrlPeekBufferPositive(0, &(input->pad), 1);
	sceTouchPeek(SCE_TOUCH_PORT_FRONT, &(input->touch), 1);
	return 0;
}

int handleInput(GUIViews view, Input *input)
{
	switch (view) {

	case MAIN_VIEW:
		handleCategoryViewInput(1, input);
		handleMainViewInput(1, input);
		handleQueueViewInput(0, input);
		break;

	case QUEUE_VIEW:
		handleCategoryViewInput(0, input);
		handleMainViewInput(0, input);
		handleQueueViewInput(1, input);
		break;
	}

	return 0;

}