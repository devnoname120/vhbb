cmake_minimum_required(VERSION 2.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if (DEFINED ENV{VITASDK})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
    else ()
        message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
    endif ()
endif ()


set(SHORT_NAME VitaHBBrowser)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(ENV{PKG_CONFIG_PATH} "$ENV{VITASDK}/arm-vita-eabi/lib/pkgconfig")
include(FindPkgConfig)

set(VITA_APP_NAME "Vita Homebrew Browser")
set(VITA_TITLEID "VHBB00001")
set(UPDATER_TITLEID "VHBBUPDT1")
set(VITA_VERSION "00.91")

# Remove leading 0s from version
string(REGEX REPLACE "0+0\\." "0." VITA_VERSION_HUMAN ${VITA_VERSION})

set(VERSION_YAML_URL "https://vhbb.download/version.php")
set(UPDATE_URL "https://github.com/devnoname120/vhbb/releases/download/${VITA_VERSION_HUMAN}/VitaHBBrowser.vpk")
# Old test URLs
#set(VERSION_YAML_URL "https://github.com/robsdedude/vhbb/raw/updateTestBranch/release/version.yml")
#set(UPDATE_URL "https://github.com/robsdedude/vhbb/raw/updateTestBranch/release/VitaHBBrowser.vpk")


set(PACKAGE_TEMP_FOLDER "ux0:/temp/pkg/")

add_subdirectory(src_updater)
create_updater("${SHORT_NAME}" "${VITA_TITLEID}" ${UPDATER_TITLEID} "${PACKAGE_TEMP_FOLDER}")

option(DEBUGNET "Enable debugnet for logging" ON)

# Default build type
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Default build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release") # For CMake GUI
endif ()


find_package(Git)
if (NOT Git_FOUND)
    message(WARNING "Git not found, using unknown as tag...")
    add_definitions(-DGIT_COMMIT="unknown")
else ()
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE DGIT_COMMIT
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )

    add_definitions(-DGIT_COMMIT="${DGIT_COMMIT}")
endif ()


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -std=c++17")

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
    FOREACH (in_f ${ARGN})
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
    ENDFOREACH ()
    SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()

file(GLOB_RECURSE res_files RELATIVE ${CMAKE_SOURCE_DIR}
        assets/head.bin
        assets/*.png
        assets/*.jpeg
        assets/*.yml
        assets/*.ttf
        assets/*.wav
        assets/*.ogg)

add_resources(PROJECT_RESOURCES ${res_files})

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.c")

set(VITA_ELF_NAME ${SHORT_NAME}.elf)
add_executable(${VITA_ELF_NAME}
        ${PROJECT_RESOURCES}
        ${PROJECT_SOURCE_FILES}
        )

target_compile_definitions(${VITA_ELF_NAME}
        PRIVATE
        VITA_VERSION="${VITA_VERSION}"
        VITA_TITLEID="${VITA_TITLEID}"
        VHBB_SHORT_NAME="${SHORT_NAME}"
        PACKAGE_TEMP_FOLDER="${PACKAGE_TEMP_FOLDER}"
        UPDATER_TITLEID="${UPDATER_TITLEID}"
        VERSION_YAML_URL="${VERSION_YAML_URL}"
        )

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug mode")
    add_definitions(-DDEBUG)
    if (DEBUGNET)
        message("Debugnet turned on")
        add_definitions(-DDEBUGNET)
        target_link_libraries(${VITA_ELF_NAME} debugnet)

        get_filename_component(debugnetipFilePath debugnetip.txt REALPATH)
        if (NOT EXISTS ${debugnetipFilePath})
            message(FATAL_ERROR "You need to put an IP in debugnetip.txt to enable the DEBUGNET option.")
        endif ()

        file(STRINGS ${debugnetipFilePath} DEBUGNETIP)
        add_definitions(-DDEBUGNETIP="${DEBUGNETIP}")
    endif ()
endif (CMAKE_BUILD_TYPE MATCHES Debug)

target_link_libraries(${VITA_ELF_NAME}
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
        SceDriverUser_stub
        SceAppUtil_stub
        ScePromoterUtil_stub
        SceIme_stub
        ScePower_stub
        SceAudio_stub
        SceAudiodec_stub
        SceVshBridge_stub
        pthread
        )

vita_create_self(${SHORT_NAME}.self ${VITA_ELF_NAME} UNSAFE UNCOMPRESSED)
vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
        VERSION ${VITA_VERSION}
        NAME ${VITA_APP_NAME}
        FILE sce_sys/icon0.png sce_sys/icon0.png
        FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
        FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
        FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml

        FILE assets/icons.zip resources/icons.zip
        FILE assets/fonts/segoeui.ttf resources/fonts/segoeui.ttf

        FILE ${CMAKE_BINARY_DIR}/${${SHORT_NAME}_UPDATER_EBOOT_NAME} resources/updater/eboot.bin
        FILE ${CMAKE_BINARY_DIR}/${${SHORT_NAME}_UPDATER_SFO_NAME} resources/updater/param.sfo
        )

add_dependencies(${SHORT_NAME}.vpk
        ${${SHORT_NAME}_UPDATER_EBOOT_NAME}
        ${${SHORT_NAME}_UPDATER_SFO_NAME}
        )

set(RELEASE_DIR ${CMAKE_SOURCE_DIR}/release)
add_custom_target(release
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${SHORT_NAME}.vpk ${RELEASE_DIR}/
        COMMAND ${CMAKE_COMMAND} -E echo "version: \"${VITA_VERSION}\"" > "${RELEASE_DIR}/version.yml"
        COMMAND ${CMAKE_COMMAND} -E echo "url: \"${UPDATE_URL}\"" >> "${RELEASE_DIR}/version.yml"
        VERBATIM
        COMMENT "Creating new release files"
        DEPENDS ${SHORT_NAME}.vpk
        )

get_filename_component(psvitaipFilePath psvitaip.txt REALPATH)
if (EXISTS "${psvitaipFilePath}")
    file(STRINGS "${psvitaipFilePath}" PSVITAIP)

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
else ()
    message("Couldn't find psvitaip.txt. Put the IP-address of your vita there to get new build targets.")
endif ()
