#include <gtest/gtest.h>
extern "C" {
    #include "../ServerUtils/serverUtils.h"
}

TEST(ExampleTest, FirstEmptyTest)
{
    GetMessageSize(1);
    EXPECT_TRUE(true);
}