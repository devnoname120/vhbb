#include "listView.h"

#include <activity.h>
#include <Views/HomebrewView/homebrewView.h>
#include <cmath>


// Ordinate of where the item should be displayed on the screen
unsigned int ListView::itemPosY(unsigned int i)
{
	return LIST_MIN_Y + i * ITEM_HEIGHT - posY;
}

// First item that is (partially or fully) displayed
unsigned int ListView::firstDisplayedItem()
{
	return (unsigned int)posY / ITEM_HEIGHT;
}

// Last item that is (partially or fully) displayed
unsigned int ListView::lastDisplayedItem()
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
	return std::min<unsigned int>(listItems.size() - 1, last);
}

unsigned int ListView::firstFullyDisplayedItem()
{
	unsigned int first = firstDisplayedItem();
	// Return first displayed item if it starts right where we begin to display
	return posY % ITEM_HEIGHT == 0 ? first : first + 1;
}

unsigned int ListView::lastFullyDisplayedItem()
{
	unsigned int last = lastDisplayedItem();
	return LIST_MAX_Y - itemPosY(last) + 1 >= ITEM_HEIGHT ? last : last-1;
}


int ListView::coordinateToItem(double coordY)
{
    log_printf(DBG_DEBUG, "posY: %d", posY);
	double absoluteY = posY + coordY - LIST_MIN_Y;
    log_printf(DBG_DEBUG, "absoluteY: %f", absoluteY);

	unsigned int item = (unsigned int)absoluteY / ITEM_HEIGHT;
    log_printf(DBG_DEBUG, "item: %d", item);

	// Item does not exist
	if (item >= listItems.size()) {
		return -1;
	}

	return item;
}

int ListView::updateScrollSpeed(double &scrollSpeed, unsigned long timeDif)
{
	const double acceleration = 0.0000000010;

	if (std::abs(scrollSpeed) <= acceleration * (double)timeDif) {
		scrollSpeed = 0;
		return 0;
	}

	double sign = scrollSpeed > 0 ? -1 : 1;

	scrollSpeed = scrollSpeed + sign * acceleration * (double)timeDif;
	return 0;
}

ListView::ListView(std::vector<Homebrew> homebrews):
	font_43(Font(std::string(FONT_DIR "segoeui.ttf"), 43))
{
    log_printf(DBG_DEBUG, "posY: %d", posY);
    log_printf(DBG_DEBUG, "homebrews size: %d", homebrews.size());
	for (Homebrew hb : homebrews) {
		listItems.emplace_back(hb);
	}
}

