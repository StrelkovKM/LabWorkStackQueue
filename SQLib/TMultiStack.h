#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include "TError_Adv.h"
#define ERROR(err,...) throw TError(err, __func__, __FILE__, __LINE__)

template<typename T>
class TMultiStack
{
protected:
    size_t capacity;
    size_t count_stacks;

    T* data;
    size_t* begin_stacks;
    size_t* top_stacks;
    
    void Repack(size_t count_of_stack);
public:
    TMultiStack();
    TMultiStack(size_t count_stacks_, size_t capacity_stack);
    TMultiStack(const TMultiStack& other);
    TMultiStack(TMultiStack&& other) noexcept;
    ~TMultiStack();

    bool operator==(const TMultiStack& other) const;
    bool operator!=(const TMultiStack& other) const;

    T operator()(size_t count_of_stack, size_t index) const;
    
    TMultiStack& operator=(const TMultiStack& other);
    TMultiStack& operator=(TMultiStack&& other) noexcept;

    size_t GetCapacity_M() const;
    size_t GetCountStacks() const;
    size_t Size(size_t count_of_stack) const;

    bool IsFull(size_t count_of_stack) const;
    bool IsEmpty(size_t count_of_stack) const;

    void Push(size_t count_of_stack, const T& value);
    T Pop(size_t count_of_stack);

    T FindMin() const;
    void SaveToFile(const std::string& filename) const;
    void LoadFromFile(const std::string& filename);

    template<class O>
    friend std::ostream& operator<<(std::ostream& os, const TMultiStack<O>& stack);
};

template<typename T>
inline void TMultiStack<T>::Repack(size_t count_of_stack)
{
    if (count_of_stack >= count_stacks) ERROR("stack_error");

    size_t find_first_nofull = 0;
    for (size_t i = 0; i < count_stacks; ++i) {
        if (!this->IsFull(i)) {
            find_first_nofull = i;
            break;
        }
    }

    if (find_first_nofull == count_stacks) ERROR("no_empty_stacks");
    if (find_first_nofull < count_of_stack) {
        for (size_t i = begin_stacks[find_first_nofull + 1]; i < top_stacks[count_of_stack]; ++i)
            data[i - 1] = std::move(data[i]);
        for (size_t i = find_first_nofull + 1; i <= count_of_stack; ++i) {
            --begin_stacks[i];
            --top_stacks[i];
        }
    } else {
        for (size_t i = top_stacks[find_first_nofull] - 1; i >= begin_stacks[count_of_stack + 1]; --i)
            data[i + 1] = std::move(data[i]);
        
        for (size_t i = count_of_stack + 1; i <= find_first_nofull; ++i) {
            ++begin_stacks[i];
            ++top_stacks[i];
        }
    }
}

template<typename T>
inline TMultiStack<T>::TMultiStack() : capacity(0), count_stacks(0), data(nullptr), begin_stacks(nullptr), top_stacks(nullptr)
{
}

template<typename T>
inline TMultiStack<T>::TMultiStack(size_t count_stacks_, size_t capacity_stack)
    : capacity(count_stacks_ * capacity_stack), count_stacks(count_stacks_)
{
    if (capacity == 0 || count_stacks == 0) {
        data = nullptr;
        begin_stacks = nullptr;
        top_stacks = nullptr;
        return;
    }
    
    data = new T[capacity];
    begin_stacks = new size_t[count_stacks];
    top_stacks = new size_t[count_stacks];
    
    for (size_t i = 0; i < count_stacks; ++i) {
        begin_stacks[i] = i * capacity_stack;
        top_stacks[i] = i * capacity_stack;
    }
}

template<typename T>
inline TMultiStack<T>::TMultiStack(const TMultiStack& other)
    : capacity(other.capacity), count_stacks(other.count_stacks)
{
    if (capacity > 0) {
        data = new T[capacity];
        for (size_t i = 0; i < capacity; ++i)
            data[i] = other.data[i];
    } else {
        data = nullptr;
    }

    if (count_stacks > 0) {
        begin_stacks = new size_t[count_stacks];
        top_stacks = new size_t[count_stacks];
        for (size_t i = 0; i < count_stacks; ++i) {
            begin_stacks[i] = other.begin_stacks[i];
            top_stacks[i] = other.top_stacks[i];
        }
    } else {
        begin_stacks = nullptr;
        top_stacks = nullptr;
    }
}

