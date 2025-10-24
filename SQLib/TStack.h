#pragma once
#include <fstream>
#include <iostream>
#include <initializer_list>

#include "TError_Adv.h"
#include "TString_Adv.h"

template<class T>
class TStack {
protected:
	size_t capacity;
	size_t top;
	T* data;

public:
	TStack();
	TStack(const size_t& capacity_);
	TStack(std::initializer_list<T> init_list, size_t capacity_);
	TStack(const TStack<T>& other);
	TStack(TStack<T>&& other) noexcept;
	TStack(const TString& filename);
	~TStack();

	size_t GetSize() const;
	size_t GetCapacity() const;

	T Get();
	void Put(const T& value);

	void Reserve(const size_t& new_capacity);

	T* begin() noexcept;
	const T* begin() const noexcept;
	const T* cbegin() const noexcept;
	T* end() noexcept;
	const T* end() const noexcept;
	const T* cend() const noexcept;

	bool IsFull() const;
	bool IsEmpty() const;

	TStack& operator=(const TStack<T>& other);
	TStack& operator=(TStack<T>&& other) noexcept;

	bool operator==(const TStack<T>& other);
	bool operator!=(const TStack<T>& other);

	T operator[](const size_t& index);
	const T operator[](const size_t& index) const;

	void SaveToFile(const TString& filename);

	T FindMin();

	template<class O>
	friend ostream& operator<<(ostream& out, const TStack<O>& other);
};

template<class T>
inline TStack<T>::TStack() : capacity(0), top(0), data(nullptr) {}

template<class T>
inline TStack<T>::TStack(const size_t& capacity_) : capacity(capacity_), top(0), data(new T[capacity]) {}


template<class T>
inline TStack<T>::TStack(std::initializer_list<T> init_list, size_t capacity_)
{
	if ( init_list.size() <= capacity_) {
		top = init_list.size();
		capacity = capacity_;

		if (capacity == 0) {
			data = nullptr;
		}
		else {
			size_t index = 0;
			data = new T[capacity];
			for (const auto& elem : init_list) data[index++] = elem;
		}
	}
	else throw TError("Incorrect input", __func__, __FILE__, __LINE__);
}

template<class T>
inline TStack<T>::TStack(const TStack<T>& other)
{
	capacity = other.capacity;

	if (capacity == 0) {
		top = 0;
		data = nullptr;
	}
	else {
		top = other.top;
		data = new T[capacity];
		for (auto i = 0; i < top; i++) {
			data[i] = other.data[i];
		}
	}
}

template<class T>
inline TStack<T>::TStack(TStack<T>&& other) noexcept
{
	capacity = other.capacity;
	top = other.top;
	data = other.data;

	other.capacity = 0;
	other.top = 0;
	other.data = nullptr;
}

template<class T>
inline TStack<T>::TStack(const TString& filename)
{
	std::ifstream file(filename.CStr());

	if (!file.is_open()) throw TError("Cannot open file ", __func__, __FILE__, __LINE__);

	if (file.is_open()) {
		file >> capacity >> top;
		if (top <= capacity) {
			data = new T[capacity];
			for (auto i = 0; i < top; i++) file >> data[i];
		}
		else throw TError("Incorrect input", __func__, __FILE__, __LINE__);
		file.close();
	}
}

template<class T>
inline TStack<T>::~TStack()
{
	capacity = 0;
	top = 0;
	if (data != nullptr) delete[] data;
}

template<class T>
inline size_t TStack<T>::GetSize() const
{
	return top;
}

template<class T>
inline size_t TStack<T>::GetCapacity() const
{
	return capacity;
}


template<class T>
inline T TStack<T>::Get()
{
	if (!IsEmpty()) {
		return data[--top];
	}
	else throw TError("Stack is empty", __func__, __FILE__, __LINE__); 
}

template<class T>
inline void TStack<T>::Put(const T& value)
{
	if ( IsFull() ) {
		throw TError("Stack is full", __func__, __FILE__, __LINE__);
	}
	else {
		data[top++] = value;
	}
}

