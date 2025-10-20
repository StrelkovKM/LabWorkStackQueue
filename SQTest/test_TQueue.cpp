#include <gtest.h>
#include <fstream>
#include "TQueue.h"

class TQueueTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Создаем тестовые файлы
    std::ofstream file1("test1.txt");
    if (file1.is_open()) {
      file1 << "5 0 3 3\n1 2 3";
      file1.close();
    }

    std::ofstream file2("test2.txt");
    if (file2.is_open()) {
      file2 << "4 2 1 3\n7 8 9";
      file2.close();
    }

    std::ofstream empty_file("empty.txt");
    if (empty_file.is_open()) {
      empty_file << "5 0 0 0";
      empty_file.close();
    }
  }

  void TearDown() override {
    // Удаляем тестовые файлы
    remove("test1.txt");
    remove("test2.txt");
    remove("empty.txt");
    remove("output.txt");
  }
};

// Тест конструктора по умолчанию
TEST_F(TQueueTest, DefaultConstructor) {
  TQueue<int> queue;
  EXPECT_TRUE(queue.IsEmpty());
  EXPECT_EQ(queue.GetSize(), 0);
}

// Тест конструктора с capacity
TEST_F(TQueueTest, CapacityConstructor) {
  TQueue<int> queue(5);
  EXPECT_TRUE(queue.IsEmpty());
  EXPECT_FALSE(queue.IsFull());
  EXPECT_EQ(queue.GetSize(), 0);

  // Тест с нулевой capacity
  TQueue<int> zero_queue(0);
  EXPECT_TRUE(zero_queue.IsEmpty());
  EXPECT_TRUE(zero_queue.IsFull());
}

// Тест операций Put и Get
TEST_F(TQueueTest, PutAndGet) {
  TQueue<int> queue(3);

  queue.Put(1);
  queue.Put(2);
  queue.Put(3);

  EXPECT_TRUE(queue.IsFull());
  EXPECT_EQ(queue.GetSize(), 3);

  EXPECT_EQ(queue.Get(), 1);
  EXPECT_EQ(queue.Get(), 2);
  EXPECT_EQ(queue.Get(), 3);

  EXPECT_TRUE(queue.IsEmpty());
}

// Тест кольцевого поведения
TEST_F(TQueueTest, CircularBehavior) {
  TQueue<int> queue(3);

  queue.Put(1);
  queue.Put(2);
  queue.Put(3);

  EXPECT_EQ(queue.Get(), 1);  // Освобождается место

  queue.Put(4);  // Должно встать на место первого элемента

  EXPECT_EQ(queue.Get(), 2);
  EXPECT_EQ(queue.Get(), 3);
  EXPECT_EQ(queue.Get(), 4);
}

// Тест конструктора копирования
TEST_F(TQueueTest, CopyConstructor) {
  TQueue<int> original(3);
  original.Put(1);
  original.Put(2);

  TQueue<int> copy(original);

  EXPECT_EQ(original.GetSize(), copy.GetSize());
  EXPECT_EQ(original.Get(), copy.Get());
  EXPECT_EQ(original.Get(), copy.Get());
}

// Тест перемещающего конструктора
TEST_F(TQueueTest, MoveConstructor) {
  TQueue<int> original(3);
  original.Put(1);
  original.Put(2);

  size_t original_size = original.GetSize();

  TQueue<int> moved(std::move(original));

  EXPECT_EQ(moved.GetSize(), original_size);
  EXPECT_TRUE(original.IsEmpty());  // original должен быть пустым после перемещения
}

// Тест конструктора из файла
TEST_F(TQueueTest, FileConstructor) {
  TQueue<int> queue("test1.txt");

  EXPECT_EQ(queue.GetSize(), 3);
  EXPECT_EQ(queue.Get(), 1);
  EXPECT_EQ(queue.Get(), 2);
  EXPECT_EQ(queue.Get(), 3);
}


// Тест конструктора из пустого файла
TEST_F(TQueueTest, FileConstructorEmpty) {
  TQueue<int> queue("empty.txt");

  EXPECT_TRUE(queue.IsEmpty());
  EXPECT_EQ(queue.GetSize(), 0);
}

// Тест оператора присваивания копированием
TEST_F(TQueueTest, CopyAssignment) {
  TQueue<int> original(3);
  original.Put(1);
  original.Put(2);

  TQueue<int> copy;
  copy = original;

  EXPECT_EQ(original.GetSize(), copy.GetSize());
  EXPECT_EQ(original.Get(), copy.Get());
  EXPECT_EQ(original.Get(), copy.Get());
}

