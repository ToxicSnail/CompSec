cmake_minimum_required(VERSION 3.5) 
project(Lab4Encryptor)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR}) 
add_executable(encryptor main.cpp Encryptor.cpp DirWalker.cpp Aes256.cpp )

target_link_libraries(encryptor ${OPENSSL_LIBRARIES})