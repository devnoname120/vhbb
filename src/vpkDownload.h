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

int download_file( const char *src, const char *dst )
	{
	netInit();
	httpInit();
	vita2d_end_drawing();
	vita2d_swap_buffers();
	int ret;
	int tpl 			= sceHttpCreateTemplate( "Vita HomeBrew Browser", 2, 1 );
	if ( tpl  < 0 ) 	{ httpTerm(); netTerm(); return tpl; }
	int conn 			= sceHttpCreateConnectionWithURL( tpl, src, 0 );
	if ( conn < 0 ) 	{ httpTerm(); netTerm(); return conn; }
	int req 			= sceHttpCreateRequestWithURL( conn, 0, src, 0 );
	if ( req  < 0 ) 	{ httpTerm(); netTerm(); return req; }
	ret 				= sceHttpSendRequest( req, NULL, 0 );
	if ( ret  < 0 ) 	{ httpTerm(); netTerm(); return ret; }
	unsigned char buf[25600] = {0}; // 4096
	long long length 	= 0;
	ret 				= sceHttpGetResponseContentLength( req, &length );
	int fd 				= sceIoOpen( dst, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 6 );
	int total_read 		= 0;
	if ( fd < 0 ) { httpTerm(); netTerm(); return fd; }
	while (1)
		{
		/// touch CHECKS -----------------------------------
		sceTouchPeek( SCE_TOUCH_PORT_FRONT, &touch, 1 );
		if ( touch.reportNum > 0 )
			{
			touch_check 				= 1;
			touch_x = (lerp(touch.report[0].x, 1919, 960));
			touch_y = (lerp(touch.report[0].y, 1087, 544));
			touch_check_released_prev 	= 0;
			touch_check_released 		= 0;
			if ( touch_check_pressed_prev == 0 ) { touch_check_pressed_prev = 1; touch_check_pressed = 1; }
			else 								 { touch_check_pressed = 0; }
			}
		else
			{
			touch_check 				= 0;
			touch_check_pressed 		= 0;
			touch_check_pressed_prev 	= 0;
			if ( touch_check_released_prev == 0 ) { touch_check_released_prev = 1; touch_check_released = 1; }
			else 								  { touch_check_released = 0; }
			}
		
		if ( touch_check_pressed )
			{	
			if ( point_in_rectangle( touch_x, touch_y, 287, 403, 682, 448 ) )
				{
				btnState_dialogBtn2		= 1;
				}
			}
		
		if ( touch_check_released )
			{
			btnState_dialogBtn2	 		= 0;
			if ( point_in_rectangle( touch_x, touch_y, 287, 403, 682, 448 ) )
				{
				httpTerm(); netTerm();
				remove( dst );
				return -1;
				}
			}
		vita2d_start_drawing();
		int read = sceHttpReadData( req, buf, sizeof(buf) );
		if ( read  < 0 ) { httpTerm(); netTerm(); return read; }
		if ( read == 0 ) { break; }
		ret 			= sceIoWrite(fd, buf, read);
		if ( ret < 0 || ret != read )
			{
			if ( ret < 0 ) httpTerm(); netTerm(); return ret;
			    httpTerm(); netTerm(); return -1;
			}
		total_read += read;
		vita2d_draw_texture( img_dialog_progress_bg, 228, 181 );
		if ( btnState_dialogBtn2 == 0 ) { vita2d_draw_texture( img_dialog_btn_cancel, 287, 403 ); }
		else							{ vita2d_draw_texture( img_dialog_btn_cancel_pressed, 287, 403 ); }
		vita2d_font_draw_textf( font_25, 310, 240, COLOUR_WHITE, 25, "%s%s%s%s", "Downloading:\n\n", previewName, "\n", previewVersion );
		vita2d_draw_rectangle( 310, 325, ((PROGRESS_BAR_WIDTH * total_read) / length), PROGRESS_BAR_HEIGHT, COLOUR_BLUEGREY );
		vita2d_end_drawing();
		vita2d_swap_buffers();
		}
	ret 				= sceIoClose(fd);
	vita2d_start_drawing();
	httpTerm(); netTerm();
	return 0;
	}