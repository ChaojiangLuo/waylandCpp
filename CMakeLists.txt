cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME "wayland-test")

project(${PROJECT_NAME})

set(SOURCES
	src/main.cpp
	src/wayland/Display.cpp
	src/wayland/Compositor.cpp
	src/wayland/SharedMemory.cpp
	src/wayland/Shell.cpp
	src/wayland/ShellSurface.cpp
	src/wayland/Surface.cpp
)

include_directories(
	src
	${XENBE_INCLUDE_PATH}
	${XEN_INCLUDE_PATH}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")

link_directories(
	${XEN_LIB_PATH}
	${XENBE_LIB_PATH}
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} xenbe xenctrl xenstore xenevtchn xengnttab wayland-client pthread)
