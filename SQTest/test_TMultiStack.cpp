#include <gtest.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "TMultiStack.h"

class TMultiStackTest : public ::testing::Test {
protected:
    void SetUp() override {
        stack3x5 = TMultiStack<int>(3, 5);
        stack2x3 = TMultiStack<int>(2, 3);
    }

    void TearDown() override {
    }

    TMultiStack<int> stack3x5;
    TMultiStack<int> stack2x3;
};

// Тест конструкторов
TEST_F(TMultiStackTest, Constructors) {
    // Конструктор по умолчанию
    TMultiStack<int> emptyStack;
    EXPECT_EQ(emptyStack.GetCountStacks(), 0);
    EXPECT_EQ(emptyStack.GetCapacity_M(), 0);
    EXPECT_EQ(emptyStack.GetSize_M(), 0);

    // Конструктор с параметрами
    EXPECT_EQ(stack3x5.GetCountStacks(), 3);
    EXPECT_EQ(stack3x5.GetCapacity_M(), 15);
    EXPECT_EQ(stack3x5.GetSize_M(), 0);
    EXPECT_TRUE(stack3x5.IsEmpty(0));
    EXPECT_TRUE(stack3x5.IsEmpty(1));
    EXPECT_TRUE(stack3x5.IsEmpty(2));

    // Конструктор копирования
    stack3x5.Push(0, 10);
    stack3x5.Push(1, 20);
    TMultiStack<int> copyStack(stack3x5);
    EXPECT_EQ(copyStack.GetCountStacks(), 3);
    EXPECT_EQ(copyStack.GetCapacity_M(), 15);
    EXPECT_EQ(copyStack.GetSizeOfStack(0), 1);
    EXPECT_EQ(copyStack.GetSizeOfStack(1), 1);
    EXPECT_EQ(copyStack(0, 0), 10);
    EXPECT_EQ(copyStack(1, 0), 20);
}

// Тест конструктора из файла
TEST_F(TMultiStackTest, FileConstructor) {
    const std::string filename = "constructor_test.bin";
    
    // Создаем и заполняем стек
    TMultiStack<int> originalStack(2, 3);
    originalStack.Push(0, 100);
    originalStack.Push(0, 200);
    originalStack.Push(1, 300);
    originalStack.SaveToFile(filename);

    // Создаем стек из файла
    TMultiStack<int> fileStack(filename);
    
    EXPECT_EQ(fileStack.GetCountStacks(), 2);
    EXPECT_EQ(fileStack.GetCapacity_M(), 6);
    EXPECT_EQ(fileStack.GetSizeOfStack(0), 2);
    EXPECT_EQ(fileStack.GetSizeOfStack(1), 1);
    EXPECT_EQ(fileStack.GetSize_M(), 3);
    EXPECT_EQ(fileStack(0, 0), 100);
    EXPECT_EQ(fileStack(0, 1), 200);
    EXPECT_EQ(fileStack(1, 0), 300);

    // Удаляем тестовый файл
    std::remove(filename.c_str());
}

// Тест конструктора перемещения
TEST_F(TMultiStackTest, MoveConstructor) {
    stack3x5.Push(0, 10);
    stack3x5.Push(1, 20);
    
    TMultiStack<int> moveStack(std::move(stack3x5));
    
    EXPECT_EQ(moveStack.GetCountStacks(), 3);
    EXPECT_EQ(moveStack.GetCapacity_M(), 15);
    EXPECT_EQ(moveStack.GetSizeOfStack(0), 1);
    EXPECT_EQ(moveStack.GetSizeOfStack(1), 1);
    EXPECT_EQ(moveStack.GetSize_M(), 2);
    EXPECT_EQ(moveStack(0, 0), 10);
    EXPECT_EQ(moveStack(1, 0), 20);

    // Проверяем, что исходный объект обнулен
    EXPECT_EQ(stack3x5.GetCountStacks(), 0);
    EXPECT_EQ(stack3x5.GetCapacity_M(), 0);
    EXPECT_EQ(stack3x5.GetSize_M(), 0);
}

