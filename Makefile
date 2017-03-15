TITLE_ID = 	VHBB00001
TARGET   = 	VitaHBBrowser
TITLE    =  Vita HomeBrew Browser

ASSET_DIR  = assets
SOURCE_DIR = src
BIN        = bin

#SOURCES  =  $(shell find $(SOURCE_DIR) -name '*.cpp')
SOURCES = src/singleton.cpp \
		  src/font.cpp \
		  src/texture.cpp \
		  src/date.cpp \
		  src/Views/View.cpp \
		  src/activity.cpp \
          src/Views/statusBar.cpp \
          src/Views/background.cpp \
          src/Views/ListView/listItem.cpp \
          src/Views/ListView/listView.cpp \
          src/Views/CategoryView/categoryView.cpp \
		  src/Views/HomebrewView/homebrewView.cpp \
          src/vhbb.cpp \
		  src/homebrewDownload.cpp \
          src/homebrewRelease.cpp \
		  src/homebrew.cpp \
          src/input.cpp \
          src/shapes.cpp \
		  src/network.cpp \
          src/database.cpp \
          src/debug.cpp

IMAGES   =  $(shell find $(ASSET_DIR) -name '*.png')
OBJS     =  $(SOURCES:%.cpp=%.o) $(IMAGES:%.png=%.o)

DEBUG = 1

ifeq ($(RELEASE), 1)
DEBUG = 0
endif

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CXX     = $(PREFIX)-g++
CFLAGS  = -Wl,-q -g -Wall -Wextra -Isrc/
CXXFLAGS = $(CFLAGS) -std=c++11
ASFLAGS = $(CFLAGS)

PSVITAIP = $(shell head -n 1 psvitaip.txt)
#DEBUGNETIP = $(shell head -n 1 debugnetip.txt)
DEBUGNETIP = $(shell cat debugnetip.txt || ip route get 1 | awk '{print $$NF;exit}')

ifeq ($(DEBUG), 1)
CFLAGS += -O0 -D_DEBUG
else
CFLAGS += -O3
endif

ifeq ($(psp2shell), 1)
CFLAGS += -DPSP2SHELL
LIBS += -lpsp2shell
endif

ifeq ($(debugnet), 1)
CFLAGS += -DDEBUGNET -DDEBUGNETIP="\"$(DEBUGNETIP)\""
LIBS += -ldebugnet
endif

LIBS += -lyaml-cpp -lm -lvita2d -lSceDisplay_stub -lSceGxm_stub \
	-lSceSysmodule_stub -lSceCtrl_stub -lSceTouch_stub -lScePgf_stub \
	-lSceCommonDialog_stub -lfreetype -lpng -ljpeg -lz -lm -lc \
	-lSceNet_stub -lSceNetCtl_stub -lSceHttp_stub \
	-lftpvita -lSceAppMgr_stub -lSceAppUtil_stub -lSceIme_stub -lScePower_stub -lSceAudio_stub -lSceAudiodec_stub \
	-lpthread


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
		--add assets/homebrews.yml=resources/homebrews.yml \
	$(BIN)/$(TARGET).vpk



$(BIN)/eboot.bin: $(BIN)/$(TARGET).velf
	vita-make-fself $< $@

%.velf: %.elf
	vita-elf-create $< $@

$(BIN)/$(TARGET).elf: binfolder $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@


%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

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

psp2shell: $(BIN)/eboot.bin
	psp2shell_cli $(PSVITAIP) 3333 reload $(BIN)/eboot.bin

binfolder:
	@mkdir $(BIN) || true
