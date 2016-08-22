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

#include "vhbb.h"
#include "tools.h"







int main()
	{
	init();
	// SHOW SPLASH
	vita2d_start_drawing();
	vita2d_clear_screen();
	vita2d_draw_texture( img_splash, 0, 0 );
	vita2d_end_drawing();
	vita2d_swap_buffers();
	boot();
	
	while(1)
		{
		
		
		
		
		}
	
	shutdown();
	return 0;
	}