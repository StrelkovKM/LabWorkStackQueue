#include <gtest.h>
#include "TMultiStack.h"

class TMultiStackTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Setup code if needed
  }

  void TearDown() override {
    // Cleanup code if needed
  }
};

// Test default constructor
TEST_F(TMultiStackTest, DefaultConstructor) {
  TMultiStack<int> stack;
  EXPECT_EQ(stack.GetSize_M(), 0);
  EXPECT_EQ(stack.GetCapacity_M(), 0);
  EXPECT_EQ(stack.GetCountStacks(), 0);
  EXPECT_TRUE(stack.IsEmpty_M());
}

// Test parameterized constructor
TEST_F(TMultiStackTest, ParameterizedConstructor) {
  TMultiStack<int> stack(100, 4);
  EXPECT_EQ(stack.GetSize_M(), 0);
  EXPECT_EQ(stack.GetCapacity_M(), 100);
  EXPECT_EQ(stack.GetCountStacks(), 4);
  EXPECT_TRUE(stack.IsEmpty_M());
}

// Test copy constructor
TEST_F(TMultiStackTest, CopyConstructor) {
  TMultiStack<int> original(100, 3);
  original.Put_M(0, 10);
  original.Put_M(1, 20);

  TMultiStack<int> copy(original);
  EXPECT_EQ(copy.GetSize_M(), 2);
  EXPECT_EQ(copy.GetCapacity_M(), 100);
  EXPECT_EQ(copy.GetCountStacks(), 3);
}

// Test move constructor
TEST_F(TMultiStackTest, MoveConstructor) {
  TMultiStack<int> original(100, 2);
  original.Put_M(0, 5);
  original.Put_M(1, 15);

  TMultiStack<int> moved(std::move(original));
  EXPECT_EQ(moved.GetSize_M(), 2);
  EXPECT_EQ(moved.GetCapacity_M(), 100);
  EXPECT_EQ(moved.GetCountStacks(), 2);
  EXPECT_EQ(original.GetSize_M(), 0); // NOLINT
}

// Test Put and Get operations
TEST_F(TMultiStackTest, PutAndGet) {
  TMultiStack<int> stack(50, 2);

  stack.Put_M(0, 100);
  stack.Put_M(1, 200);

  EXPECT_EQ(stack.GetSize_M(), 2);
  EXPECT_FALSE(stack.IsEmptyStack_M(0));
  EXPECT_FALSE(stack.IsEmptyStack_M(1));

  int val1 = stack.Get_M(0);
  int val2 = stack.Get_M(1);

  EXPECT_EQ(val1, 100);
  EXPECT_EQ(val2, 200);
  EXPECT_EQ(stack.GetSize_M(), 0);
}

// Test IsFull and IsEmpty
TEST_F(TMultiStackTest, IsFullIsEmpty) {
  TMultiStack<int> stack(10, 2);

  EXPECT_TRUE(stack.IsEmpty_M());
  EXPECT_FALSE(stack.IsFull_M());

  // Fill the stacks
  for (int i = 0; i < 5; i++) {
    stack.Put_M(0, i);
    stack.Put_M(1, i + 5);
  }

  EXPECT_TRUE(stack.IsFull_M());
  EXPECT_FALSE(stack.IsEmpty_M());
}

// Test stack operations (PushStack_M and PopStack_M)
TEST_F(TMultiStackTest, StackOperations) {
  TMultiStack<int> stack(30, 2);

  stack.PushStack_M(1, 10); // Add new stack at position 1 with capacity 10
  EXPECT_EQ(stack.GetCountStacks(), 3);
  EXPECT_EQ(stack.GetCapacity_M(), 30);

  stack.PopStack_M(1); // Remove the stack at position 1
  EXPECT_EQ(stack.GetCountStacks(), 2);
}

// Test repack functionality
TEST_F(TMultiStackTest, RepackTest) {
  TMultiStack<int> stack(20, 2);

  // Fill first stack almost completely
  for (int i = 0; i < 9; i++) {
    stack.Put_M(0, i);
  }

  // Put one element in second stack
  stack.Put_M(1, 100);

  EXPECT_FALSE(stack.IsFullStack_M(0));
  EXPECT_FALSE(stack.IsFullStack_M(1));

  // This should trigger repack
  stack.Put_M(0, 999);

  EXPECT_EQ(stack.GetSize_M(), 11);
}

// Test comparison operators
TEST_F(TMultiStackTest, ComparisonOperators) {
  TMultiStack<int> stack1(50, 3);
  TMultiStack<int> stack2(50, 3);
  TMultiStack<int> stack3(100, 2);

  stack1.Put_M(0, 10);
  stack1.Put_M(1, 20);
  stack2.Put_M(0, 10);
  stack2.Put_M(1, 20);

  EXPECT_TRUE(stack1 == stack2);
  EXPECT_FALSE(stack1 != stack2);
  EXPECT_TRUE(stack1 != stack3);
}

// Test exception handling
TEST_F(TMultiStackTest, ExceptionHandling) {
  TMultiStack<int> stack(10, 2);

  // Test Get from empty stack
  EXPECT_THROW(stack.Get_M(0), TError);

  // Fill stack to capacity
  for (int i = 0; i < 5; i++) {
    stack.Put_M(0, i);
    stack.Put_M(1, i);
  }

  // Test Put to full stack
  EXPECT_THROW(stack.Put_M(0, 100), TError);
}

// Test output operator
TEST_F(TMultiStackTest, OutputOperator) {
  TMultiStack<int> stack(20, 2);
  stack.Put_M(0, 1);
  stack.Put_M(1, 2);

  std::stringstream ss;
  ss << stack;

  // The exact format depends on TStack's output operator implementation
  EXPECT_FALSE(ss.str().empty());
}

// Test edge cases
TEST_F(TMultiStackTest, EdgeCases) {
  // Test with zero capacity
  TMultiStack<int> stack1(0, 0);
  EXPECT_TRUE(stack1.IsEmpty_M());
  EXPECT_EQ(stack1.GetCapacity_M(), 0);

  TMultiStack<int> stack2(10, 1);
  stack2.Put_M(0, 42);
  EXPECT_EQ(stack2.GetSize_M(), 1);
  int m = stack2.Get_M(0);
  EXPECT_EQ(m, 42);
}

// Test with different data types
TEST_F(TMultiStackTest, DifferentDataTypes) {
  TMultiStack<double> doubleStack(20, 2);
  doubleStack.Put_M(0, 3.14);
  doubleStack.Put_M(1, 2.71);

  EXPECT_EQ(doubleStack.GetSize_M(), 2);
  EXPECT_DOUBLE_EQ(doubleStack.Get_M(0), 3.14);
  EXPECT_DOUBLE_EQ(doubleStack.Get_M(1), 2.71);

  TMultiStack<std::string> stringStack(30, 2);
  stringStack.Put_M(0, "hello");
  stringStack.Put_M(1, "world");

  EXPECT_EQ(stringStack.GetSize_M(), 2);
  EXPECT_EQ(stringStack.Get_M(0), "hello");
  EXPECT_EQ(stringStack.Get_M(1), "world");
}
