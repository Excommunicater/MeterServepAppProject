#include <gtest/gtest.h>
extern "C" {
    #include "../ServerUtils/messageUtils.h"
}

// Mocking functions



TEST(PushMessageToQueue, FirstEmptyTest)
{
    GetMessageSize(1);
    EXPECT_TRUE(true);
}