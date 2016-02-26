cmake_minimum_required(VERSION 2.6)

project(Primordial)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

# Find SFML
find_package(SFML 2 REQUIRED system window graphics audio)

if(NOT SFML_FOUND)
	message(SEND_ERROR "Failed to find SFML.")
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

if(NOT OPENGL_FOUND)
	message(SEND_ERROR "Failed to find OpenGL.")
endif()

# Find GLEW
find_package(GLEW REQUIRED)

if(NOT GLEW_FOUND)
	message(SEND_ERROR "Failed to find GLEW.")
endif()

# Find Bullet
find_package(Bullet REQUIRED)

if(NOT BULLET_FOUND)
        message(SEND_ERROR "Failed to find Bullet.")
endif()

# Find X11
find_package(X11 REQUIRED)

if(NOT X11_FOUND)
        message(SEND_ERROR "Failed to find X11.")
endif()

find_package( Boost 1.40 COMPONENTS system thread REQUIRED )
if(NOT Boost_FOUND)
        message(SEND_ERROR "Failed to find Boost.")
endif()


set(HDRS
    ./prog/engine/include/terrain.h
    ./prog/engine/include/renderer.h
    ./prog/engine/include/postprocstage.h
    ./prog/engine/include/smallvisual.h
    ./prog/engine/include/uniformbufferobject.h
    ./prog/engine/include/sound.h
    ./prog/engine/include/master.h
    ./prog/engine/include/geometry.h
    ./prog/engine/include/mechanics.h
    ./prog/engine/include/resourcemanager.h
    ./prog/engine/include/shader.h
    ./prog/engine/include/foliage.h
    ./prog/engine/include/shadowmap.h
    ./prog/engine/include/closedmap.h
    ./prog/engine/include/rigidbody.h
    ./prog/engine/include/camera.h
    ./prog/engine/include/combinationstage.h
    ./prog/engine/include/actor.h
    ./prog/engine/include/debugdrawer.h
    ./prog/engine/include/backgroundmaster.h
    ./prog/engine/include/light.h
    ./prog/engine/include/stats.h
    ./prog/engine/include/mesh.h
    ./prog/engine/include/physicsworld.h
    ./prog/engine/include/object3d.h
    ./prog/engine/include/charactercontroller.h
    ./prog/engine/include/textshader.h
    ./prog/engine/include/world.h
    ./prog/engine/include/shaderbase.h
    ./prog/engine/include/renderstage.h
    ./prog/engine/include/grassshader.h
    ./prog/engine/include/parser.h
    ./prog/engine/include/worldobject.h
    ./prog/engine/include/prop.h
    ./prog/engine/include/artificialintelligence.h
    ./prog/engine/include/signals.h
    ./prog/engine/include/skeleton.h
    ./prog/engine/include/texture.h
    ./prog/engine/include/global.h
    ./prog/engine/include/shadowinstshader.h
    ./prog/engine/include/creature.h
    ./prog/engine/include/soundemitter.h
    ./prog/engine/include/skyshader.h
    ./prog/engine/include/creaturesignals.h
    ./prog/engine/dependencies/glerrorcodes.h
    ./prog/engine/dependencies/threadingwrapper.h
    ./prog/engine/dependencies/helperfunctions.hpp
    ./prog/engine/dependencies/timeseries.hpp
    ./prog/engine/dependencies/memstreamer.hpp
    ./prog/engine/dependencies/threadiobuffers.hpp
    ./prog/engine/dependencies/packedlinkedlist.hpp
    ./prog/engine/dependencies/quadtree.hpp
)
 
