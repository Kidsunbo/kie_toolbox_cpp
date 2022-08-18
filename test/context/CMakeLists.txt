add_executable(context_test context_test.cpp)
target_link_libraries(context_test PUBLIC kie_toolbox)
target_compile_options(context_test PRIVATE -fsanitize=address -g --coverage -Wall -Wextra -Werror)
target_link_options(context_test PRIVATE -fsanitize=address --coverage)

add_test(context_test context_test)
