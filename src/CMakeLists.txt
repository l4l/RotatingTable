add_subdirectory(table)

include_directories(table)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} table)
