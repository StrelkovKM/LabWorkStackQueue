#include <gtest.h>
#include <fstream>
#include "TStack.h"

// Предполагаем, что TString имеет конструктор от const char*
// Если нет, нужно будет адаптировать тесты

class TStackTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Создаем тестовый файл
    std::ofstream file("test_stack.txt");
    file << "5 3\n1 2 3";
    file.close();
  }

  void TearDown() override {
    // Удаляем тестовый файл
    remove("test_stack.txt");
    remove("output_stack.txt");
  }
};

// Тест конструктора по умолчанию
TEST_F(TStackTest, DefaultConstructor) {
  TStack<int> stack;
  EXPECT_EQ(stack.GetSize(), 0);
  EXPECT_TRUE(stack.IsEmpty());
  EXPECT_FALSE(stack.IsFull());
}

// Тест конструктора с initializer_list
TEST_F(TStackTest, InitializerListConstructor) {
  TStack<int> stack({ 1, 2, 3 }, 5);
  EXPECT_EQ(stack.GetSize(), 3);
  EXPECT_FALSE(stack.IsEmpty());
  EXPECT_FALSE(stack.IsFull());
}

// Тест конструктора копирования
TEST_F(TStackTest, CopyConstructor) {
  TStack<int> stack1({ 1, 2, 3 }, 5);
  TStack<int> stack2(stack1);

  EXPECT_EQ(stack1.GetSize(), stack2.GetSize());
  EXPECT_EQ(stack1[0], stack2[0]);
  EXPECT_EQ(stack1[1], stack2[1]);
  EXPECT_EQ(stack1[2], stack2[2]);
}

// Тест конструктора перемещения
TEST_F(TStackTest, MoveConstructor) {
  TStack<int> stack1({ 1, 2, 3 }, 5);
  TStack<int> stack2 (std::move(stack1));

  EXPECT_EQ(stack2.GetSize(), 3);
  EXPECT_EQ(stack2[0], 1);
  EXPECT_EQ(stack2[1], 2);
  EXPECT_EQ(stack2[2], 3);
  EXPECT_EQ(stack1.GetSize(), 0); // После перемещения исходный объект пуст
}

// Тест конструктора из файла
TEST_F(TStackTest, FileConstructor) {
  TStack<int> stack("test_stack.txt");

  EXPECT_EQ(stack.GetSize(), 3);
  EXPECT_EQ(stack[0], 1);
  EXPECT_EQ(stack[1], 2);
  EXPECT_EQ(stack[2], 3);
}

// Тест методов Put и Get
TEST_F(TStackTest, PutAndGet) {
  TStack<int> stack({}, 3);

  stack.Put(10);
  stack.Put(20);
  stack.Put(30);

  EXPECT_EQ(stack.GetSize(), 3);
  EXPECT_TRUE(stack.IsFull());

  EXPECT_EQ(stack.Get(), 30);
  EXPECT_EQ(stack.Get(), 20);
  EXPECT_EQ(stack.Get(), 10);
  EXPECT_TRUE(stack.IsEmpty());
}

// Тест оператора присваивания
TEST_F(TStackTest, AssignmentOperator) {
  TStack<int> stack1({ 1, 2, 3 }, 5);
  TStack<int> stack2;
  stack2 = stack1;

  EXPECT_EQ(stack1.GetSize(), stack2.GetSize());
  EXPECT_EQ(stack1[0], stack2[0]);
  EXPECT_EQ(stack1[1], stack2[1]);
  EXPECT_EQ(stack1[2], stack2[2]);
}

// Тест оператора перемещающего присваивания
TEST_F(TStackTest, MoveAssignmentOperator) {
  TStack<int> stack1({ 1, 2, 3 }, 5);
  TStack<int> stack2;
  stack2 = std::move(stack1);

  EXPECT_EQ(stack2.GetSize(), 3);
  EXPECT_EQ(stack2[0], 1);
  EXPECT_EQ(stack2[1], 2);
  EXPECT_EQ(stack2[2], 3);
  EXPECT_EQ(stack1.GetSize(), 0);
}

// Тест операторов сравнения
TEST_F(TStackTest, ComparisonOperators) {
  TStack<int> stack1({ 1, 2, 3 }, 5);
  TStack<int> stack2({ 1, 2, 3 }, 5);
  TStack<int> stack3({ 4, 5, 6 }, 5);

  EXPECT_TRUE(stack1 == stack2);
  EXPECT_FALSE(stack1 == stack3);
  EXPECT_TRUE(stack1 != stack3);
  EXPECT_FALSE(stack1 != stack2);
}

// Тест метода SaveToFile
TEST_F(TStackTest, SaveToFile) {
  TStack<int> stack({ 7, 8, 9 }, 5);
  stack.SaveToFile("output_stack.txt");

  // Проверяем, что файл создан и содержит правильные данные
  std::ifstream file("output_stack.txt");
  size_t capacity, size;
  file >> capacity >> size;

  EXPECT_EQ(capacity, 5);
  EXPECT_EQ(size, 3);

  int val1, val2, val3;
  file >> val1 >> val2 >> val3;

  EXPECT_EQ(val1, 7);
  EXPECT_EQ(val2, 8);
  EXPECT_EQ(val3, 9);

  file.close();
}

// Тест метода FindMin
TEST_F(TStackTest, FindMin) {
  TStack<int> stack({ 5, 2, 8, 1, 9 }, 10);

  EXPECT_EQ(stack.FindMin(), 1);
}

// Тест FindMin с одним элементом
TEST_F(TStackTest, FindMinSingleElement) {
  TStack<int> stack({ 42 }, 5);

  EXPECT_EQ(stack.FindMin(), 42);
}

// Тест итераторов
TEST_F(TStackTest, Iterators) {
  TStack<int> stack({ 1, 2, 3 }, 5);

  int sum = 0;
  for (auto it = stack.begin(); it != stack.end(); ++it) {
    sum += *it;
  }
  EXPECT_EQ(sum, 6);

  // Range-based for loop
  sum = 0;
  for (const auto& item : stack) {
    sum += item;
  }
  EXPECT_EQ(sum, 6);
}

// Тест исключений
TEST_F(TStackTest, Exceptions) {
  TStack<int> empty_stack;

  // Попытка Get из пустого стека
  EXPECT_THROW(empty_stack.Get(), TError);

  // Попытка Put в полный стек
  TStack<int> full_stack({ 1, 2 }, 2);
  EXPECT_THROW(full_stack.Put(3), TError);

  // Попытка доступа по неверному индексу
  TStack<int> stack({ 1, 2 }, 3);
  EXPECT_THROW(stack[5], TError);

  // Попытка FindMin в пустом стеке
  EXPECT_THROW(empty_stack.FindMin(), TError);
}