int ListView::HandleInput(int focus, const Input& input)
{

	if (input.KeyNewPressed(SCE_CTRL_SELECT)) {
        log_printf(DBG_DEBUG, "posY: %d", posY);
        log_printf(DBG_DEBUG, "firstDisplayedItem(): %d", firstDisplayedItem());
        log_printf(DBG_DEBUG, "lastDisplayedItem(): %d", lastDisplayedItem());
	}

	// Calculate new posY from scrolling speed and time elapsed
	unsigned long timeDif;
	input.TouchDifference(nullptr, nullptr, &timeDif);
	//log_printf(DBG_DEBUG, "timeDif: %u", timeDif);
	//log_printf(DBG_DEBUG, "posY before: %d", posY);
	if (!input.TouchPressed())
		posY = std::min<unsigned int>(std::max<int>(ITEM_HEIGHT*listItems.size() - LIST_HEIGHT, 0), std::max<int>(posY + (int)(scrollSpeed * (double)timeDif), 0));

	updateScrollSpeed(scrollSpeed, timeDif);

	//log_printf(DBG_DEBUG, "posY after: %d", posY);

	if (focus) {
		if (input.TouchStopPressed()) {
			if (preSelectedItem != -1) {
				selectedItem = preSelectedItem;
                log_printf(DBG_DEBUG, "Clicked, adding view...");
				try {
					Activity::get_instance()->AddView(std::make_shared<HomebrewView>(listItems.at(selectedItem).homebrew));
				} catch (const std::exception &ex) {
                    log_printf(DBG_ERROR, ex.what());
				}
			}

		}
		// Touch has the priority over the dpad
		if (input.TouchPressed()) {
			double touchY;
			input.TouchCoordinates(nullptr, &touchY);
			if (input.TouchNewPressed()) {
				preSelectedItem = coordinateToItem(touchY);
			}
			//log_printf(DEBUG, "Scroll speed: %f", touchSpeedY);
			//log_printf(DEBUG, "speed: %f", fabs(touchSpeedY));

			double touchSpeedY;
			input.TouchSpeed(NULL, &touchSpeedY, NULL);

			if (input.TouchNewMovement() && fabs(touchSpeedY) != 0.) {
				preSelectedItem = -1;
			}


			if (input.TouchAlreadyMovement()) {
				// If the touch speed is not negligible, unselect
				if (fabs(touchSpeedY) > LIST_SELECTION_MAX_SPEED) {
					preSelectedItem = -1;
					selectedItem = -1;
				}

				double touchDifY;
				unsigned long timeDif;

				input.TouchDifference(NULL, &touchDifY, &timeDif);
				posY = std::min(std::max<int>(ITEM_HEIGHT*listItems.size() - LIST_HEIGHT, 0),
								std::max<int>(0, posY - touchDifY));
				scrollSpeed = -touchSpeedY;
                log_printf(DBG_DEBUG, "scrollSpeed: %f", scrollSpeed);
			}
			//momentum
		// There is a selected item
		} else if (selectedItem != -1) {
			// TODO: timer instead so that the user can stay clicked
			if (input.KeyNewPressedRapidFire(SCE_CTRL_UP) && selectedItem > 0) {
				selectedItem--;
				resetHighlight();
				// Scroll up if the selected item is outside of view
				if (selectedItem < firstFullyDisplayedItem()) {
					posY = std::max<int>(0, posY - ITEM_HEIGHT);
				}

			} else if (input.KeyNewPressedRapidFire(SCE_CTRL_DOWN) && selectedItem < listItems.size() - 1) {
				selectedItem++;
				resetHighlight();
                log_printf(DBG_DEBUG, "lastFullyDisplayedItem(): %d", lastFullyDisplayedItem());
				// Scroll down if the selected item is outside of view
				if (selectedItem > lastFullyDisplayedItem()) {
					posY += ITEM_HEIGHT;
				}
			} else if (input.KeyNewPressed(SCE_CTRL_ENTER)) {
                log_printf(DBG_DEBUG, "Pressed, adding view...");
				try {
					Activity::get_instance()->AddView(std::make_shared<HomebrewView>(listItems.at(selectedItem).homebrew));
				} catch (const std::exception &ex) {
                    log_printf(DBG_ERROR, ex.what());
				}
			}
		// No item is selected
		} else {
			if (input.KeyNewPressedRapidFire(SCE_CTRL_UP)) {
				selectedItem = lastFullyDisplayedItem();
			} else if (input.KeyNewPressedRapidFire(SCE_CTRL_DOWN)) {
				selectedItem = firstFullyDisplayedItem();
			}
		}
	}

	return 0;
}


int ListView::resetHighlight()
{
	itemHighlightAlpha = 255;
	itemHighlightDirection = 0;
	return 0;
}


int ListView::Display()
{
	if (listItems.empty()) {
		font_43.DrawCentered(Rectangle(Point(0, LIST_MIN_Y), Point(SCREEN_WIDTH, LIST_MAX_Y)), "No results");
		return 0;
	}
	for (int i=firstDisplayedItem(); i <= lastDisplayedItem(); i++) {
		listItems[i].Display(itemPosY(i), i == selectedItem, itemHighlightAlpha);
	}
	if (itemHighlightDirection) {
		if (itemHighlightAlpha < 255 - itemHighlightSpeed)
			itemHighlightAlpha += itemHighlightSpeed;
		else
			itemHighlightDirection = !itemHighlightDirection;
	}
	else {
		if (itemHighlightAlpha > itemHighlightSpeed)
			itemHighlightAlpha -= itemHighlightSpeed;
		else
			itemHighlightDirection = !itemHighlightDirection;
	}
	return 0;
}

