#define LIST_BASE_X 62
#define LIST_BASE_Y 115

#include <global_include.h>

#define LIST_MIN_Y 79
#define LIST_MAX_Y 543 // This ordinate is included too
#define LIST_RANGE_Y (LIST_MAX_Y - LIST_MIN_Y)
#define LIST_HEIGHT (LIST_RANGE_Y + 1)

// Max speed px/ms above which selected item is no more selected
#define LIST_SELECTION_MAX_SPEED 0.000070


extern Category *categoryList;

int selectedCat = 0;

ListView categoryListView[categoryList_s]; 


// Retrieve the list of the currently selected category
ListView *curList()
{
	return &(categoryListView[selectedCat]);
}

// Ordinate of where the item should be displayed on the screen
int itemPosY(int i)
{
	return LIST_MIN_Y + i * ITEM_HEIGHT - curList()->posY; 
}

// First item that is (partially or fully) displayed
int firstDisplayedItem()
{
	return (int)curList()->posY / ITEM_HEIGHT;
}

// Last item that is (partially or fully) displayed
int lastDisplayedItem()
{
	// Height of the hidden/shown part of the first item
	int firstHidden = curList()->posY % ITEM_HEIGHT;
	int firstShown = ITEM_HEIGHT - firstHidden;

	// Height of the hidden/shown part of the last item 
	int lastShown = (firstHidden + LIST_RANGE_Y) % ITEM_HEIGHT;
	int lastHidden = ITEM_HEIGHT - lastShown;

	// Number of elements between the first displayed element
	// and the last displayed element
	int inbetween = (LIST_RANGE_Y  - firstShown + lastHidden) / ITEM_HEIGHT;
	int last = firstDisplayedItem() + inbetween;

	// Don't display an element that doesn't exist
	return MIN(curList()->hblist_s - 1, last);
}

int firstFullyDisplayedItem()
{
	int first = firstDisplayedItem();
	// Return first displayed item if it starts right where we begin to display
	return curList()->posY % ITEM_HEIGHT == 0 ? first : first + 1;
}

int lastFullyDisplayedItem()
{
	int last = lastDisplayedItem();
	return LIST_MAX_Y - itemPosY(last) + 1 >= ITEM_HEIGHT ? last : last-1; 
}


int coordinateToItem(double coordY)
{
	double absoluteY = curList()->posY + coordY - LIST_MIN_Y;
	int item = (int)absoluteY / ITEM_HEIGHT;

	// Item does not exist
	if (item >= curList()->hblist_s) {
		return -1;
	}

	return item;
}


int initListView()
{
	initListItem();
	for (int i=0; i < categoryList_s; i++) {
		ListView *cList = &(categoryListView[i]);
		cList->posY = 0;
		cList->selectedItem = -1;
		cList->momentum = 0;
		// FIXME: Depends on what the list contains
		cList->hblist_s = i+2;
	}
	return 0;
}

int handleListViewInput(int focus, Input *input)
{
	ListView *cList = curList();

	if (keyNewPressed(input, SCE_CTRL_SELECT)) {
		dbg_printf(DEBUG, "posY: %d", cList->posY);
		dbg_printf(DEBUG, "firstDisplayedItem(): %d", firstDisplayedItem());
		dbg_printf(DEBUG, "lastDisplayedItem(): %d", lastDisplayedItem());
	}

	if (focus) {
		// Touch has the priority over the dpad
		if (touchPressed(input)) {
			//dbg_printf(DEBUG, "Scroll speed: %f", touchSpeedY);
			//dbg_printf(DEBUG, "speed: %f", fabs(touchSpeedY));

			if (touchNewPressed(input)) {
				dbg_printf(DEBUG, "clicked on something");
				double touchY;
				touchCoordinates(input, NULL, &touchY);

				cList->selectedItem = coordinateToItem(touchY);
			} else {
				double touchSpeedY;
				touchSpeed(input, NULL, &touchSpeedY, NULL);
				
				// If the touch speed is not negligible, unselect
				if (fabs(touchSpeedY) > LIST_SELECTION_MAX_SPEED)
					cList->selectedItem = -1;

				double touchDifY;
				unsigned long timeDif;

				touchDifference(input, NULL, &touchDifY, &timeDif);
				cList->posY = MIN(MAX(ITEM_HEIGHT*cList->hblist_s - LIST_HEIGHT, 0),
								MAX(0, cList->posY - touchDifY));
			}
			//cList->momentum
		// There is a selected item
		} else if (cList->selectedItem != -1) {
			// TODO: timer instead so that the user can stay clicked
			if (keyNewPressed(input, SCE_CTRL_UP) && cList->selectedItem > 0) {
				cList->selectedItem--;

				// Scroll up if the selected item is outside of view
				if (cList->selectedItem < firstFullyDisplayedItem()) {
					cList->posY = MAX(0, cList->posY - ITEM_HEIGHT);
				}
								
			} else if (keyNewPressed(input, SCE_CTRL_DOWN) && cList->selectedItem < cList->hblist_s - 1) {
				cList->selectedItem++;
				dbg_printf(DEBUG, "lastFullyDisplayedItem(): %d", lastFullyDisplayedItem());
				// Scroll down if the selected item is outside of view
				if (cList->selectedItem > lastFullyDisplayedItem()) {
					cList->posY += ITEM_HEIGHT;
				}
			}
		// No item is selected
		} else {
			if (keyNewPressed(input, SCE_CTRL_UP)) {
				cList->selectedItem = lastFullyDisplayedItem();
			} else if (keyNewPressed(input, SCE_CTRL_DOWN)) {
				cList->selectedItem = firstFullyDisplayedItem();
			} 
		}
	}

	return 0;
}

int displayListView()
{
	for (int i=firstDisplayedItem(); i <= lastDisplayedItem(); i++) { 
		displayListItem(itemPosY(i));
	}

	if (curList()->selectedItem >= 0) {
		displayListItemHighlight(itemPosY(curList()->selectedItem));
	}
	return 0;
}