// Тест операторов присваивания
TEST_F(TMultiStackTest, AssignmentOperators) {
    // Оператор копирующего присваивания
    stack3x5.Push(0, 100);
    stack3x5.Push(1, 200);
    TMultiStack<int> assignedStack;
    assignedStack = stack3x5;
    EXPECT_EQ(assignedStack.GetCountStacks(), 3);
    EXPECT_EQ(assignedStack.GetSize_M(), 2);
    EXPECT_EQ(assignedStack(0, 0), 100);
    EXPECT_EQ(assignedStack(1, 0), 200);

    // Оператор перемещающего присваивания
    TMultiStack<int> moveAssignedStack;
    moveAssignedStack = std::move(stack3x5);
    EXPECT_EQ(moveAssignedStack.GetCountStacks(), 3);
    EXPECT_EQ(moveAssignedStack.GetSize_M(), 2);
    EXPECT_EQ(moveAssignedStack(0, 0), 100);
    EXPECT_EQ(moveAssignedStack(1, 0), 200);
}

// Тест операторов сравнения
TEST_F(TMultiStackTest, ComparisonOperators) {
    TMultiStack<int> stack1(2, 3);
    TMultiStack<int> stack2(2, 3);
    
    stack1.Push(0, 1);
    stack1.Push(1, 2);
    stack2.Push(0, 1);
    stack2.Push(1, 2);

    EXPECT_TRUE(stack1 == stack2);
    EXPECT_FALSE(stack1 != stack2);

    stack2.Push(0, 3);
    EXPECT_FALSE(stack1 == stack2);
    EXPECT_TRUE(stack1 != stack2);
}

// Тест основных операций Push/Pop
TEST_F(TMultiStackTest, PushPopOperations) {
    stack3x5.Push(0, 10);
    stack3x5.Push(0, 20);
    stack3x5.Push(1, 30);
    
    EXPECT_EQ(stack3x5.GetSizeOfStack(0), 2);
    EXPECT_EQ(stack3x5.GetSizeOfStack(1), 1);
    EXPECT_EQ(stack3x5.GetSizeOfStack(2), 0);
    EXPECT_EQ(stack3x5.GetSize_M(), 3);

    EXPECT_EQ(stack3x5.Pop(0), 20);
    EXPECT_EQ(stack3x5.Pop(0), 10);
    EXPECT_EQ(stack3x5.Pop(1), 30);
    
    EXPECT_EQ(stack3x5.GetSize_M(), 0);
}

// Тест проверки на пустоту и полноту для отдельных стеков
TEST_F(TMultiStackTest, IsEmptyIsFull) {
    EXPECT_TRUE(stack2x3.IsEmpty(0));
    EXPECT_TRUE(stack2x3.IsEmpty(1));
    EXPECT_FALSE(stack2x3.IsFull(0));
    EXPECT_FALSE(stack2x3.IsFull(1));

    // Заполняем стек
    for (int i = 0; i < 3; ++i) {
        stack2x3.Push(0, i);
    }
    
    EXPECT_FALSE(stack2x3.IsEmpty(0));
    EXPECT_TRUE(stack2x3.IsFull(0));
}

// Тест проверки на пустоту и полноту для всего мультистека
TEST_F(TMultiStackTest, IsEmptyIsFullMulti) {
    TMultiStack<int> stack(2, 2);
    
    EXPECT_TRUE(stack.IsEmpty_M());
    EXPECT_FALSE(stack.IsFull_M());

    // Заполняем частично
    stack.Push(0, 1);
    stack.Push(1, 2);
    EXPECT_FALSE(stack.IsEmpty_M());
    EXPECT_FALSE(stack.IsFull_M());

    // Заполняем полностью
    stack.Push(0, 3);
    stack.Push(1, 4);
    EXPECT_FALSE(stack.IsEmpty_M());
    EXPECT_TRUE(stack.IsFull_M());
}

