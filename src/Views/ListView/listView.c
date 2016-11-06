#define LIST_BASE_X 62
#define LIST_BASE_Y 115

#include <global_include.h>

#define LIST_MIN_Y 79
#define LIST_MAX_Y 543 // This ordinate is included too
#define LIST_DELTA_Y (LIST_MAX_Y - LIST_MIN_Y)
#define LIST_HEIGHT (LIST_DELTA_Y + 1)


extern Category *categoryList;

int selectedCat = 0;

ListView categoryListView[countof(categoryList)]; 


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
	int lastShown = (firstHidden + LIST_DELTA_Y) % ITEM_HEIGHT;
	int lastHidden = ITEM_HEIGHT - lastShown;

	// Number of elements between the first displayed element
	// and the last displayed element
	int inbetween = (LIST_DELTA_Y  - firstShown + lastHidden) / ITEM_HEIGHT;
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
	return LIST_MAX_Y - itemPosY(last) + 1 == ITEM_HEIGHT ? last : last-1; 
}

int initListView()
{
	initListItem();
	for (int i=0; i < countof(categoryListView); i++) {
		ListView *cList = &categoryListView[i];
		cList->posY = 0;
		cList->selectedItem = -1;
		cList->momentum = 0;
		// FIXME: Depends on what the list contains
		cList->hblist_s = 7;
	}
	return 0;
}

int handleListViewInput(int focus, Input *input)
{
	ListView *cList = curList();

	if (focus) {
		// Touch has the priority over the dpad
		if (touchAlreadyPressed(input)) {
			cList->selectedItem = -1;

			int touchDifY;
			unsigned long timeDif;

			touchDifference(input, NULL, &touchDifY, &timeDif);
			
			cList->posY = MIN(ITEM_HEIGHT*cList->hblist_s - LIST_HEIGHT, MAX(0, cList->posY - touchDifY));
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
	dbg_printf(DEBUG, "First displayed item: %d", firstDisplayedItem());
	dbg_printf(DEBUG, "Last displayed item: %d", lastDisplayedItem());
	for (int i=firstDisplayedItem(); i <= lastDisplayedItem(); i++) { 
		displayListItem(itemPosY(i));
	}

	if (curList()->selectedItem >= 0) {
		displayListItemHighlight(itemPosY(curList()->selectedItem));
	}
	return 0;
}

