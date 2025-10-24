#pragma once
#include <gtest.h>
#include "TMultiStack.h"

class TMultiStackTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Setup code before each test
  }

  void TearDown() override {
    // Cleanup code after each test
  }

  // Helper methods for tests
  void FillStack(TMultiStack<int>& stack, size_t stackIndex, const std::vector<int>& values) {
    for (int value : values) {
      stack.Put_M(stackIndex, value);
    }
  }

  std::vector<int> DrainStack(TMultiStack<int>& stack, size_t stackIndex, size_t count) {
    std::vector<int> result;
    for (size_t i = 0; i < count; ++i) {
      if (!stack.IsEmptyStack_M(stackIndex)) {
        result.push_back(stack.Get_M(stackIndex));
      }
    }
    return result;
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

// Test LIFO behavior for each stack
TEST_F(TMultiStackTest, LIFOBehavior) {
  TMultiStack<int> stack(6, 2);

  // Fill stack 0
  stack.Put_M(0, 1);
  stack.Put_M(0, 2);
  stack.Put_M(0, 3);

  // Fill stack 1
  stack.Put_M(1, 4);
  stack.Put_M(1, 5);

  // Verify LIFO order
  EXPECT_EQ(stack.Get_M(0), 3);
  EXPECT_EQ(stack.Get_M(0), 2);
  EXPECT_EQ(stack.Get_M(1), 5);
  EXPECT_EQ(stack.Get_M(1), 4);
  EXPECT_EQ(stack.Get_M(0), 1);
}

// Test IsFull and IsEmpty for individual stacks
TEST_F(TMultiStackTest, StackStatus) {
  TMultiStack<int> stack(6, 2); // 2 stacks with total capacity 6

  EXPECT_TRUE(stack.IsEmptyStack_M(0));
  EXPECT_TRUE(stack.IsEmptyStack_M(1));
  EXPECT_FALSE(stack.IsFullStack_M(0));
  EXPECT_FALSE(stack.IsFullStack_M(1));

  // Fill stack 0 completely (assuming it gets 3 capacity)
  stack.Put_M(0, 1);
  stack.Put_M(0, 2);
  stack.Put_M(0, 3);

  EXPECT_TRUE(stack.IsFullStack_M(0));
  EXPECT_FALSE(stack.IsEmptyStack_M(0));

  // Partially fill stack 1
  stack.Put_M(1, 4);
  EXPECT_FALSE(stack.IsFullStack_M(1));
  EXPECT_FALSE(stack.IsEmptyStack_M(1));
}

// Test full multistack detection
TEST_F(TMultiStackTest, FullMultiStack) {
  TMultiStack<int> stack(6, 2);

  // Fill both stacks completely
  FillStack(stack, 0, { 1, 2, 3 });
  FillStack(stack, 1, { 4, 5, 6 });

  EXPECT_TRUE(stack.IsFull_M());
  EXPECT_FALSE(stack.IsEmpty_M());

  // Try to add to full stack - should throw
  EXPECT_THROW(stack.Put_M(0, 7), TError);
}

// Test empty multistack detection
TEST_F(TMultiStackTest, EmptyMultiStack) {
  TMultiStack<int> stack(6, 2);

  EXPECT_TRUE(stack.IsEmpty_M());

  // Try to get from empty stack - should throw
  EXPECT_THROW(stack.Get_M(0), TError);

  // Add and remove elements
  stack.Put_M(0, 1);
  stack.Get_M(0);

  EXPECT_TRUE(stack.IsEmpty_M());
}

// Test PushStack_M functionality
TEST_F(TMultiStackTest, PushNewStack) {
  TMultiStack<int> stack(9, 2); // Start with 2 stacks

  // Fill some data
  FillStack(stack, 0, { 1, 2 });
  FillStack(stack, 1, { 3, 4 });

  size_t initialSize = stack.GetSize_M();
  size_t initialStacks = stack.GetCountStacks();

  // Add new stack
  stack.PushStack_M(1, 2); // Add stack at position 1 with capacity 2

  EXPECT_EQ(stack.GetCountStacks(), initialStacks + 1);
  EXPECT_EQ(stack.GetSize_M(), initialSize); // Size should remain the same

  // Should be able to use new stack
  stack.Put_M(1, 5); // Put in new stack at position 1
  EXPECT_EQ(stack.GetSize_M(), initialSize + 1);
}

// Test PopStack_M functionality
TEST_F(TMultiStackTest, PopStack) {
  TMultiStack<int> stack(9, 3);

  // Fill some data
  FillStack(stack, 0, { 1 });
  FillStack(stack, 1, { 2, 3 });
  FillStack(stack, 2, { 4 });

  size_t initialSize = stack.GetSize_M();
  size_t stack1Size = 2; // We put 2 elements in stack 1

  // Remove stack at position 1
  stack.PopStack_M(1);

  EXPECT_EQ(stack.GetCountStacks(), 2);
  // Total size should decrease by the size of removed stack
  EXPECT_EQ(stack.GetSize_M(), initialSize - stack1Size);
}

// Test Repack functionality (indirectly through filling)
TEST_F(TMultiStackTest, RepackFunctionality) {
  TMultiStack<int> stack(9, 3); // 3 stacks

  // Fill stacks unevenly to trigger repack
  FillStack(stack, 0, { 1, 2, 3 }); // Fill stack 0 completely
  FillStack(stack, 2, { 4, 5, 6 }); // Fill stack 2 completely
  // Stack 1 remains empty

  // Now try to add to stack 0 - should trigger repack
  // This test might need adjustment based on actual repack logic
  EXPECT_NO_THROW(stack.Put_M(0, 7)); // Should trigger repack and succeed
}

// Test copy constructor
TEST_F(TMultiStackTest, CopyConstructor) {
  TMultiStack<int> original(6, 2);
  FillStack(original, 0, { 1, 2 });
  FillStack(original, 1, { 3 });

  TMultiStack<int> copy(original);

  EXPECT_EQ(original.GetSize_M(), copy.GetSize_M());
  EXPECT_EQ(original.GetCountStacks(), copy.GetCountStacks());

  // Verify data integrity
  EXPECT_EQ(original.Get_M(0), copy.Get_M(0));
  EXPECT_EQ(original.Get_M(0), copy.Get_M(0));
  EXPECT_EQ(original.Get_M(1), copy.Get_M(1));
}


// Test error conditions
TEST_F(TMultiStackTest, ErrorConditions) {
  TMultiStack<int> stack(3, 1);

  // Test out of bounds stack access
  EXPECT_THROW(stack.Put_M(5, 1), TError); // Invalid stack index
  EXPECT_THROW(stack.Get_M(5), TError);    // Invalid stack index

  // Test overflow
  FillStack(stack, 0, { 1, 2, 3 }); // Fill to capacity
  EXPECT_THROW(stack.Put_M(0, 4), TError); // Overflow

  // Test underflow
  DrainStack(stack, 0, 3); // Empty the stack
  EXPECT_THROW(stack.Get_M(0), TError); // Underflow
}

// Test SayStatusStack (mostly for coverage, hard to verify output)
TEST_F(TMultiStackTest, SayStatusStack) {
  TMultiStack<int> stack(3, 1);

  // Should not crash
  EXPECT_NO_THROW(stack.SayStatusStack());

  stack.Put_M(0, 1);
  EXPECT_NO_THROW(stack.SayStatusStack());

  FillStack(stack, 0, { 2, 3 });
  EXPECT_NO_THROW(stack.SayStatusStack());
}