// Тест оператора доступа
TEST_F(TMultiStackTest, AccessOperator) {
    stack3x5.Push(0, 10);
    stack3x5.Push(0, 20);
    stack3x5.Push(1, 30);

    EXPECT_EQ(stack3x5(0, 0), 10);
    EXPECT_EQ(stack3x5(0, 1), 20);
    EXPECT_EQ(stack3x5(1, 0), 30);
}

// Тест поиска минимального элемента
TEST_F(TMultiStackTest, FindMin) {
    // Пустой стек - должно бросить исключение
    EXPECT_THROW(stack3x5.FindMin(), TError);

    stack3x5.Push(0, 50);
    stack3x5.Push(0, 30);
    stack3x5.Push(1, 40);
    stack3x5.Push(1, 20);
    stack3x5.Push(2, 60);
    stack3x5.Push(2, 10);

    EXPECT_EQ(stack3x5.FindMin(), 10);
}

// Тест репака (перераспределения памяти)
TEST_F(TMultiStackTest, Repack) {
    TMultiStack<int> stack(3, 2);
    
    // Заполняем все стеки
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            stack.Push(i, i * 10 + j);
        }
    }

    // Все стеки полны
    EXPECT_TRUE(stack.IsFull(0));
    EXPECT_TRUE(stack.IsFull(1));
    EXPECT_TRUE(stack.IsFull(2));
    EXPECT_TRUE(stack.IsFull_M());

    EXPECT_THROW(stack.Push(1, 999), TError);
}

// Тест работы с файлами
TEST_F(TMultiStackTest, FileOperations) {
    const std::string filename = "test_stack.bin";
    
    // Заполняем стек данными
    stack3x5.Push(0, 100);
    stack3x5.Push(0, 200);
    stack3x5.Push(1, 300);
    stack3x5.Push(2, 400);

    // Сохраняем в файл
    EXPECT_NO_THROW(stack3x5.SaveToFile(filename));

    // Проверяем что файл создался
    std::ifstream testFile(filename, std::ios::binary);
    EXPECT_TRUE(testFile.is_open());
    if (testFile.is_open()) {
        testFile.close();
    }

    // Загружаем из файла
    TMultiStack<int> loadedStack(filename);
    
    // Проверяем загруженные данные
    EXPECT_EQ(loadedStack.GetCountStacks(), 3);
    EXPECT_EQ(loadedStack.GetCapacity_M(), 15);
    EXPECT_EQ(loadedStack.GetSizeOfStack(0), 2);
    EXPECT_EQ(loadedStack.GetSizeOfStack(1), 1);
    EXPECT_EQ(loadedStack.GetSizeOfStack(2), 1);
    EXPECT_EQ(loadedStack.GetSize_M(), 4);
    EXPECT_EQ(loadedStack(0, 0), 100);
    EXPECT_EQ(loadedStack(0, 1), 200);
    EXPECT_EQ(loadedStack(1, 0), 300);
    EXPECT_EQ(loadedStack(2, 0), 400);

    // Удаляем тестовый файл
    std::remove(filename.c_str());
}

// Тест обработки ошибок
TEST_F(TMultiStackTest, ErrorHandling) {
    // Попытка доступа к несуществующему стеку
    EXPECT_THROW(stack3x5.Push(5, 10), TError);
    EXPECT_THROW(stack3x5.Pop(5), TError);
    EXPECT_THROW(stack3x5.IsEmpty(5), TError);
    EXPECT_THROW(stack3x5.IsFull(5), TError);
    EXPECT_THROW(stack3x5.GetSizeOfStack(5), TError);
    EXPECT_THROW(stack3x5(5, 0), TError);

    // Попытка извлечь из пустого стека
    EXPECT_THROW(stack3x5.Pop(0), TError);

    // Попытка доступа к несуществующему индексу в стеке
    stack3x5.Push(0, 10);
    EXPECT_THROW(stack3x5(0, 5), TError);

    // Попытка загрузить из несуществующего файла
    EXPECT_THROW(TMultiStack<int> stack("nonexistent_file.bin"), TError);
}

