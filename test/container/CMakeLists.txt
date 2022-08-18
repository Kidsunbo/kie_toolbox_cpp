find_package(GTest REQUIRED)

add_executable(tuple_test tuple_test.cpp)
target_link_libraries(tuple_test PUBLIC kie_toolbox gtest::gtest)
target_compile_options(tuple_test PRIVATE -fsanitize=address -g --coverage -Wall -Wextra -Werror)
target_link_options(tuple_test PRIVATE -fsanitize=address --coverage)

add_test(tuple_test tuple_test)
