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

set(ENV{PKG_CONFIG_PATH} "$ENV{VITASDK}/arm-vita-eabi/lib/pkgconfig")
include(FindPkgConfig)

set(VITA_APP_NAME "Vita Homebrew Browser")
set(VITA_TITLEID  "VHBB00001")
set(VITA_VERSION "00.82")


option(DEBUGNET "Enable debugnet for logging" ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Default build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release") # For CMake GUI
endif()



set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter")

#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -Og")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Og")

#set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
pkg_check_modules(CURLPP REQUIRED curlpp)

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

file(GLOB_RECURSE res_files RELATIVE ${CMAKE_SOURCE_DIR} assets/head.bin assets/*.png assets/*.jpeg assets/*.yml assets/*.ttf assets/*.wav assets/*.ogg)
add_resources(PROJECT_RESOURCES ${res_files})

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.c")
add_executable(${SHORT_NAME}
  ${PROJECT_RESOURCES}
  ${PROJECT_SOURCE_FILES}
)

if(DEBUGNET)
  add_definitions(-DDEBUGNET)
  target_link_libraries(${SHORT_NAME} debugnet)
  file(STRINGS "debugnetip.txt" DEBUGNETIP)
  add_definitions(-DDEBUGNETIP="${DEBUGNETIP}")
endif()

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
  SceSsl_stub
  ${CURLPP_LDFLAGS}
  ${CURL_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ftpvita
  SceAppMgr_stub
  SceAppUtil_stub
  ScePromoterUtil_stub
  SceIme_stub
  ScePower_stub
  SceAudio_stub
  SceAudiodec_stub
  pthread
)

vita_create_self(${SHORT_NAME}.self ${SHORT_NAME} UNSAFE UNCOMPRESSED)
vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml

  FILE assets/icons.zip resources/icons.zip
  FILE assets/fonts/segoeui.ttf resources/fonts/segoeui.ttf
)

file(STRINGS "psvitaip.txt" PSVITAIP)

add_custom_target(send
  COMMAND curl -T ${SHORT_NAME}.self ftp://${PSVITAIP}:1337/ux0:/app/${VITA_TITLEID}/eboot.bin
  DEPENDS ${SHORT_NAME}.self
)

add_custom_target(shellsend
  COMMAND psp2shell_cli ${PSVITAIP} 3333 load ${VITA_TITLEID} ${SHORT_NAME}.self
  DEPENDS ${SHORT_NAME}.self
)

add_custom_target(vpksend
  COMMAND curl -T ${SHORT_NAME}.vpk ftp://${PSVITAIP}:1337/ux0:/
  DEPENDS ${SHORT_NAME}.vpk
)
