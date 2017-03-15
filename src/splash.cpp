#include "splash.h"
#include <global_include.h>

extern unsigned char _binary_assets_spr_img_splash_png_start;


void displaySplash()
{
    vita2d_texture *tex_splash = vita2d_load_PNG_buffer(&_binary_assets_spr_img_splash_png_start);
    vita2d_start_drawing();
    vita2d_clear_screen();
    vita2d_draw_texture(tex_splash, 0, 0);
    vita2d_end_drawing();
    vita2d_swap_buffers();
    sceDisplayWaitVblankStart();
    vita2d_free_texture(tex_splash);
}
