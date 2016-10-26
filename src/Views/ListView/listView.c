#define LIST_BASE_X 62
#define LIST_BASE_Y 115

#include <global_include.h>

#define LIST_MIN_Y 79
#define LIST_MAX_Y 543


extern Category *categoryList;

int selectedCat = 0;
int selectedItem = 0;

ListView categoryListView[countof(categoryList)]; 

int initListView()
{
	memset(&categoryListView, 0, countof(categoryList));

	initListItem();
	return 0;
}

int handleListViewInput(int focus, Input *input)
{
	ListView *cList = &(categoryListView[selectedCat]);

	if (focus) {
		// Touch has the priority over the dpad
		if (touchPressed(input)) {

			//cList->momentum
		} else {
			if (input->pad.buttons & SCE_CTRL_UP && selectedItem > 0) {
				cList->selectedItem--;
			} else if (input->pad.buttons & SCE_CTRL_DOWN && selectedItem < cList->hblist_s - 1) {
				cList->selectedItem++;
			}
		}
	}

	return 0;
}

int displayListView()
{
	ListView *cList = &(categoryListView[selectedCat]);

	int firstItem = (int)cList->posY / ITEM_HEIGHT;
	int lastItem = firstItem + (int)(LIST_MAX_Y - LIST_MIN_Y - cList->posY % ITEM_HEIGHT) / ITEM_HEIGHT;

	for (int i=firstItem; i <= lastItem; i++) {
		int itemPosY = LIST_MIN_Y + i * ITEM_HEIGHT - cList->posY;
		displayListItem(itemPosY);
	}

	return 0;
}