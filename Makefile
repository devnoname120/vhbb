TITLE_ID = 	VHBB00001
TARGET   = 	VitaHBBrowser
TITLE    =  Vita HomeBrew Browser
OBJS     = 	src/vhbb.o 									\
			assets/img_splash.o							\
			assets/img_bg.o								\
			assets/img_topbar_apps.o					\
			assets/img_topbar_demos.o					\
			assets/img_topbar_emu.o						\
			assets/img_topbar_games.o					\
			assets/img_topbar_new.o						\
			assets/img_topbar_themes.o					\
			assets/img_topbar_util.o					\
			assets/img_btn_search.o						\
			assets/img_item_panel_new.o					\
			assets/img_item_panel_apps.o				\
			assets/img_item_panel_games.o				\
			assets/img_item_panel_emu.o					\
			assets/img_item_panel_util.o				\
			assets/img_item_panel_themes.o				\
			assets/img_item_panel_demos.o				\
			assets/img_preview_btn_download.o			\
			assets/img_preview_btn_download_pressed.o	\
			assets/img_preview_btn_install.o			\
			assets/img_preview_btn_install_pressed.o	\
			assets/img_preview_btn_update.o				\
			assets/img_preview_btn_update_pressed.o		\
			assets/img_preview_infobg.o					\
			assets/img_preview_btn_back.o				\
			assets/img_preview_btn_back_pressed.o		\
			assets/img_icon_default.o					\
			assets/img_preview_default.o				\
			assets/img_settings_about.o					\
			assets/img_settings_about_pressed.o			\
			assets/img_settings_bg.o					\
			assets/img_settings_cont.o					\
			assets/img_settings_cont_pressed.o			\
			assets/img_settings_ctrl.o					\
			assets/img_settings_ctrl_pressed.o			\
			assets/img_settings_snd.o					\
			assets/img_settings_snd_pressed.o			\
			assets/img_settings_topbar.o				\
			assets/img_dialog_progress_bg.o				\
			assets/img_dialog_btn_cancel.o				\
			assets/img_dialog_btn_cancel_pressed.o
			

LIBS = -lvita2d -lSceKernel_stub -lSceDisplay_stub -lSceGxm_stub \
	-lSceSysmodule_stub -lSceCtrl_stub -lSceTouch_stub -lScePgf_stub \
	-lSceCommonDialog_stub -lfreetype -lpng -ljpeg -lz -lm -lc \
	-lSceNet_stub -lSceNetCtl_stub -lSceHttp_stub \
	-lftpvita -lSceAppMgr_stub -lSceAppUtil_stub -lSceIme_stub -lScePower_stub -lSceAudio_stub -lSceAudiodec_stub

	
BIN = bin
	   
	   
	
PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -O3
ASFLAGS = $(CFLAGS)
PSVITAIP = 10.0.63

all: $(BIN)/$(TARGET).vpk

%.vpk: $(BIN)/eboot.bin
	vita-mksfoex -s TITLE_ID=$(TITLE_ID) "$(TITLE)" $(BIN)/param.sfo
	vita-pack-vpk -s $(BIN)/param.sfo -b $(BIN)/eboot.bin \
		--add sce_sys/icon0.png=sce_sys/icon0.png \
		--add sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
		--add sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
		--add sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \
		\
		--add assets/fonts/segoeui.ttf=resources/fonts/segoeui.ttf \
	$(BIN)/$(TARGET).vpk
	
	

$(BIN)/eboot.bin: $(BIN)/$(TARGET).velf
	vita-make-fself $< $@

%.velf: %.elf
	vita-elf-create $< $@

$(BIN)/$(TARGET).elf: binfolder $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

%.o: %.png
	$(PREFIX)-ld -r -b binary -o $@ $^

clean:
	@rm -rf $(BIN) $(OBJS)

vpksend: $(BIN)/$(TARGET).vpk
	curl -T $(BIN)/$(TARGET).vpk ftp://$(PSVITAIP):1337/ux0:/
	@echo "Sent."

send: $(BIN)/eboot.bin
	curl -T $(BIN)/eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/$(TITLE_ID)/
	@echo "Sent."

binfolder:
	@mkdir $(BIN) || true