template<typename T>
inline TMultiStack<T>::TMultiStack(TMultiStack&& other) noexcept
    : capacity(other.capacity), count_stacks(other.count_stacks),
      data(other.data), begin_stacks(other.begin_stacks), top_stacks(other.top_stacks)
{
    other.capacity = 0;
    other.count_stacks = 0;
    other.data = nullptr;
    other.begin_stacks = nullptr;
    other.top_stacks = nullptr;
}

template<typename T>
inline TMultiStack<T>::~TMultiStack()
{
    delete[] data;
    delete[] begin_stacks;
    delete[] top_stacks;
}

template<typename T>
inline bool TMultiStack<T>::operator==(const TMultiStack& other) const
{
    if (count_stacks != other.count_stacks || capacity != other.capacity)
        return false;
        
    for (size_t i = 0; i < count_stacks; ++i) {
        if (begin_stacks[i] != other.begin_stacks[i] || top_stacks[i] != other.top_stacks[i])
            return false;
            
        for (size_t j = begin_stacks[i]; j < top_stacks[i]; ++j) {
            if (data[j] != other.data[j])
                return false;
        }
    }
    return true;
}

template<class T>
inline bool TMultiStack<T>::operator!=(const TMultiStack& other) const
{
    return !(*this == other);
}

template<class T>
inline T TMultiStack<T>::operator()(size_t number_of_stacks, size_t index) const
{
    if (number_of_stacks >= count_stacks) ERROR("stacks_error");
    if (index >= this->Size(number_of_stacks)) ERROR("size_error");
    return data[begin_stacks[number_of_stacks] + index];
}

template<class T>
inline TMultiStack<T>& TMultiStack<T>::operator=(const TMultiStack<T>& other)
{
    if (this != &other) {
        TMultiStack temp(other);
        *this = std::move(temp);
    }
    return *this;
}

template<class T>
inline TMultiStack<T>& TMultiStack<T>::operator=(TMultiStack<T>&& other) noexcept
{
    if (this != &other) {
        delete[] data;
        delete[] begin_stacks;
        delete[] top_stacks;

        data = other.data;
        begin_stacks = other.begin_stacks;
        top_stacks = other.top_stacks;
        capacity = other.capacity;
        count_stacks = other.count_stacks;

        other.data = nullptr;
        other.begin_stacks = nullptr;
        other.top_stacks = nullptr;
        other.capacity = 0;
        other.count_stacks = 0;
    }
    return *this;
}

template<class T>
inline size_t TMultiStack<T>::GetCapacity_M() const
{
    return capacity;
}

template<class T>
inline size_t TMultiStack<T>::GetCountStacks() const
{
    return count_stacks;
}

template<class T>
inline size_t TMultiStack<T>::Size(size_t number_of_stack) const
{
    if (number_of_stack >= count_stacks) ERROR("stack_error");
    return top_stacks[number_of_stack] - begin_stacks[number_of_stack];
}

template<class T>
inline bool TMultiStack<T>::IsFull(size_t number_of_stack) const
{
    if (number_of_stack >= count_stacks) ERROR("stack_error");
    
    if (number_of_stack == count_stacks - 1)
        return top_stacks[number_of_stack] == capacity;
    else
        return top_stacks[number_of_stack] == begin_stacks[number_of_stack + 1];
}

template<class T>
inline bool TMultiStack<T>::IsEmpty(size_t number_of_stack) const
{
    if (number_of_stack >= count_stacks) ERROR("stack_error");
    return top_stacks[number_of_stack] == begin_stacks[number_of_stack];
}

template<class T>
inline void TMultiStack<T>::Push(size_t number_of_stack, const T& value)
{
    if (number_of_stack >= count_stacks) ERROR("stack_error");
    if (this->IsFull(number_of_stack)) this->Repack(number_of_stack); 
    data[top_stacks[number_of_stack]++] = value;
}

