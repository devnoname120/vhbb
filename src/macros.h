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
 
// INFORMATION
#define VHBB_VERSION					"0.7.4"
#define MAX_ITEMS						500

// CLOUD ADDRESSES
#define VHBB_CLOUD_ADDRESS_ROOT 			"http://www.playlight.com.au/vita/vhbb/"
#define VHBB_CLOUD_ADDRESS_ASSETS 			"http://www.playlight.com.au/vita/vhbb/dta/assets/"
#define VHBB_CLOUD_ADDRESS_FILES 			"http://www.playlight.com.au/vita/vhbb/dta/files/"

// CLOUD FILES
#define VHBB_CLOUD_FILE_DATABASE			"http://www.playlight.com.au/vita/vhbb/vhbb.db"

// APPLICATION ADDRESSES
#define VHBB_APP_ADDRESS_ROOT 				"ux0:/data/VitaHbBrowser/"
#define VHBB_APP_ADDRESS_STORAGE 			"ux0:/data/VitaHbBrowser/dta/"
#define VHBB_APP_ADDRESS_STORAGE_ICONS  	"ux0:/data/VitaHbBrowser/dta/icons/"
#define VHBB_APP_ADDRESS_STORAGE_PREVS  	"ux0:/data/VitaHbBrowser/dta/prevs/"
#define VHBB_APP_ADDRESS_STORAGE_FILES  	"ux0:/data/VitaHbBrowser/dta/files/"

// APPLICATION FILES
#define VHBB_APP_FILE_DATABASE				"ux0:/data/VitaHbBrowser/dta/vhbb.db"
#define VHBB_APP_FILE_SYSTEMFILE 		 	"ux0:/data/VitaHbBrowser/sys/sys.config"
#define VHBB_APP_FILE_LOGCAT 		 		"ux0:/data/VitaHbBrowser/sys/vhbb.log"
#define VHBB_APP_FILE_DLque				    "ux0:/data/VitaHbBrowser/sys/que.assets"

// MATH
#define lerp(value, from_max, to_max) ((((value *10) *(to_max *10)) /(from_max *10)) /10)
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))


// DIALOGS
#define PROGRESS_BAR_WIDTH					342
#define PROGRESS_BAR_HEIGHT					36