template<class T>
inline void TStack<T>::Reserve(const size_t& new_capacity)
{
	if (capacity == new_capacity) return;
	else if (top <= new_capacity) {
		TStack<T> buff (*this);
		if (data) delete[] data;
		data = new T[new_capacity];
		for (auto i = 0; i < buff.GetSize(); i++) data[i] = buff[i];
		capacity = new_capacity;
	}
	else throw TError("Incorrect input", __func__, __FILE__, __LINE__);
}


template<class T>
inline T* TStack<T>::begin() noexcept
{
	return data;
}

template<class T>
inline const T* TStack<T>::begin() const noexcept
{
	return data;
}

template<class T>
inline const T* TStack<T>::cbegin() const noexcept
{
	return data;
}

template<class T>
inline T* TStack<T>::end() noexcept
{
	return data + top;
}

template<class T>
inline const T* TStack<T>::end() const noexcept
{
	return data + top;
}

template<class T>
inline const T* TStack<T>::cend() const noexcept
{
	return data + top;
}
template<class T> 
inline bool TStack<T>::IsFull() const
{
	return (capacity == top && top != 0);
}

template<class T>
inline bool TStack<T>::IsEmpty() const
{
	return top == 0;
}

template<class T>
inline TStack<T>& TStack<T>::operator=(const TStack<T>& other)
{
	if (this != &other) {
		if (data != nullptr) delete[] data;
		capacity = other.capacity;
		top = other.top;
		if (capacity == 0) return *this;
		else {
			data = new T[capacity];
			for (auto i = 0; i < top; i++) data[i] = other.data[i];
			return *this;
		}
	}
	else return *this;
}

template<class T>
inline TStack<T>& TStack<T>::operator=(TStack<T>&& other) noexcept
{
	if (this != &other) {
		if (data != nullptr) delete[] data;
		capacity = other.capacity;
		top = other.top;
		data = other.data;

		other.capacity = 0;
		other.top = 0;
		other.data = nullptr;
	}
	return *this;
}

template<class T>
inline bool TStack<T>::operator==(const TStack<T>& other)
{
	if (capacity == other.capacity && top == other.top) {
		if (capacity != 0) {
			for (auto i = 0; i < top; i++) {
				if (data[i] != other.data[i]) return false;
			}
		}
		return true;
	}
	else return false;
}

template<class T>
inline bool TStack<T>::operator!=(const TStack<T>& other)
{
	return !(*this == other);
}

template<class T>
inline T TStack<T>::operator[](const size_t& index)
{
	if (index < top) return data[index];
	else throw TError("Incorrect input", __func__, __FILE__, __LINE__);
}

template<class T>
inline const T TStack<T>::operator[](const size_t& index) const
{
	if (index < top) return data[index];
	else throw TError("Incorrect input", __func__, __FILE__, __LINE__);
}

template<class T>
inline void TStack<T>::SaveToFile(const TString& filename)
{
	std::ofstream file(filename.CStr());

	if (!file.is_open()) throw TError("Cannot open file ", __func__, __FILE__, __LINE__);

	if (file.is_open())
	{
		file << capacity << " " << top << "\n";
		for (auto i = 0; i < top; i++) file << data[i] << " ";
	}
	file.close();
}

template<class T>
inline T TStack<T>::FindMin()
{
	if ( !(IsEmpty()) ) {
		T buffer = data[0];
		for (auto i = 0; i < top; i++) {
			if (buffer > data[i]) buffer = data[i];
		}
		return buffer;
	}
	else throw TError("Stack is empty", __func__, __FILE__, __LINE__);
}

template<class O>
inline ostream& operator<<(ostream& out, const TStack<O>& other)
{
	out << "{ ";
	if ( !(other.IsEmpty()) ) {
		for (auto i = 0; i < other.top - 1; i++) {
			std::cout << other.data[i] << "; ";
		}
		out << other.data[other.top - 1];
	}
	out << " }";
	return out;
}
