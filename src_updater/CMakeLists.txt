cmake_minimum_required(VERSION 2.8)

macro(SET_EXPORTED VAR VAL EXPORT_PREFIX)
    set("${VAR}" "${VAL}")
    set("${EXPORT_PREFIX}${VAR}" "${VAL}" PARENT_SCOPE)
endmacro(SET_EXPORTED)

set(UPDATER_SRC_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

function(create_updater MAIN_SHORT_NAME MAIN_TITLEID UPDATER_TITLEID PKG_FOLDER)
    # e.g. create_updater(VitaHBBrowser VHBB00001 VHBB00002 ux0:/temp/pkg/)

    if (NOT (MAIN_SHORT_NAME AND MAIN_TITLEID AND UPDATER_TITLEID AND PKG_FOLDER))
        message(WARNING "Not all of `create_updater`'s the arguments were given:\n"
                "\"${MAIN_SHORT_NAME}\", \"${MAIN_TITLEID}\", \"${UPDATER_TITLEID}\", \"${PKG_FOLDER}\"")
    endif ()

    set_exported(UPDATER_SHORT_NAME "${MAIN_SHORT_NAME}Updater" "${MAIN_SHORT_NAME}_")
    set_exported(UPDATER_TITLEID ${UPDATER_TITLEID} "${MAIN_SHORT_NAME}_")
    set_exported(UPDATER_VERSION 00.01 "${MAIN_SHORT_NAME}_")


    set(UPDATER_ELF_NAME ${UPDATER_SHORT_NAME}.elf)
    set(UPDATER_VELF_NAME ${UPDATER_SHORT_NAME}.velf)
    set(UPDATER_SELF_NAME ${UPDATER_SHORT_NAME}.self)
    set_exported(UPDATER_EBOOT_NAME ${UPDATER_SELF_NAME} "${MAIN_SHORT_NAME}_")
    set_exported(UPDATER_SFO_NAME ${UPDATER_SHORT_NAME}_param.sfo "${MAIN_SHORT_NAME}_")

    file(GLOB_RECURSE PROJECT_SOURCE_FILES ${UPDATER_SRC_DIR}/*.h ${UPDATER_SRC_DIR}/*.hpp ${UPDATER_SRC_DIR}/*.cpp ${UPDATER_SRC_DIR}/*.c)

    add_executable(${UPDATER_ELF_NAME}
            ${PROJECT_SOURCE_FILES}
            )

    if (CMAKE_BUILD_TYPE MATCHES Debug AND DEBUGNET)
        target_link_libraries(${UPDATER_ELF_NAME} debugnet)
    endif ()

    target_compile_definitions(${UPDATER_ELF_NAME}
            PRIVATE UPDATE_TITLEID="${MAIN_TITLEID}"
            PACKAGE_DIR="${PKG_FOLDER}"
            UPDATER_VERSION="${UPDATER_VERSION}"
            )
    target_link_libraries(${UPDATER_ELF_NAME}
            SceNetCtl_stub
            SceNet_stub
            SceShellSvc_stub
            SceAppMgr_stub
            SceDriverUser_stub
            ScePromoterUtil_stub
            SceSysmodule_stub
            SceVshBridge_stub
            )

    add_custom_command(OUTPUT ${UPDATER_VELF_NAME}
            COMMAND ${VITA_ELF_CREATE} ${UPDATER_ELF_NAME} ${UPDATER_VELF_NAME}
            DEPENDS ${UPDATER_ELF_NAME}
            COMMENT "Converting to Sony ELF ${UPDATER_VELF_NAME}" VERBATIM
            )
    set(UPDATER_MAKE_FSELF_FLAGS -c)
    add_custom_target(${UPDATER_SELF_NAME}
            COMMAND ${VITA_MAKE_FSELF} ${UPDATER_MAKE_FSELF_FLAGS} ${UPDATER_VELF_NAME} ${UPDATER_SELF_NAME}
            DEPENDS ${UPDATER_VELF_NAME}
            COMMENT "Creating SELF ${UPDATER_SELF_NAME}"
            )

    set(UPDATER_MKSFOEX_FLAGS -s APP_VER=${UPDATER_VERSION} -s TITLE_ID=${UPDATER_TITLEID})
    add_custom_target(${UPDATER_SFO_NAME}
            COMMAND ${VITA_MKSFOEX} ${UPDATER_MKSFOEX_FLAGS} ${UPDATER_SHORT_NAME} ${UPDATER_SFO_NAME}
            COMMENT "Generating param.sfo for ${UPDATER_SHORT_NAME}"
            )
endfunction(create_updater)
