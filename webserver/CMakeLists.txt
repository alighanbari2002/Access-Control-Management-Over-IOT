cmake_minimum_required(VERSION 3.14)

project(CPSwebserver LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS WebSockets)


add_executable(CPSwebserver
  main.cpp
  webserver.h webserver.cpp
  filehandler.h filehandler.cpp
  resourcefiles.qrc
  websocket.h websocket.cpp
  defs.h
  websocketresponsehandler.h websocketresponsehandler.cpp
  cpstextformatmanip.h
  cpstextformatmanip.cpp
  webcontainer.h webcontainer.cpp
)
target_link_libraries(CPSwebserver Qt${QT_VERSION_MAJOR}::Core Qt6::Network Qt6::WebSockets)

include(GNUInstallDirs)
install(TARGETS CPSwebserver
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
