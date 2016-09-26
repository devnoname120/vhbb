/*
 * 	Copyright (c) 2016 Arkanite
 *
 *	 	-A Native HomeBrew Browser for the Playstation Vita-
 *
 *	This program is free software:
 *		you can redistribute it and/or modify it under the terms of the
 *		GNU General Public License as published by the Free Software Foundation,
 *		either version 3 of the License, or (at your option) any later version.
 *		
 *		This program comes WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *		Public License for more details.
 *		You should have received a copy of the GNU General Public License
 *		along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

// MAIN SCROLL
if ( momentum < -1 || momentum > 1 ){
	clickable = 0;
}
if ( !previewActive && !settingsOpen && !install_que_open ) {
	if ( scroll && ground > 544 && touch_y > 90 ) {
		if ( touch_check ) {
			newy	= 0; newy = originY + ( touch_y - prevy );
			if ( usemomentum == 1 ) {
				momentum = abs( round(( touch_y - prevy ) /2 ));
				if ( touch_y - prevy > 0 ) {
					scrolldir = 1;
				}
				else {
					scrolldir = 0;
				}
			}
			if ( newy > 0 ) {
				newy = 0;
			}
			if ( newy -544 < (-ground) ) {
				newy = (-ground) +544;
			}
			originY = newy; prevy = touch_y;          
		}
		if ( touch_check_released ) {
			prevy = 0; scroll = 0;
		}
	}
	else {
		if ( touch_check_pressed ) {
			scroll = 1; prevy = touch_y;
		}
		if ( usemomentum && momentum > 0 ) {
			newy	= 0;
			if ( scrolldir == 1 ) {
				newy = originY +momentum;
				if ( newy > 0 ) {
					newy = 0; momentum = 0;
				}
			}
			else {
				newy    = originY -momentum;
				if ( newy - 544 <= (-ground) ) {
					newy = (-ground) +544; momentum = 0;
				}
			}
			originY = newy;
			if ( momentum > 0 ) {
				momentum -= 0.2;
			}
		}    
	}
}
// SETTINGS SCROLL
if ( settingsOpen ) {
	if ( scroll && settingsGround > 544 && touch_y > 90 ) {
		if ( touch_check && touch_x < 207 ) {
			newy	= 0; newy = settingsPosY + ( touch_y - prevy );
			if ( usemomentum ) {
				momentum = abs( round(( touch_y - prevy ) /2 ));
				if ( touch_y - prevy > 0 ) {
					scrolldir = 1;
				}
				else {
					scrolldir = 0;
				}
			}
			if ( newy > 0 ) {
				newy = 0;
			}
			if ( newy -544 < (-settingsGround) ) {
				newy = (-settingsGround) +544;
			}
			settingsPosY = newy; prevy = touch_y;          
		}
		if ( touch_check_released ) {
			prevy = 0; scroll = 0;
		}
	}
	else {
		if ( touch_check_pressed && touch_x < 207 ) {
			scroll = 1; prevy = touch_y;
		}
		if ( usemomentum && momentum > 0 ) {
			newy	= 0;
			if ( scrolldir == 1 ) {
				newy = settingsPosY +momentum;
				if ( newy > 0 ) {
					newy = 0; momentum = 0;
				}
			}
			else {
				newy    = settingsPosY -momentum;
				if ( newy - 544 <= (-settingsGround) ) {
					newy = (-settingsGround) +544; momentum = 0;
				}
			}
			settingsPosY = newy;
			if ( momentum > 0 ) {
				momentum -= 0.2;
			}
		}    
	}
}
	
// INSTALL QUE OPEN
if ( install_que_open ) {
	if ( scroll && settingsGround > 544 && touch_y > 96 ) {
		if ( touch_check ) {
			newy	= 0; newy = install_que_posy + ( touch_y - prevy );
			if ( usemomentum ) {
				momentum = abs( round(( touch_y - prevy ) /2 ));
				if ( touch_y - prevy > 0 ) {
					scrolldir = 1;
				}
				else {
					scrolldir = 0;
				}
			}
			if ( newy > 0 ) {
				newy = 0;
			}
			if ( newy -544 < (-settingsGround) ) {
				newy = (-settingsGround) +544;
			}
			install_que_posy = newy; prevy = touch_y;          
		}
		if ( touch_check_released ) {
			prevy = 0; scroll = 0;
		}
	}
	else {
		if ( touch_check_pressed ) {
			scroll = 1; prevy = touch_y;
		}
		if ( usemomentum && momentum > 0 ) {
			newy	= 0;
			if ( scrolldir == 1 ) {
				newy = install_que_posy +momentum;
				if ( newy > 0 ) {
					newy = 0; momentum = 0;
				}
			}
			else {
				newy    = install_que_posy -momentum;
				if ( newy - 544 <= (-settingsGround) ) {
					newy = (-settingsGround) +544; momentum = 0;
				}
			}
			install_que_posy = newy;
			if ( momentum > 0 ) {
				momentum -= 0.2;
			}
		}    
	}
}
	
	
	
	