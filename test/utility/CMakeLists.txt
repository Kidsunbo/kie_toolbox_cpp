find_package(GTest REQUIRED)

add_executable(defer_test defer_test.cpp)
target_link_libraries(defer_test PUBLIC kie_toolbox gtest::gtest)
# target_compile_options(defer_test PRIVATE -fsanitize=address -g --coverage -Wall -Wextra -Werror -Wno-missing-field-initializers)
# target_link_options(defer_test PRIVATE -fsanitize=address --coverage)
add_test(defer_test defer_test)