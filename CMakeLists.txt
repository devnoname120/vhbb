cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()


set(SHORT_NAME VitaHBBrowser)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Vita Homebrew Browser")
set(VITA_TITLEID  "VHBB00001")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -g -Wall -Wextra -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-q -g -Wall -Wextra -std=c++11")

include_directories(
  src/
)

FUNCTION(ADD_RESOURCES out_var)
  SET(result)
  FOREACH(in_f ${ARGN})
    SET(out_f "${CMAKE_CURRENT_BINARY_DIR}/${in_f}.o")
    GET_FILENAME_COMPONENT(out_dir ${out_f} DIRECTORY)
    ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${out_dir}
      COMMAND ${CMAKE_LINKER} -r -b binary -o ${out_f} ${in_f}
      DEPENDS ${in_f}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Building resource ${out_f}"
      VERBATIM
      )
    LIST(APPEND result ${out_f})
  ENDFOREACH()
  SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()

file(GLOB_RECURSE res_files RELATIVE ${CMAKE_SOURCE_DIR} assets/*.png assets/*.jpeg assets/*.yml assets/*.ttf assets/*.wav assets/*.ogg)
add_resources(PROJECT_RESOURCES ${res_files})

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp")
add_executable(${SHORT_NAME}
  ${PROJECT_RESOURCES}
  ${PROJECT_SOURCE_FILES}
)

target_link_libraries(${SHORT_NAME}
  yaml-cpp
  m
  vita2d
  SceDisplay_stub
  SceGxm_stub
  SceSysmodule_stub
  SceCtrl_stub
  SceTouch_stub
  ScePgf_stub
  SceCommonDialog_stub
  freetype
  png
  jpeg
  z
  m
  c
  SceNet_stub
  SceNetCtl_stub
  SceHttp_stub
  ftpvita
  SceAppMgr_stub
  SceAppUtil_stub
  SceIme_stub
  ScePower_stub
  SceAudio_stub
  SceAudiodec_stub
  pthread
)

vita_create_self(eboot.bin ${SHORT_NAME} UNSAFE)
vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml

  FILE assets/fonts/segoeui.ttf resources/fonts/segoeui.ttf
  FILE assets/homebrews.yml resources/homebrews.yml
)

add_custom_target(send
  COMMAND curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/${VITA_TITLEID}/
  DEPENDS eboot.bin
)

add_custom_target(psp2shell
  COMMAND psp2shell_cli $(PSVITAIP) 3333 reload eboot.bin
  DEPENDS eboot.bin
)