find_package(GTest REQUIRED)

add_executable(field_test field_test.cpp)
target_link_libraries(field_test PUBLIC kie_toolbox gtest::gtest)
target_compile_options(field_test PRIVATE -fsanitize=address -g --coverage)
target_link_options(field_test PRIVATE -fsanitize=address --coverage)
add_test(field_test field_test)


add_executable(from_json_test from_json_test.cpp)
target_link_libraries(from_json_test PUBLIC kie_toolbox gtest::gtest)
target_compile_options(from_json_test PRIVATE -fsanitize=address -g --coverage)
target_link_options(from_json_test PRIVATE -fsanitize=address --coverage)
add_test(from_json_test from_json_test)


add_executable(to_json_test to_json_test.cpp)
target_link_libraries(to_json_test PUBLIC kie_toolbox gtest::gtest)
target_compile_options(to_json_test PRIVATE -fsanitize=address -g --coverage)
target_link_options(to_json_test PRIVATE -fsanitize=address --coverage)
add_test(to_json_test to_json_test)
