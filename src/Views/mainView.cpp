#include "background.h"
#include <texture.h>
#include "mainView.h"

MainView::MainView() = default;

int MainView::HandleInput(int focus, const Input& input)
{
	categoryView.HandleInput(focus, input);
	return 0;
}

int MainView::Display()
{
	background.Display();
	categoryView.Display();
	statusBar.Display();
	return 0;
}
