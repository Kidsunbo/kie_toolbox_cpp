find_package(GTest REQUIRED)

add_executable(serde_test serde_test.cpp)
target_link_libraries(serde_test PUBLIC kie_toolbox gtest::gtest)
target_compile_options(serde_test PRIVATE -fsanitize=address -g --coverage)
target_link_options(serde_test PRIVATE -fsanitize=address --coverage)

add_test(serde_test serde_test)
