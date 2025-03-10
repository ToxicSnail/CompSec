add_executable(my_test_project main.cpp)
target_link_libraries(my_test_project PRIVATE allocator)
target_include_directories(my_test_project PRIVATE ${CMAKE_SOURCE_DIR}/allocator)
