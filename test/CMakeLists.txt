add_executable(test_echoLiveMsgAPIConvert test_msgAPI.cpp)

find_package(GTest REQUIRED)
target_link_libraries(test_echoLiveMsgAPIConvert GTest::GTest GTest::Main libEchoLiveMsgAPIConvert)
add_dependencies(test_echoLiveMsgAPIConvert libEchoLiveMsgAPIConvert)


add_test(NAME test_msgAPI COMMAND test_msgAPI)