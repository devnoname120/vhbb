TITLE_ID = 	VHBB00001
TARGET   = 	VitaHBBrowser
TITLE    =  Vita HomeBrew Browser
OBJS     = 	lib/cJSON/cJSON.o							\
			src/network.o								\
			src/tools.o									\
			src/api/api.o								\
			src/api/json.o								\
			src/vhbb.o									\
			src/vpk_install/vpk_install.o				\
			src/vpk_install/archive.o					\
			src/vpk_install/sha1/sha1.o					\
			src/vpk_install/file.o						\
			src/minizip/ioapi.o							\
			src/minizip/unzip.o							\
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
			assets/img_item_panel_highlight.o			\
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
			assets/img_dialog_btn_cancel_pressed.o		\
			assets/img_statsbar_battery.o				\
			assets/img_inst_que1.o						\
			assets/img_inst_que2.o						\
			assets/img_inst_dialog.o					\
			src/vitaAudio/ogg/util/memory.o				\
			src/vitaAudio/ogg/util/float-to-int16.o		\
			src/vitaAudio/ogg/util/decode-frame.o		\
			src/vitaAudio/ogg/decode/stb_vorbis.o		\
			src/vitaAudio/ogg/decode/setup.o			\
			src/vitaAudio/ogg/decode/seek.o				\
			src/vitaAudio/ogg/decode/packet.o			\
			src/vitaAudio/ogg/decode/io.o				\
			src/vitaAudio/ogg/decode/decode.o			\
			src/vitaAudio/ogg/decode/crc32.o			\
			src/vitaAudio/ogg/api/version.o				\
			src/vitaAudio/ogg/api/seek-tell.o			\
			src/vitaAudio/ogg/api/read-int16.o			\
			src/vitaAudio/ogg/api/read-float.o			\
			src/vitaAudio/ogg/api/open-file.o			\
			src/vitaAudio/ogg/api/open-callbacks.o		\
			src/vitaAudio/ogg/api/open-buffer.o			\
			src/vitaAudio/ogg/api/info.o				\
			src/vitaAudio/ogg/api/close.o				\
			src/vitaAudio/wav/wav.o						\
			src/vitaAudio/buffer/buffer.o				\
			src/vitaAudio/vitaAudio.o					
			

LIBS = -lm -lvita2d -lSceKernel_stub -lSceDisplay_stub -lSceGxm_stub \
	-lSceSysmodule_stub -lSceCtrl_stub -lSceTouch_stub -lScePgf_stub \
	-lSceCommonDialog_stub -lfreetype -lpng -ljpeg -lz -lm -lc \
	-lSceNet_stub -lSceNetCtl_stub -lSceHttp_stub \
	-lftpvita -lSceAppMgr_stub -lSceAppUtil_stub -lSceIme_stub -lScePower_stub -lSceAudio_stub -lSceAudiodec_stub \
	src/vpk_install/libpromoter/libScePromoterUtil_stub.a

EXTRA_NIDS = src/vpk_install/libpromoter/promoterutil.json
	
BIN = bin
	   
	   
	
PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -O3 -std=c99 -Ilib/cJSON
ASFLAGS = $(CFLAGS)
PSVITAIP = 10.188.178.81

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
		\
		--add assets/snd/snd_tap.ogg=resources/snd/snd_tap.ogg \
		--add assets/snd/snd_installed.wav=resources/snd/snd_installed.wav \
	$(BIN)/$(TARGET).vpk
	
	

$(BIN)/eboot.bin: $(BIN)/$(TARGET).velf
	vita-make-fself $< $@

%.velf: %.elf
	vita-elf-create $< $@ $(EXTRA_NIDS)

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
