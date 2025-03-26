cmake_minimum_required(VERSION 3.10)
project(SAT_DPLL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматическая генерация moc-файлов (при работе с Qt)
set(CMAKE_AUTOMOC ON)

# Находим Qt (модуль Core)
find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(sat_dpll
    main.cpp
    BBV.cpp
    boolinterval.cpp
    boolequation.cpp
    MinOccurenceBranchingStrategy.cpp
)

target_include_directories(sat_dpll
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/allocator"
)

target_link_libraries(sat_dpll
    PRIVATE
        Qt5::Core
        allocator
)