// Тест оператора присваивания перемещением
TEST_F(TQueueTest, MoveAssignment) {
  TQueue<int> original(3);
  original.Put(1);
  original.Put(2);

  size_t original_size = original.GetSize();

  TQueue<int> moved;
  moved = std::move(original);

  EXPECT_EQ(moved.GetSize(), original_size);
  EXPECT_TRUE(original.IsEmpty());
}

// Тест оператора равенства
TEST_F(TQueueTest, EqualityOperator) {
  TQueue<int> queue1(3);
  queue1.Put(1);
  queue1.Put(2);

  TQueue<int> queue2(3);
  queue2.Put(1);
  queue2.Put(2);

  TQueue<int> queue3(3);
  queue3.Put(3);
  queue3.Put(4);

  EXPECT_TRUE(queue1 == queue2);
  EXPECT_FALSE(queue1 == queue3);
}

// Тест оператора неравенства
TEST_F(TQueueTest, InequalityOperator) {
  TQueue<int> queue1(3);
  queue1.Put(1);
  queue1.Put(2);

  TQueue<int> queue2(3);
  queue2.Put(3);
  queue2.Put(4);

  EXPECT_TRUE(queue1 != queue2);
  EXPECT_FALSE(queue1 != queue1);
}

// Тесты для физической индексации
TEST_F(TQueueTest, PhysicalIndexing) {
  TQueue<int> queue(5);

  // Заполняем очередь: [10, 20, 30]
  queue.Put(10);
  queue.Put(20);
  queue.Put(30);

  // Проверяем доступ к элементам по физическим индексам
  // head=0, tail=3, элементы в data[0], data[1], data[2]
  EXPECT_EQ(queue[0], 10);
  EXPECT_EQ(queue[1], 20);
  EXPECT_EQ(queue[2], 30);

  // Проверяем исключения для невалидных индексов
  EXPECT_THROW(queue[3], TError);  // Индекс за пределами элементов
  EXPECT_THROW(queue[4], TError);  // Индекс за пределами элементов
  EXPECT_THROW(queue[100], TError); // Индекс за пределами capacity

  // Извлекаем элемент
  queue.Get();  // Удаляем 10, теперь head=1, tail=3

  // Проверяем, что data[0] больше недоступен
  EXPECT_THROW(queue[0], TError);

  // Проверяем доступные элементы
  EXPECT_EQ(queue[1], 20);
  EXPECT_EQ(queue[2], 30);

  // Добавляем элементы для создания кольцевой структуры
  queue.Put(40);
  queue.Put(50);  // Теперь head=1, tail=0, count=4

  // Проверяем доступ к элементам в кольцевой структуре
  // Элементы: data[1]=20, data[2]=30, data[3]=40, data[4]=50
  EXPECT_EQ(queue[1], 20);
  EXPECT_EQ(queue[2], 30);
  EXPECT_EQ(queue[3], 40);
  EXPECT_EQ(queue[4], 50);

  // Проверяем, что data[0] недоступен (пустой)
  EXPECT_THROW(queue[0], TError);
}

TEST_F(TQueueTest, PhysicalIndexingEmptyQueue) {
  TQueue<int> queue(3);

  // Проверяем исключение при обращении к любому индексу пустой очереди
  EXPECT_THROW(queue[0], TError);
  EXPECT_THROW(queue[1], TError);
  EXPECT_THROW(queue[2], TError);
}

TEST_F(TQueueTest, PhysicalIndexingFullCircular) {
  TQueue<int> queue(3);

  // Создаем кольцевую структуру
  queue.Put(1);
  queue.Put(2);
  queue.Put(3);
  // head=0, tail=0, count=3

  // Получаем элементы в правильном порядке
  EXPECT_EQ(queue[0], 1);
  EXPECT_EQ(queue[1], 2);
  EXPECT_EQ(queue[2], 3);

  // Извлекаем один элемент
  queue.Get();  // head=1, tail=0, count=2

  // Проверяем доступные элементы
  EXPECT_THROW(queue[0], TError);  // Больше не принадлежит очереди
  EXPECT_EQ(queue[1], 2);
  EXPECT_EQ(queue[2], 3);

  // Добавляем новый элемент
  queue.Put(4);  // head=1, tail=1, count=3

  // Проверяем новое расположение
  EXPECT_EQ(queue[1], 2);
  EXPECT_EQ(queue[2], 3);
  EXPECT_EQ(queue[0], 4);  // Новый элемент в data[0]
}