template<class T>
inline T TMultiStack<T>::Pop(size_t number_of_stack)
{
    if (number_of_stack >= count_stacks) ERROR("stack_error");
    if (this->IsEmpty(number_of_stack)) ERROR("empty_stack");
    
    return data[--top_stacks[number_of_stack]];
}

template<class O>
std::ostream& operator<<(std::ostream& out, const TMultiStack<O>& stack)
{
    out << "{ ";
    for (size_t i = 0; i < stack.GetCountStacks(); ++i) {
        out << "[";
        for (size_t j = 0; j < stack.Size(i); ++j) {
            out << stack(i, j);
            if (j < stack.Size(i) - 1) {
                out << ",";
            }
        }
        out << "]";
        if (i < stack.GetCountStacks() - 1) {
            out << ", ";
        }
    }
    out << " }";
    return out;
}

template<class T>
inline T TMultiStack<T>::FindMin() const
{
    if (capacity == 0 || count_stacks == 0)
        ERROR("empty_stack");

    bool found = false;
    T minElem;
    
    for (size_t i = 0; i < count_stacks; ++i) {
        if (!this->IsEmpty(i)) {
            minElem = data[begin_stacks[i]];
            found = true;
            break;
        }
    }

    if (!found) ERROR("all_stacks_empty");
    
    for (size_t i = 0; i < count_stacks; ++i) {
        if (!this->IsEmpty(i)) {
            for (size_t j = begin_stacks[i]; j < top_stacks[i]; ++j) {
                if (data[j] < minElem)
                    minElem = data[j];
            }
        }
    }

    return minElem;
}

template<class T>
inline void TMultiStack<T>::SaveToFile(const std::string& filename) const
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
        ERROR("cannot_open_file");
        
    file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
    file.write(reinterpret_cast<const char*>(&count_stacks), sizeof(count_stacks));
    
    if (capacity > 0) {
        for (size_t i = 0; i < capacity; ++i) {
            file.write(reinterpret_cast<const char*>(&data[i]), sizeof(T));
        }
    }
    
    if (count_stacks > 0) {
        for (size_t i = 0; i < count_stacks; ++i) {
            file.write(reinterpret_cast<const char*>(&begin_stacks[i]), sizeof(size_t));
        }
        for (size_t i = 0; i < count_stacks; ++i) {
            file.write(reinterpret_cast<const char*>(&top_stacks[i]), sizeof(size_t));
        }
    }

    file.close();
}

template<class T>
inline void TMultiStack<T>::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
        ERROR("cannot_open_file");

    size_t temp_capacity, temp_count_stacks;
    file.read(reinterpret_cast<char*>(&temp_capacity), sizeof(temp_capacity));
    file.read(reinterpret_cast<char*>(&temp_count_stacks), sizeof(temp_count_stacks));

    T* temp_data = nullptr;
    size_t* temp_begin_stacks = nullptr;
    size_t* temp_top_stacks = nullptr;

    try {
        if (temp_capacity > 0) {
            temp_data = new T[temp_capacity];
            for (size_t i = 0; i < temp_capacity; ++i) {
                file.read(reinterpret_cast<char*>(&temp_data[i]), sizeof(T));
            }
        }

        if (temp_count_stacks > 0) {
            temp_begin_stacks = new size_t[temp_count_stacks];
            temp_top_stacks = new size_t[temp_count_stacks];
            for (size_t i = 0; i < temp_count_stacks; ++i) {
                file.read(reinterpret_cast<char*>(&temp_begin_stacks[i]), sizeof(size_t));
            }
            for (size_t i = 0; i < temp_count_stacks; ++i) {
                file.read(reinterpret_cast<char*>(&temp_top_stacks[i]), sizeof(size_t));
            }
        }
    } catch (...) {
        delete[] temp_data;
        delete[] temp_begin_stacks;
        delete[] temp_top_stacks;
        throw;
    }

    delete[] data;
    delete[] begin_stacks;
    delete[] top_stacks;

    data = temp_data;
    begin_stacks = temp_begin_stacks;
    top_stacks = temp_top_stacks;
    capacity = temp_capacity;
    count_stacks = temp_count_stacks;

    file.close();
}