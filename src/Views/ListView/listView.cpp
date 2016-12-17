#define LIST_BASE_X 62
#define LIST_BASE_Y 115

#include <global_include.h>

#define LIST_MIN_Y 79
#define LIST_MAX_Y 543 // This ordinate is included too
#define LIST_RANGE_Y (LIST_MAX_Y - LIST_MIN_Y)
#define LIST_HEIGHT (LIST_RANGE_Y + 1)

// Max speed px/ms above which selected item is no more selected
#define LIST_SELECTION_MAX_SPEED 0.000070



// Ordinate of where the item should be displayed on the screen
int ListView::itemPosY(int i)
{
	return LIST_MIN_Y + i * ITEM_HEIGHT - posY; 
}

// First item that is (partially or fully) displayed
int ListView::firstDisplayedItem()
{
	return (int)posY / ITEM_HEIGHT;
}

// Last item that is (partially or fully) displayed
int ListView::lastDisplayedItem()
{
	// Height of the hidden/shown part of the first item
	int firstHidden = posY % ITEM_HEIGHT;
	int firstShown = ITEM_HEIGHT - firstHidden;

	// Height of the hidden/shown part of the last item 
	int lastShown = (firstHidden + LIST_RANGE_Y) % ITEM_HEIGHT;
	int lastHidden = ITEM_HEIGHT - lastShown;

	// Number of elements between the first displayed element
	// and the last displayed element
	int inbetween = (LIST_RANGE_Y  - firstShown + lastHidden) / ITEM_HEIGHT;
	int last = firstDisplayedItem() + inbetween;

	// Don't display an element that doesn't exist
	return MIN(count - 1, last);
}

int ListView::firstFullyDisplayedItem()
{
	int first = firstDisplayedItem();
	// Return first displayed item if it starts right where we begin to display
	return posY % ITEM_HEIGHT == 0 ? first : first + 1;
}

int ListView::lastFullyDisplayedItem()
{
	int last = lastDisplayedItem();
	return LIST_MAX_Y - itemPosY(last) + 1 >= ITEM_HEIGHT ? last : last-1; 
}


int ListView::coordinateToItem(double coordY)
{
	double absoluteY = posY + coordY - LIST_MIN_Y;
	int item = (int)absoluteY / ITEM_HEIGHT;

	// Item does not exist
	if (item >= count) {
		return -1;
	}

	return item;
}


ListView::ListView()
{
	count = 10;
	listItems.reserve(count);
	for (int i=0; i < count; i++) {
		listItems.push_back(ListItem());
	}
}

int ListView::HandleInput(int focus, const Input& input)
{

	if (input.KeyNewPressed(SCE_CTRL_SELECT)) {
		dbg_printf(DBG_DEBUG, "posY: %d", posY);
		dbg_printf(DBG_DEBUG, "firstDisplayedItem(): %d", firstDisplayedItem());
		dbg_printf(DBG_DEBUG, "lastDisplayedItem(): %d", lastDisplayedItem());
	}

	if (focus) {
		// Touch has the priority over the dpad
		if (input.TouchPressed()) {
			//dbg_printf(DEBUG, "Scroll speed: %f", touchSpeedY);
			//dbg_printf(DEBUG, "speed: %f", fabs(touchSpeedY));

			double touchSpeedY;
			input.TouchSpeed(NULL, &touchSpeedY, NULL);

			if (input.TouchNewMovement() && fabs(touchSpeedY) == 0.) {
				double touchY;
				input.TouchCoordinates(NULL, &touchY);

				selectedItem = coordinateToItem(touchY);
				dbg_printf(DBG_DEBUG, "Clicked on element #%d", selectedItem);
			} else {

				// If the touch speed is not negligible, unselect
				if (fabs(touchSpeedY) > LIST_SELECTION_MAX_SPEED)
					selectedItem = -1;

				double touchDifY;
				unsigned long timeDif;

				input.TouchDifference(NULL, &touchDifY, &timeDif);
				posY = MIN(MAX(ITEM_HEIGHT*count - LIST_HEIGHT, 0),
								MAX(0, posY - touchDifY));
			}
			//momentum
		// There is a selected item
		} else if (selectedItem != -1) {
			// TODO: timer instead so that the user can stay clicked
			if (input.KeyNewPressed(SCE_CTRL_UP) && selectedItem > 0) {
				selectedItem--;

				// Scroll up if the selected item is outside of view
				if (selectedItem < firstFullyDisplayedItem()) {
					posY = MAX(0, posY - ITEM_HEIGHT);
				}
								
			} else if (input.KeyNewPressed(SCE_CTRL_DOWN) && selectedItem < count - 1) {
				selectedItem++;
				dbg_printf(DEBUG, "lastFullyDisplayedItem(): %d", lastFullyDisplayedItem());
				// Scroll down if the selected item is outside of view
				if (selectedItem > lastFullyDisplayedItem()) {
					posY += ITEM_HEIGHT;
				}
			}
		// No item is selected
		} else {
			if (input.KeyNewPressed(SCE_CTRL_UP)) {
				selectedItem = lastFullyDisplayedItem();
			} else if (input.KeyNewPressed(SCE_CTRL_DOWN)) {
				selectedItem = firstFullyDisplayedItem();
			} 
		}
	}

	return 0;
}

int ListView::Display()
{
	for (int i=firstDisplayedItem(); i <= lastDisplayedItem(); i++) { 
		listItems[i].Display(itemPosY(i), i == selectedItem);
	}

	return 0;
}

