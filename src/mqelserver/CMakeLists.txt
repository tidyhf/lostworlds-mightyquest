cmake_minimum_required(VERSION 3.15)
project(mqelserver CXX)
set(CMAKE_CXX_STANDARD 17)

# OpenSSL: provide via the vcpkg toolchain file (see ../../build.ps1) or -DOPENSSL_ROOT_DIR=...
find_package(OpenSSL REQUIRED)

# The game-server logic is MQELOffline's frontend/backend (vendored, patched). We compile
# all of it EXCEPT its injected DllMain entrypoint (Appmain.cpp), which we replace with main.cpp.
set(MQEL "${CMAKE_CURRENT_SOURCE_DIR}/../mqeloffline/Source")
file(GLOB_RECURSE MQEL_SRC "${MQEL}/*.cpp")
list(FILTER MQEL_SRC EXCLUDE REGEX "Appmain\\.cpp$")

add_executable(mqelserver main.cpp ${MQEL_SRC})
target_include_directories(mqelserver PRIVATE "${MQEL}")
target_link_libraries(mqelserver PRIVATE ws2_32 OpenSSL::SSL OpenSSL::Crypto)
target_compile_definitions(mqelserver PRIVATE
    _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0601)
set_property(TARGET mqelserver PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
