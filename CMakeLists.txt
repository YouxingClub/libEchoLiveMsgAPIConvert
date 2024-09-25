cmake_minimum_required(VERSION 3.29)
project(libEchoLiveMsgAPIConvert)

add_subdirectory(test)

set(CMAKE_CXX_STANDARD 20)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(nlohmann_json CONFIG REQUIRED)

add_library(libEchoLiveMsgAPIConvert SHARED libEchoLiveMsgAPIConvert.cpp libEchoLiveMsgAPIConvert.h echoLiveWsAPIStruct.h)

#add_executable(TypeEchoMsgConvert main.cpp)

target_compile_definitions(libEchoLiveMsgAPIConvert PRIVATE BUILD_DLL)

target_link_libraries(libEchoLiveMsgAPIConvert PRIVATE nlohmann_json::nlohmann_json)
