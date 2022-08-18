#include <container/tuple.hpp>
#include <iostream>
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(TupleTest, StructComplex)
{
    std::tuple t = std::make_tuple(1, 2.0, 3.0);
    double sum = 0;
    kie::container::loop_tuple(t, [&sum](auto &value){
        sum += value;
        std::cout<<value<<" ";
    });
    std::cout<<std::endl;
    ASSERT_EQ(sum, 6.0);

    kie::container::loop_tuple_reverse(t, [&sum](auto &value){
        sum += value;
        std::cout<<value<<" "; 
    });
    std::cout<<std::endl;
    ASSERT_EQ(sum, 12.0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}