// Тест с различными типами данных
TEST(TMultiStackDifferentTypes, VariousTypes) {
    // Тест с double
    TMultiStack<double> doubleStack(2, 3);
    doubleStack.Push(0, 3.14);
    doubleStack.Push(1, 2.71);
    EXPECT_DOUBLE_EQ(doubleStack(0, 0), 3.14);
    EXPECT_DOUBLE_EQ(doubleStack(1, 0), 2.71);

    // Тест с std::string
    TMultiStack<std::string> stringStack(2, 2);
    stringStack.Push(0, "hello");
    stringStack.Push(1, "world");
    EXPECT_EQ(stringStack(0, 0), "hello");
    EXPECT_EQ(stringStack(1, 0), "world");
}

// Тест граничных случаев
TEST_F(TMultiStackTest, EdgeCases) {
    // Стек с нулевой емкостью
    TMultiStack<int> zeroStack(0, 0);
    EXPECT_EQ(zeroStack.GetCountStacks(), 0);
    EXPECT_EQ(zeroStack.GetCapacity_M(), 0);
    EXPECT_EQ(zeroStack.GetSize_M(), 0);

    // Стек с одним элементом
    TMultiStack<int> singleStack(1, 1);
    singleStack.Push(0, 42);
    EXPECT_TRUE(singleStack.IsFull(0));
    EXPECT_EQ(singleStack.Pop(0), 42);
    EXPECT_TRUE(singleStack.IsEmpty(0));
}

// Тест переполнения мультистека
TEST_F(TMultiStackTest, MultiStackOverflow) {
    TMultiStack<int> stack(2, 2);
    
    // Заполняем полностью
    stack.Push(0, 1);
    stack.Push(0, 2);
    stack.Push(1, 3);
    stack.Push(1, 4);
    
    EXPECT_TRUE(stack.IsFull_M());
    
    // Попытка добавить в полный мультистек
    // Должен сработать репак, но если все стеки полны - исключение
    // В текущей реализации Repack проверяет IsFull_M() и бросает исключение
    EXPECT_THROW(stack.Push(0, 5), TError);
}

// Тест оператора вывода
TEST_F(TMultiStackTest, OutputOperator) {
    stack2x3.Push(0, 1);
    stack2x3.Push(0, 2);
    stack2x3.Push(1, 3);
    
    std::ostringstream oss;
    oss << stack2x3;
    
    // Проверяем форматированный вывод
    std::string result = oss.str();
    EXPECT_TRUE(result.find("[1,2]") != std::string::npos);
    EXPECT_TRUE(result.find("[3]") != std::string::npos);
    EXPECT_TRUE(result.find("{") != std::string::npos);
    EXPECT_TRUE(result.find("}") != std::string::npos);
}

// Тест метода GetSize_M
TEST_F(TMultiStackTest, GetSize_M) {
    EXPECT_EQ(stack3x5.GetSize_M(), 0);
    
    stack3x5.Push(0, 1);
    EXPECT_EQ(stack3x5.GetSize_M(), 1);
    
    stack3x5.Push(1, 2);
    stack3x5.Push(1, 3);
    EXPECT_EQ(stack3x5.GetSize_M(), 3);
    
    stack3x5.Push(2, 4);
    stack3x5.Push(2, 5);
    stack3x5.Push(2, 6);
    EXPECT_EQ(stack3x5.GetSize_M(), 6);
}

// Тест метода GetSizeOfStack
TEST_F(TMultiStackTest, GetSizeOfStack) {
    EXPECT_EQ(stack3x5.GetSizeOfStack(0), 0);
    EXPECT_EQ(stack3x5.GetSizeOfStack(1), 0);
    EXPECT_EQ(stack3x5.GetSizeOfStack(2), 0);
    
    stack3x5.Push(0, 1);
    stack3x5.Push(0, 2);
    EXPECT_EQ(stack3x5.GetSizeOfStack(0), 2);
    EXPECT_EQ(stack3x5.GetSizeOfStack(1), 0);
    
    stack3x5.Push(1, 3);
    EXPECT_EQ(stack3x5.GetSizeOfStack(1), 1);
}
