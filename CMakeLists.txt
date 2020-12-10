cmake_minimum_required(VERSION 3.9)
project(qlearn)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX") # TODO: MSVC Support
else()
    if(UNIX)
        # Not Verified to work
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -lstdc++fs -lX11 -lGL -lpthread -lpng")
    elseif(WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static")
    endif()
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
option(BUILD_SHARED_LIBS "" off)

add_subdirectory(extern)

set(SOURCE_FILES

    # GAME SHIT
    source/game.cpp
    source/game.h

    source/main.cpp

)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC "extern")
target_include_directories(${PROJECT_NAME} PUBLIC "source") # to allow paths relative to project root
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE zlib ${OPENGL_LIBRARIES})