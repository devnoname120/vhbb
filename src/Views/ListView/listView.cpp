#include "listView.h"


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
	unsigned int firstHidden = posY % ITEM_HEIGHT;
	unsigned int firstShown = ITEM_HEIGHT - firstHidden;

	// Height of the hidden/shown part of the last item 
	unsigned int lastShown = (firstHidden + LIST_RANGE_Y) % ITEM_HEIGHT;
	unsigned int lastHidden = ITEM_HEIGHT - lastShown;

	// Number of elements between the first displayed element
	// and the last displayed element
	unsigned int inbetween = (LIST_RANGE_Y  - firstShown + lastHidden) / ITEM_HEIGHT;
	unsigned int last = firstDisplayedItem() + inbetween;

	// Don't display an element that doesn't exist
	return MIN(listItems.size() - 1, last);
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
	dbg_printf(DBG_DEBUG, "posY: %d", posY);
	double absoluteY = posY + coordY - LIST_MIN_Y;
	dbg_printf(DBG_DEBUG, "absoluteY: %f", absoluteY);

	unsigned int item = (int)absoluteY / ITEM_HEIGHT;
	dbg_printf(DBG_DEBUG, "item: %d", item);

	// Item does not exist
	if (item >= listItems.size()) {
		return -1;
	}

	return item;
}

int ListView::updateScrollSpeed(double &scrollSpeed, unsigned long timeDif)
{
	const double acceleration = 0.0000000010;

	if (abs(scrollSpeed) <= acceleration * (double)timeDif) {
		scrollSpeed = 0;
		return 0;
	}

	double sign = scrollSpeed > 0 ? -1 : 1;

	scrollSpeed = scrollSpeed + sign * acceleration * (double)timeDif;
	return 0;
}

ListView::ListView(std::vector<Homebrew> homebrews)
{
	dbg_printf(DBG_DEBUG, "posY: %d", posY);
	dbg_printf(DBG_DEBUG, "homebrews size: %d", homebrews.size());
	for (Homebrew hb : homebrews) {
		listItems.push_back(ListItem(hb));
	}
}

int ListView::HandleInput(int focus, const Input& input)
{

	if (input.KeyNewPressed(SCE_CTRL_SELECT)) {
		dbg_printf(DBG_DEBUG, "posY: %d", posY);
		dbg_printf(DBG_DEBUG, "firstDisplayedItem(): %d", firstDisplayedItem());
		dbg_printf(DBG_DEBUG, "lastDisplayedItem(): %d", lastDisplayedItem());
	}

	// Calculate new posY from scrolling speed and time elapsed
	unsigned long timeDif;
	input.TouchDifference(NULL, NULL, &timeDif);
	//dbg_printf(DBG_DEBUG, "timeDif: %u", timeDif);
	//dbg_printf(DBG_DEBUG, "posY before: %d", posY);
	if (!input.TouchPressed())
		posY = MIN(MAX(ITEM_HEIGHT*listItems.size() - LIST_HEIGHT, 0), MAX(posY + (int)(scrollSpeed * (double)timeDif), 0)); 
	
	updateScrollSpeed(scrollSpeed, timeDif);

	//dbg_printf(DBG_DEBUG, "posY after: %d", posY);

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

				dbg_printf(DBG_DEBUG, "Touch Y: %f", touchY);

				selectedItem = coordinateToItem(touchY);
				dbg_printf(DBG_DEBUG, "Clicked on element #%d", selectedItem);
			} else if (input.TouchAlreadyMovement()) {
				// If the touch speed is not negligible, unselect
				if (fabs(touchSpeedY) > LIST_SELECTION_MAX_SPEED)
					selectedItem = -1;

				double touchDifY;
				unsigned long timeDif;

				input.TouchDifference(NULL, &touchDifY, &timeDif);
				posY = MIN(MAX(ITEM_HEIGHT*listItems.size() - LIST_HEIGHT, 0),
								MAX(0, posY - touchDifY));
				scrollSpeed = -touchSpeedY; 
				dbg_printf(DBG_DEBUG, "scrollSpeed: %f", scrollSpeed);
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
								
			} else if (input.KeyNewPressed(SCE_CTRL_DOWN) && selectedItem < listItems.size() - 1) {
				selectedItem++;
				dbg_printf(DBG_DEBUG, "lastFullyDisplayedItem(): %d", lastFullyDisplayedItem());
				// Scroll down if the selected item is outside of view
				if (selectedItem > lastFullyDisplayedItem()) {
					posY += ITEM_HEIGHT;
				}
			} else if (input.KeyNewPressed(SCE_CTRL_CROSS)) {
				// TODO Create HomebrewView
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

