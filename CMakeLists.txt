# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER g++)


cmake_minimum_required(VERSION 3.10)
project(App)


set(CMAKE_CXX_STANDARD 17) #gán giá trị dữ liệu cho biến CMAKE_CXX_STANDARD
set(CMAKE_CXX_STANDARD_REQUIRED ON) #gán giá trị dữ liệu cho biến CMAKE_CXX_STANDARD_REQUIRED

# include(FetchContent)  # Include module FetchContent to download raylib
# FetchContent_Declare(raylib
#     GIT_REPOSITORY https://github.com/raysan5/raylib.git
#     GIT_TAG 5.0
# )
# FetchContent_MakeAvailable(raylib)


set(RAYLIB_PATH "C:/raylib") # Set the path to raylib


include_directories(${RAYLIB_PATH}/include) # Include the directory with raylib.h
link_directories(${RAYLIB_PATH}/lib) # Include the directory with raylib.lib


include_directories(include) # Include the directory with your headers


file(GLOB_RECURSE SOURCES "source/*.cpp")  # Get all .cpp files in src/ folder


add_executable(main ${SOURCES}) # Tạo file thực thi từ file main.cpp


target_link_libraries(main raylib -lopengl32 -lgdi32 -lwinmm) # Link the executable with raylib
