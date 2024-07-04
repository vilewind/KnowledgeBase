#include <gtest/gtest.h>
#include <iostream>

TEST(Lambda, LambdaTest) {
  int x = 0;
  int y = 0;

  /*
   * @note 未捕获值
  auto nohunt = []() { std::cout << x << " " << y << std::endl; };

  nohunt();
  */

  auto huntWithValue = [x]() {
    /* @note 值捕获具有const性
     * x = 10;
     * */
  };
  huntWithValue();
  EXPECT_NE(x, 10);

  auto huntWithAllValue = [=]() {
    /*
    @note 值捕获具有const性
      x = 10;
      y = 10;
    */
  };
  huntWithAllValue();
  EXPECT_NE(x, 10);
  EXPECT_NE(y, 10);

  auto huntWithRef = [&x]() { x = 10; };
  huntWithRef();
  EXPECT_EQ(x, 10);

  auto huntWithAllRef = [&]() {
    x = 100;
    y = 100;
  };
  huntWithAllRef();
  EXPECT_EQ(x, 100);
  EXPECT_EQ(y, 100);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