TEST_F(TQueueTest, PhysicalIndexingEdgeCases) {
  TQueue<int> queue(4);

  // Тестируем граничные случаи
  queue.Put(1);
  queue.Put(2);
  // head=0, tail=2

  EXPECT_EQ(queue[0], 1);
  EXPECT_EQ(queue[1], 2);
  EXPECT_THROW(queue[2], TError);
  EXPECT_THROW(queue[3], TError);

  // Создаем случай, когда head > tail
  queue.Get();  // head=1, tail=2
  queue.Put(3);
  queue.Put(4);  // head=1, tail=0

  EXPECT_THROW(queue[0], TError);  // Пустой
  EXPECT_EQ(queue[1], 2);
  EXPECT_EQ(queue[2], 3);
  EXPECT_EQ(queue[3], 4);
}

TEST_F(TQueueTest, PhysicalIndexingConstVersion) {
  const TQueue<int> queue = []() {
    TQueue<int> q(3);
    q.Put(10);
    q.Put(20);
    return q;
    }();

  // Проверяем константную версию оператора []
  EXPECT_EQ(queue[0], 10);
  EXPECT_EQ(queue[1], 20);
  EXPECT_THROW(queue[2], TError);
}

// Тест для проверки, что оператор не модифицирует очередь
TEST_F(TQueueTest, PhysicalIndexingNonModifying) {
  TQueue<int> queue(3);
  queue.Put(1);
  queue.Put(2);

  size_t initial_size = queue.GetSize();

  // Многократный доступ не должен влиять на состояние
  queue[0];
  queue[1];

  EXPECT_EQ(queue.GetSize(), initial_size);
  EXPECT_FALSE(queue.IsEmpty());
}

// Тест сохранения в файл
TEST_F(TQueueTest, SaveToFile) {
  TQueue<int> queue(3);
  queue.Put(1);
  queue.Put(2);
  queue.Put(3);

  queue.SaveToFile("output.txt");

  // Проверяем, что файл создан и содержит правильные данные
  TQueue<int> loaded_queue("output.txt");

  EXPECT_EQ(loaded_queue.GetSize(), 3);
  EXPECT_EQ(loaded_queue.Get(), 1);
  EXPECT_EQ(loaded_queue.Get(), 2);
  EXPECT_EQ(loaded_queue.Get(), 3);
}

// Тест поиска минимума
TEST_F(TQueueTest, FindMin) {
  TQueue<int> queue(5);
  queue.Put(5);
  queue.Put(2);
  queue.Put(8);
  queue.Put(1);
  queue.Put(4);

  EXPECT_EQ(queue.FindMin(), 1);

  // После удаления некоторых элементов
  queue.Get();  // Удаляем 5
  queue.Get();  // Удаляем 2
  EXPECT_EQ(queue.FindMin(), 1);  // Минимум все еще 1

  queue.Get();  // Удаляем 8
  queue.Get();  // Удаляем 1
  EXPECT_EQ(queue.FindMin(), 4);  // Теперь минимум 4
}

// Тест оператора вывода
TEST_F(TQueueTest, OutputOperator) {
  TQueue<int> queue(3);
  queue.Put(1);
  queue.Put(2);
  queue.Put(3);

  std::stringstream ss;
  ss << queue;

  EXPECT_EQ(ss.str(), "{ 1; 2; 3 }");
}

// Тест исключений
TEST_F(TQueueTest, Exceptions) {
  TQueue<int> empty_queue;

  // Попытка Get из пустой очереди
  EXPECT_THROW(empty_queue.Get(), TError);

  // Попытка Put в полную очередь
  TQueue<int> full_queue(1);
  full_queue.Put(1);
  EXPECT_THROW(full_queue.Put(2), TError);

  // Попытка доступа по неверному индексу
  EXPECT_THROW(empty_queue[0], TError);

  // Попытка FindMin в пустой очереди
  EXPECT_THROW(empty_queue.FindMin(), TError);

  // Попытка создания из несуществующего файла
  EXPECT_THROW(TQueue<int> queue("nonexistent.txt"), TError);
}

// Тест с пользовательским типом
struct TestStruct {
  int value;
  bool operator<(const TestStruct& other) const { return value < other.value; }
  bool operator==(const TestStruct& other) const { return value == other.value; }

  friend std::ostream& operator<<(std::ostream& os, const TestStruct& ts) {
    return os << ts.value;
  }
};

TEST_F(TQueueTest, CustomType) {
  TQueue<TestStruct> queue(3);
  queue.Put(TestStruct{ 3 });
  queue.Put(TestStruct{ 1 });
  queue.Put(TestStruct{ 2 });

  EXPECT_EQ(queue.FindMin().value, 1);
  EXPECT_EQ(queue.Get().value, 3);
  EXPECT_EQ(queue.Get().value, 1);
}