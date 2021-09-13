#include <gtest/gtest.h>
#include <numeric>
#include <vector>

TEST(MyTest, Sum) {
    std::vector<int> vec{1, 3, 5};
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    EXPECT_EQ(sum, 9);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
