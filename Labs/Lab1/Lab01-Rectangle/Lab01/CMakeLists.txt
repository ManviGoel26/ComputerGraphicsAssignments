cmake_minimum_required(VERSION 3.18)
project(lab01)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_VERBOSE_MAKEFILE 1)
set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)

set(APP_TARGET ${PROJECT_NAME})

#link_libraries(${GLFW_LIBRARY_DIRS})

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCES
	"main2.cpp"
	"imgui/imgui_impl_glfw.cpp"
	"imgui/imgui_impl_opengl3.cpp"
	"imgui/imgui.cpp"
	"imgui/imgui_demo.cpp"
	"imgui/imgui_draw.cpp"
	"imgui/imgui_widgets.cpp"
	)

add_executable(${APP_TARGET} ${SOURCES})

target_include_directories(${APP_TARGET} PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(${APP_TARGET} PRIVATE ${GLFW_INCLUDE_DIRS})
target_include_directories(${APP_TARGET} PRIVATE ${OPENGL_INCLUDE_DIR})

target_link_libraries( ${APP_TARGET}
	glfw
	${OPENGL_LIBRARIES}
	GLEW::GLEW
)
