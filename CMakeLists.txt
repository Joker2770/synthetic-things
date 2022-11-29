cmake_minimum_required(VERSION 3.10.0)

project(synthetic-things)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-g -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17")

if (POLICY CMP0072)
set(OpenGL_GL_PREFERENCE LEGACY)
endif()

if(NOT TARGET box2d)
    add_subdirectory(src/box2d)
endif()

find_package (PkgConfig REQUIRED)

# Check for ALSA libraries.
pkg_check_modules (ALSA REQUIRED alsa)
find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)
find_package(X11 REQUIRED)

set(POLICY CMP0072)

set(PROJECT_SOURCES
	src/main.cpp
)

include_directories(${OpenGL_INCLUDE_DIR})
# include_directories(${GLUT_INCLUDE_DIR})

include_directories(
				${CMAKE_CURRENT_SOURCE_DIR}/src/
				${CMAKE_CURRENT_SOURCE_DIR}/src/box2d/include/
				${CMAKE_CURRENT_SOURCE_DIR}/src/olcPixelGameEngine/
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} box2d)
target_link_libraries(${PROJECT_NAME} png)
target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ALSA_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARIES})

# Install
if(UNIX)
	set(CMAKE_INSTALL_PREFIX /usr/local)
	install(TARGETS ${PROJECT_NAME} DESTINATION bin)
	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION bin)
elseif(WIN32)
	set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/")
	install(TARGETS ${PROJECT_NAME} DESTINATION synthetic-things)
	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION synthetic-things)
endif()