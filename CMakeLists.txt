cmake_minimum_required(VERSION 3.23)

project(FileSorter
    VERSION 1.2.1
    DESCRIPTION "Sorts your downloads automatically."
    HOMEPAGE_URL "https://github.com/Zheng-Bote/file-sorter"
    LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Standard-Setup
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
qt_standard_project_setup()

# --- Configure (für rz_config.hpp) ---
set(EXECUTABLE_NAME "${PROJECT_NAME}")
set(PROG_LONGNAME "FileSorter for Download-Folder")
set(PROG_CREATED "2025")
set(PROG_AUTHOR "ZHENG Bote")
set(PROG_ORGANIZATION_NAME "ZHENG Robert")
set(PROG_ORGANIZATION_DOMAIN "net.hase-zheng")

if(Qt6_FOUND)
    set(CMAKE_QTV "${Qt6_VERSION}")
else()
    set(CMAKE_QTV "Unknown")
endif()

# Konfigurations-Header erstellen
add_subdirectory(configure) 

# 1. Executable definieren
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/MainWindow.cpp
    src/FileSorter.cpp
    src/AboutDialog.cpp

    include/MainWindow.hpp
    include/FileSorter.hpp
    include/AboutDialog.hpp

    # Windows Icon (.rc Datei verweist auf .ico)
    resources/windows_icon.rc
)

if(APPLE)
    target_sources(${PROJECT_NAME} PRIVATE resources/app_icon.icns)
    set_source_files_properties(resources/app_icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_ICON_FILE app_icon.icns
        MACOSX_BUNDLE_GUI_IDENTIFIER "${PROG_ORGANIZATION_DOMAIN}.${PROJECT_NAME}"
        MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_COPYRIGHT "${PROG_ORGANIZATION_NAME}"
    )
endif()

# 2. Includes & Ressourcen
target_include_directories(${PROJECT_NAME} PRIVATE include)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc")
    qt_add_resources(${PROJECT_NAME} "resources"
        PREFIX "/"
        FILES resources.qrc
    )
endif()

# 3. Übersetzungen
qt_add_translations(${PROJECT_NAME}
    TS_FILES translations/${PROJECT_NAME}_de.ts
    RESOURCE_PREFIX "/i18n"
)

# 4. Linken
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# 5. Windows-Einstellungen (GUI-App, keine Konsole)
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
endif()

# ==============================================================================
# INSTALLATION (Dateien kopieren)
# ==============================================================================

# Exe in das Wurzelverzeichnis des Pakets kopieren
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION .
)

# Readme hinzufügen
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    install(FILES README.md DESTINATION .)
endif()

# ==============================================================================
# DEPLOYMENT
# ==============================================================================
if(WIN32)
    # Windeployqt suchen
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    # Windeployqt ausführen, um DLLs direkt neben die Exe zu legen
    install(CODE "
        message(STATUS \"Running windeployqt to allow portable ZIP usage...\")
        execute_process(
            COMMAND \"${WINDEPLOYQT_EXECUTABLE}\"
            --dir \"\${CMAKE_INSTALL_PREFIX}\"
            --no-compiler-runtime
            --no-opengl-sw
            --no-system-d3d-compiler
            \"\${CMAKE_INSTALL_PREFIX}/${EXECUTABLE_NAME}.exe\"
        )
    ")
elseif(APPLE)
    # Macdeployqt suchen
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

    # Macdeployqt ausführen
    install(CODE "
        message(STATUS \"Running macdeployqt to bundle Qt dependencies...\")
        execute_process(
            COMMAND \"${MACDEPLOYQT_EXECUTABLE}\"
            \"\${CMAKE_INSTALL_PREFIX}/${EXECUTABLE_NAME}.app\"
            -always-overwrite
            RESULT_VARIABLE macdeployqt_res
        )
        if(NOT macdeployqt_res EQUAL 0)
            message(WARNING \"macdeployqt failed with code \${macdeployqt_res}\")
        endif()
    ")
endif()

# ==============================================================================
# CPACK (ZIP Paket erstellen)
# ==============================================================================
set(CPACK_PACKAGE_VENDOR "${PROG_ORGANIZATION_NAME}")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_CONTACT "robert@hase-zheng.net")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Portable FileSorter Application")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")

# Generator für macOS festlegen (DragNDrop für DMG)
if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
else()
    set(CPACK_GENERATOR "ZIP")
endif()

# Module laden
include(CPack)

# ==============================================================================
# LINUX SPECIFIC (AppImage Vorbereitung)
# ==============================================================================
if(UNIX AND NOT APPLE)
    # 1. Das Programm installieren (nach /usr/bin im AppImage)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
    )

    # 2. Desktop-Datei installieren (nach /usr/share/applications)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/FileSorter.desktop")
        install(FILES resources/FileSorter.desktop DESTINATION share/applications)
    endif()

    # 3. Icon installieren (nach /usr/share/icons/hicolor/256x256/apps)
    # Damit linuxdeploy es automatisch findet und einbettet
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/app_icon.png")
        install(FILES resources/app_icon.png 
                DESTINATION share/icons/hicolor/256x256/apps 
                RENAME app_icon.png)
    endif()

    # 4. Custom Target "appimage" hinzufügen
    # Damit kannst du "cmake --build . --target appimage" aufrufen
    add_custom_target(appimage
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/create_appimage.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Erstelle Linux AppImage..."
        USES_TERMINAL
    )
endif()

# --- 5. Custom Generators (Original Code) ---

# CMake File API Query (Datei erzeugen)
set(QUERY_DIR "${CMAKE_BINARY_DIR}/.cmake/api/v1/query/client-project")
file(MAKE_DIRECTORY "${QUERY_DIR}")
file(WRITE "${QUERY_DIR}/codemodel-v2" "")

# libs.txt Generierung (Timing-sicher via GENERATE)
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/libs.txt"
     CONTENT "$<TARGET_PROPERTY:${PROJECT_NAME},LINK_LIBRARIES>")
