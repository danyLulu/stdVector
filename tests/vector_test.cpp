#include <cstddef>
#include <stdexcept>

#include "gtest/gtest.h"

#include "vector.h"

TEST(MyVectorTest, DefaultConstructorCreatesEmptyVector) {
  MyVector<int> vector;

  EXPECT_TRUE(vector.empty());
  EXPECT_EQ(vector.getSize(), 0U);
  EXPECT_EQ(vector.getCapacity(), 0U);
  EXPECT_EQ(vector.getData(), nullptr);
}

TEST(MyVectorTest, SizeConstructorCreatesValueInitializedElements) {
  const MyVector<int> vector(3);

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.getSize(), 3U);
  EXPECT_EQ(vector.getCapacity(), 3U);
  EXPECT_EQ(vector[0], 0);
  EXPECT_EQ(vector[1], 0);
  EXPECT_EQ(vector[2], 0);
}

TEST(MyVectorTest, PushBackAppendsElementsAndGrowsCapacity) {
  MyVector<int> vector;

  vector.push_back(10);
  vector.push_back(20);
  vector.push_back(30);

  ASSERT_EQ(vector.getSize(), 3U);
  EXPECT_GE(vector.getCapacity(), vector.getSize());
  EXPECT_EQ(vector[0], 10);
  EXPECT_EQ(vector[1], 20);
  EXPECT_EQ(vector[2], 30);
}

TEST(MyVectorTest, PopBackRemovesLastElement) {
  MyVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);

  vector.pop_back();
  EXPECT_EQ(vector.getSize(), 1U);
  EXPECT_EQ(vector.back(), 1);

  vector.pop_back();
  vector.pop_back();
  EXPECT_TRUE(vector.empty());
}

TEST(MyVectorTest, AtReturnsElementAndChecksBounds) {
  MyVector<int> vector;
  vector.push_back(42);

  EXPECT_EQ(vector.at(0), 42);
  EXPECT_THROW(vector.at(1), std::out_of_range);
}

TEST(MyVectorTest, FrontAndBackCheckEmptyVector) {
  MyVector<int> vector;

  EXPECT_THROW(vector.front(), std::out_of_range);
  EXPECT_THROW(vector.back(), std::out_of_range);

  vector.push_back(7);
  EXPECT_EQ(vector.front(), 7);
  EXPECT_EQ(vector.back(), 7);
}

TEST(MyVectorTest, ReserveOnlyIncreasesCapacity) {
  MyVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);
  const std::size_t old_capacity = vector.getCapacity();

  vector.reserve(old_capacity + 5);
  EXPECT_EQ(vector.getCapacity(), old_capacity + 5);
  EXPECT_EQ(vector.getSize(), 2U);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);

  vector.reserve(1);
  EXPECT_EQ(vector.getCapacity(), old_capacity + 5);
}

TEST(MyVectorTest, ResizeChangesSizeAndInitializesNewElements) {
  MyVector<int> vector;
  vector.push_back(5);

  vector.resize(4);
  EXPECT_EQ(vector.getSize(), 4U);
  EXPECT_EQ(vector[0], 5);
  EXPECT_EQ(vector[1], 0);
  EXPECT_EQ(vector[2], 0);
  EXPECT_EQ(vector[3], 0);

  vector.resize(1);
  EXPECT_EQ(vector.getSize(), 1U);
  EXPECT_EQ(vector[0], 5);
}

TEST(MyVectorTest, ClearKeepsCapacityAndRemovesElements) {
  MyVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);
  const std::size_t capacity = vector.getCapacity();

  vector.clear();

  EXPECT_TRUE(vector.empty());
  EXPECT_EQ(vector.getSize(), 0U);
  EXPECT_EQ(vector.getCapacity(), capacity);
}

TEST(MyVectorTest, ShrinkToFitMatchesCapacityToSize) {
  MyVector<int> vector;
  vector.reserve(10);
  vector.push_back(1);
  vector.push_back(2);

  vector.shrink_to_fit();

  EXPECT_EQ(vector.getCapacity(), vector.getSize());
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
}

TEST(MyVectorTest, InsertAddsElementAtRequestedPosition) {
  MyVector<int> vector;
  vector.push_back(1);
  vector.push_back(3);

  vector.insert(1, 2);

  ASSERT_EQ(vector.getSize(), 3U);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
  EXPECT_EQ(vector[2], 3);
}

TEST(MyVectorTest, InsertWorksForEmptyVector) {
  MyVector<int> vector;

  vector.insert(0, 99);

  ASSERT_EQ(vector.getSize(), 1U);
  EXPECT_EQ(vector.front(), 99);
}

TEST(MyVectorTest, InsertChecksIndex) {
  MyVector<int> vector;
  vector.push_back(1);

  EXPECT_THROW(vector.insert(2, 3), std::out_of_range);
}

TEST(MyVectorTest, CopyConstructorCreatesIndependentCopy) {
  MyVector<int> original;
  original.push_back(1);
  original.push_back(2);

  MyVector<int> copy(original);
  copy[0] = 10;

  EXPECT_EQ(original[0], 1);
  EXPECT_EQ(copy[0], 10);
  EXPECT_EQ(copy[1], original[1]);
}

TEST(MyVectorTest, AssignmentCreatesIndependentCopy) {
  MyVector<int> original;
  original.push_back(1);
  original.push_back(2);
  MyVector<int> assigned;

  assigned = original;
  assigned[1] = 20;

  EXPECT_EQ(original[1], 2);
  EXPECT_EQ(assigned[1], 20);
}

TEST(MyVectorTest, SelfAssignmentKeepsVectorUnchanged) {
  MyVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);

  vector = vector;

  EXPECT_EQ(vector.getSize(), 2U);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
}
