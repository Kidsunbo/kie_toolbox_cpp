#include <utility/defer.hpp>
#include <gtest/gtest.h>


// Demonstrate some basic assertions.
TEST(Utility, Defer)
{
  using namespace kie::utility;

  defer [](){
    std::cout<<"End is called"<<std::endl;
  };

  std::cout<<"hello world"<<std::endl;
}

// Demonstrate some basic assertions.
TEST(Utility, TryDefer)
{
  using namespace kie::utility;

  try_defer [](){
    std::cout<<"End is called"<<std::endl;
    throw std::runtime_error("error occurs");
    std::cout<<"Unreachable"<<std::endl;
  };

  std::cout<<"hello world"<<std::endl;
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}