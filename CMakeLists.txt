cmake_minimum_required(VERSION 3.23)

project(FileSorter
    VERSION 1.2.1
    DESCRIPTION "Sorts your downloads automatically."
    HOMEPAGE_URL "https://github.com/Zheng-Bote/file-sorter"
    LANGUAGES CXX)

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
# (Annahme: Ihre configure/CMakeLists.txt kümmert sich um rz_config.hpp.in 
#  ODER Sie nutzen hier direkt configure_file wie vorher. Falls add_subdirectory
#  leer ist, nutzen Sie den configure_file Befehl von vorher.)

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
# DEPLOYMENT (DLLs für ZIP vorbereiten)
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

# WICHTIG: Nur ZIP Generator nutzen
set(CPACK_GENERATOR "ZIP")

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
