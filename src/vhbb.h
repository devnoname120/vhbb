#pragma once

#include <global_include.h>

#include "Views/CategoryView/categoryView.h"
#include "Views/background.h"
#include "Views/statusBar.h"
#include "input.h"

typedef enum {
	LIST_VIEW,
	QUEUE_VIEW
} GUIViews;

int main();
int initAll();
int displayGUI();
