


// INFORMATION
#define VHBB_VERSION "0.7.8"
#define MAX_ITEMS 500
// CLOUD ADDRESSES
#define VHBB_CLOUD_ADDRESS_ROOT "http://www.playlight.com.au/Dev/vhbb/"
#define VHBB_CLOUD_ADDRESS_ASSETS "http://www.playlight.com.au/Dev/vhbb/dta/assets/"
#define VHBB_CLOUD_ADDRESS_FILES "http://www.playlight.com.au/Dev/vhbb/dta/files/"
// CLOUD FILES
#define VHBB_CLOUD_FILE_DATABASE "http://www.playlight.com.au/Dev/vhbb/vhbb.db"
// APPLICATION ADDRESSES
#define VHBB_APP_ADDRESS_ROOT "ux0:/data/VitaHbBrowser/"
#define VHBB_APP_ADDRESS_STORAGE "ux0:/data/VitaHbBrowser/dta/"
#define VHBB_APP_ADDRESS_STORAGE_ICONS "ux0:/data/VitaHbBrowser/dta/icons/"
#define VHBB_APP_ADDRESS_STORAGE_PREVS "ux0:/data/VitaHbBrowser/dta/prevs/"
#define VHBB_APP_ADDRESS_STORAGE_FILES "ux0:/data/VitaHbBrowser/dta/files/"
#define VHBB_RES_DIR_FONTS "ux0:/app/VHBB00001/resources/fonts/"
#define VHBB_RES_DIR_SOUND "ux0:/app/VHBB00001/resources/snd/"
// APPLICATION FILES
#define VHBB_APP_FILE_DATABASE "ux0:/data/VitaHbBrowser/dta/vhbb.db"
#define VHBB_APP_FILE_SYSTEMFILE "ux0:/data/VitaHbBrowser/sys/sys.config"
#define VHBB_APP_FILE_LOGCAT "ux0:/data/VitaHbBrowser/sys/vhbb.log"
#define VHBB_APP_FILE_DLque "ux0:/data/VitaHbBrowser/sys/que.assets"
// MATH
#define lerp(value, from_max, to_max) ((((value *10) *(to_max *10)) /(from_max *10)) /10)
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))
// ALIGNMENT
#define ALIGN_CENTER(a, b) ((a - b) / 2)
#define ALIGN_LEFT(x, w) (x - w)
// DIALOGS
#define PROGRESS_BAR_WIDTH 342
#define PROGRESS_BAR_HEIGHT 36