set(SRCS
    ./prog/engine/main.cpp
    ./prog/engine/dependencies/glerrorcodes.cpp
    ./prog/engine/dependencies/threadingwrapper.cpp
    ./prog/engine/src/artificialintelligence.cpp
    ./prog/engine/src/smallvisual.cpp
    ./prog/engine/src/physicsworld.cpp
    ./prog/engine/src/uniformbufferobject.cpp
    ./prog/engine/src/actor.cpp
    ./prog/engine/src/shadowinstshader.cpp
    ./prog/engine/src/creature.cpp
    ./prog/engine/src/worldobject.cpp
    ./prog/engine/src/charactercontroller.cpp
    ./prog/engine/src/textshader.cpp
    ./prog/engine/src/shader.cpp
    ./prog/engine/src/combinationstage.cpp
    ./prog/engine/src/grassshader.cpp
    ./prog/engine/src/camera.cpp
    ./prog/engine/src/terrain.cpp
    ./prog/engine/src/renderer.cpp
    ./prog/engine/src/stats.cpp
    ./prog/engine/src/shadowmap.cpp
    ./prog/engine/src/postprocstage.cpp
    ./prog/engine/src/world.cpp
    ./prog/engine/src/parser.cpp
    ./prog/engine/src/renderstage.cpp
    ./prog/engine/src/mechanics.cpp
    ./prog/engine/src/mesh.cpp
    ./prog/engine/src/skyshader.cpp
    ./prog/engine/src/rigidbody.cpp
    ./prog/engine/src/object3d.cpp
    ./prog/engine/src/backgroundmaster.cpp
    ./prog/engine/src/light.cpp
    ./prog/engine/src/geometry.cpp
    ./prog/engine/src/sound.cpp
    ./prog/engine/src/soundemitter.cpp
    ./prog/engine/src/shaderbase.cpp
    ./prog/engine/src/texture.cpp
    ./prog/engine/src/global.cpp
    ./prog/engine/src/master.cpp
    ./prog/engine/src/foliage.cpp
    ./prog/engine/src/skeleton.cpp
    ./prog/engine/src/debugdrawer.cpp
    ./prog/engine/src/prop.cpp
)

include_directories(
    ${SFML_INCLUDE_DIRS}
    ${GL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${X11_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ./prog/engine/include/
    ./prog/engine/dependencies/
    ./prog/engine/src/
)

add_executable(Primordial WIN32 ${HDRS} ${SRCS})

set_target_properties(Primordial PROPERTIES DEBUG_POSTFIX _d)

# message(STATUS "=====DEBUG MESSAGES======")
# message(STATUS "SFML_LIBRARIES:         " ${SFML_LIBRARIES})

message(STATUS "====INCLUDE DIRS====")
message(STATUS "SFML_INCLUDE_DIRS:         " ${SFML_INCLUDE_DIRS})
message(STATUS "OPENGL_INCLUDE_DIRS:         " ${OPENGL_INCLUDE_DIRS})
message(STATUS "GLEW_INCLUDE_DIRS:         " ${GLEW_INCLUDE_DIRS})
message(STATUS "BULLET_INCLUDE_DIRS:         " ${BULLET_INCLUDE_DIRS})
message(STATUS "X11_INCLUDE_DIRS:         " ${X11_INCLUDE_DIRS})
message(STATUS "BOOST_INCLUDE_DIRS:         " ${BOOST_INCLUDE_DIRS})

message(STATUS "=====LIBRARIES======")
message(STATUS "SFML_LIBRARIES:         " ${SFML_LIBRARIES})
message(STATUS "OPENGL_LIBRARIES:         " ${OPENGL_LIBRARIES})
message(STATUS "GLEW_LIBRARIES:         " ${GLEW_LIBRARIES})
message(STATUS "BULLET_LIBRARIES:         " ${BULLET_LIBRARIES})
message(STATUS "X11_LIBRARIES:         " ${X11_LIBRARIES})
message(STATUS "Boost_LIBRARIES:         " ${Boost_LIBRARIES})

target_link_libraries(Primordial
        ${SFML_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${BULLET_LIBRARIES}
	${X11_LIBRARIES}
        ${Boost_LIBRARIES}
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)

if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)

# copy assets to binary output directiry
file(COPY ./prog/engine/assets DESTINATION ${PROJECT_BINARY_DIR}/dist/bin)
file(COPY ./prog/engine/shaders DESTINATION ${PROJECT_BINARY_DIR}/dist/bin)

#if(UNIX)
 
#        install(TARGETS Primordial
#		RUNTIME DESTINATION bin
#		CONFIGURATIONS All)
   
#endif(UNIX)
