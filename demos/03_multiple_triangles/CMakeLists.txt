ADD_EXECUTABLE(03_multiple_triangles WIN32 main.cpp)

TARGET_LINK_LIBRARIES(03_multiple_triangles core)
target_include_directories(03_multiple_triangles PRIVATE ../include)

TARGET_COMPILE_DEFINITIONS(03_multiple_triangles PRIVATE UNICODE _UNICODE)
TARGET_COMPILE_DEFINITIONS(03_multiple_triangles PRIVATE RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
