ADD_EXECUTABLE(04_mesh_loader WIN32 main.cpp)

TARGET_LINK_LIBRARIES(04_mesh_loader core)
target_include_directories(04_mesh_loader PRIVATE ../include)

TARGET_COMPILE_DEFINITIONS(04_mesh_loader PRIVATE UNICODE _UNICODE)
TARGET_COMPILE_DEFINITIONS(04_mesh_loader PRIVATE RESOURCES_PATH="${PROJECT_SOURCE_DIR}/resources")
