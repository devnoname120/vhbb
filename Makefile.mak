TITLE_ID 	= 	VHBB00001
TARGET   	= 	vhbb
TITLE    	=  Vita HomeBrew Browser
OBJS     	= 	src/vhbb.o 									\
			src/font/fontDefault.o					 	
			
			

LIBS 		= 	-lvita2d -lSceKernel_stub -lSceDisplay_stub -lSceGxm_stub 								\
				-lSceSysmodule_stub -lSceCtrl_stub -lSceTouch_stub -lScePgf_stub 						\
				-lSceCommonDialog_stub -lfreetype -lpng -ljpeg -lz -lm -lc 								\
				-lSceNet_stub -lSceNetCtl_stub -lSceHttp_stub -lftpvita -lSceAppMgr_stub 				\
				-lSceAppUtil_stub -lSceIme_stub -lScePower_stub -lSceAudio_stub -lSceAudiodec_stub

	

	   
	   
	
PREFIX  	= arm-vita-eabi
CC      	= $(PREFIX)-gcc
CFLAGS  	= -Wl,-q -Wall -O3
ASFLAGS 	= $(CFLAGS)
PSVITAIP 	= 10.0.63
all: $(TARGET).vpk



	
	
%.vpk: eboot.bin
	vita-mksfoex -s TITLE_ID=$(TITLE_ID) "$(TITLE)" param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin \
		--add sce_sys/icon0.png=sce_sys/icon0.png \
		--add sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
		--add sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
		--add sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \
	$(TARGET).vpk
	
	

eboot.bin: $(TARGET).velf
	vita-make-fself $< $@

%.velf: %.elf
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.o: %.png
	$(PREFIX)-ld -r -b binary -o $@ $^

clean:
	@rm -rf $(TARGET).vpk $(TARGET).velf $(TARGET).elf $(OBJS) \
		eboot.bin param.sfo

vpksend: $(TARGET).vpk
	curl -T $(TARGET).vpk ftp://$(PSVITAIP):1337/ux0:/
	@echo "Sent."

send: eboot.bin
	curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/$(TITLE_ID)/
	@echo "Sent."
	
