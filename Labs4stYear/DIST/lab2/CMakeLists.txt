cmake_minimum_required(VERSION 3.10)
project(MyAllocatorTest)

# Добавляем поддиректорию с аллокатором
add_subdirectory(allocator)

# Добавляем поддиректорию с тестовым проектом
add_subdirectory(my_test_project)
