#include "splash_thread.h"
#include <global_include.h>

extern unsigned char _binary_assets_spr_img_splash_png_start;
extern unsigned char _binary_assets_spr_gekihen_splash_png_start;


void splash_thread(SceSize args_size, void *args)
{
    vita2d_texture *tex_splash = vita2d_load_PNG_buffer(&_binary_assets_spr_gekihen_splash_png_start);

    int alpha = 0;
    for (; alpha < 255 - 4; alpha+=4) {
        vita2d_start_drawing();
        vita2d_clear_screen();
        vita2d_draw_texture_tint(tex_splash, 0, 0, RGBA8(255, 255, 255, alpha));
        vita2d_end_drawing();
        vita2d_swap_buffers();
        sceDisplayWaitVblankStart();
    }


    vita2d_start_drawing();
    vita2d_clear_screen();
    vita2d_draw_texture(tex_splash, 0, 0);
    vita2d_end_drawing();
    vita2d_swap_buffers();
    sceDisplayWaitVblankStart();
    sceKernelDelayThread(5 * 1000 * 1000);

    alpha = 255;
    for (; alpha > 4; alpha-=4) {
        vita2d_start_drawing();
        vita2d_clear_screen();
        vita2d_draw_texture_tint(tex_splash, 0, 0, RGBA8(255, 255, 255, alpha));
        vita2d_end_drawing();
        vita2d_swap_buffers();
        sceDisplayWaitVblankStart();
    }

    vita2d_free_texture(tex_splash);

    vita2d_texture *tex_splash2 = vita2d_load_PNG_buffer(&_binary_assets_spr_img_splash_png_start);

    for (alpha=0; alpha < 255 - 4; alpha+=4) {
        vita2d_start_drawing();
        vita2d_clear_screen();
        vita2d_draw_texture_tint(tex_splash2, 0, 0, RGBA8(255, 255, 255, alpha));
        vita2d_end_drawing();
        vita2d_swap_buffers();
        sceDisplayWaitVblankStart();
    }

    vita2d_start_drawing();
    vita2d_clear_screen();
    vita2d_draw_texture(tex_splash2, 0, 0);
    vita2d_end_drawing();
    vita2d_swap_buffers();
    sceDisplayWaitVblankStart();

    vita2d_free_texture(tex_splash2);

    sceKernelExitDeleteThread(0);
}
