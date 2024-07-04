#include <chrono>
#include <gtest/gtest.h>
#include <string>
#include <thread>

#include "unbounded_queue.h"

TEST(UnboundedQueueTest, ConsumerProducer) {
  UnboundedQueue<int> que;

  std::atomic<int> input_sum = 0;
  std::vector<std::thread> producers;
  const static int kProducerCnt = 3;
  const static int kSteps = 3;
  for (int i = 1; i <= kProducerCnt; ++i) {
    producers.emplace_back([&, i]() {
      for (int j = 1; j <= kSteps; ++j) {
        /// @note 连续的 <<操作符是非线程安全的
        std::cout << std::string("enquue ") + std::to_string(i * j) +
                         std::string("\n");
        que.Enqueue(i * j);
        input_sum.fetch_add(i * j);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
      }
      std::cout << std::endl;
    });
  }

  int sum = 0;
  std::thread consumer([&]() {
    int i = 0;
    while (i < kProducerCnt * kSteps) {
      int value = 0;
      if (!que.Dequeue(&value)) {
        continue;
      }
      std::cout << "dequeue " << value << std::endl;
      sum += value;
      ++i;
    }
  });

  for (int i = 0; i < kProducerCnt; ++i) {
    producers[i].join();
  }
  consumer.join();

  const static int kSum = 100;
  EXPECT_EQ(sum, input_sum.load());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
