cmake_minimum_required(VERSION 3.19)
project(lab5 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets PrintSupport)

qt_standard_project_setup()

qt_add_executable(lab5
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    carddialog.cpp
    carddialog.h
    parent.cpp
    parent.h
)

target_link_libraries(lab5
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::PrintSupport
)

file(COPY "${CMAKE_SOURCE_DIR}/images" DESTINATION "${CMAKE_BINARY_DIR}/")

include(GNUInstallDirs)

install(TARGETS lab5
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET lab5
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
