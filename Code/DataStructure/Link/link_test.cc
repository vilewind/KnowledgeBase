#include "link.h"
#include "gtest/gtest.h"
#include <utility>

class LinkTest : public ::testing::Test {
protected:
  LinkTest() {
    link.AddNode(1);
    link.AddNode(2);
    link.AddNode(3);
    link.AddNode(4);
  }

  void SetUp() override {
    //
  }

  void TearDown() override {
    //
  }

  Link<int> link;
};

TEST_F(LinkTest, IsEmpty) { EXPECT_FALSE(link.IsEmpty()); }

TEST_F(LinkTest, LinkFunction) {
  int value = 0;
  EXPECT_TRUE(link.GetValue(&value));
  link.Iterate();
  link.Reverse();
  link.Iterate();
  link.CrusionReverse();
  link.Iterate();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
