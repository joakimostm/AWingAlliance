set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH /usr/local/lib/cmake/)

add_library(actors actor/actor.cpp actor/camera.cpp actor/ship.cpp actor/laser.cpp actor/billboard.cpp)

add_executable(awing main.cpp model.cpp keyboard.cpp shaders.cpp terrain.cpp spline.cpp parser.cpp renderer.cpp game_state.cpp fbo.cpp)

target_link_libraries(awing actors ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})


if(APPLE)
    target_link_libraries(awing "-framework GLUT" "-framework OpenGL")
else()
    target_link_libraries(awing -lglut)
endif()