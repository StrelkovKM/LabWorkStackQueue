#pragma once

#include <iostream>
#include <initializer_list>

using namespace std;

#include "TString_Adv.h"
#include "TError_Adv.h"
#include "TQueue.h"
#include "TStack.h"

template<class T>
class TVector 
{
protected:
	T* data;
	size_t size;
	size_t capacity;

public:
	TVector();
	TVector(const size_t& size_);
	TVector(size_t size_, const T& element);
	TVector(initializer_list<T> init_list);
	TVector(const TVector& other);
	TVector(TVector&& other) noexcept;

	~TVector();

	size_t GetSize() const;
	size_t GetCapacity() const;

	void Reserve(size_t new_cap);
	void Resize(size_t new_size);

	void push_pos(size_t pos, const T& value);
	void push_back(const T& value);
	void push_front(const T& value);

	void pop_back();
	void pop_front();
	void pop_pos(size_t pos);

	void Reverse();
	bool IsEmpty() const noexcept;
	bool IsFull() const noexcept;

	T& operator[](const size_t index);
	const T& operator[](const size_t index) const;

	TVector& operator=(const TVector<T>& other);
	TVector& operator=(TVector<T>&& other) noexcept;
	TVector& operator=(initializer_list<T> init);


	T* begin() noexcept;
	const T* begin() const noexcept;
	const T* cbegin() const noexcept;

	T* end() noexcept;
	const T* end() const noexcept;
	const T* cend() const noexcept;

	T* rbegin() noexcept;
	const T* rbegin() const noexcept;
	const T* rcbegin() const noexcept;

	T* rend() noexcept;
	const T* rend() const noexcept;
	const T* rcend() const noexcept;

	TVector operator+(const TVector<T>& other);
	bool operator==(const TVector<T>& other) const;
	bool operator!=(const TVector<T>& other) const;

	template<class O>
	friend ostream& operator<<(ostream& out, TVector<O>& t);

};

template<class T>
inline TVector<T>::TVector()
{
	capacity = 0;
	size = 0;
	data = nullptr;
}

template<class T>
inline TVector<T>::TVector(const size_t& size_)
{
	if (size_ > 0)
	{
		capacity = size_;
		size = size_;
		data = new T[capacity]{};
		for (auto i = 0; i < size; i++) data[i] = T();
	}
	else if (size_ == 0)
	{
		capacity = 0;
		size = 0;
		data = nullptr;
	}
	else throw TError("size can't be < 0", __func__, __FILE__, __LINE__);
}

template<class T>
inline TVector<T>::TVector(const size_t size_, const T& element)
{
	if (size > 0)
	{
		capacity = size_;
		size = size_;
		data = new T[capacity]{};
		for (auto i = 0; i < size; i++) data[i] = element;
	}
	else
	{
		capacity = 0;
		size = 0;
		data = nullptr;
	}
}

template<class T>
inline TVector<T>::TVector(initializer_list<T> init) : capacity(init.size()), size(init.size())
{
	if (size > 0)
	{
		data = new T[capacity];
		T* dest = data;
		const T* src = init.begin();
		const T* end = init.end();

		while (src != end)
			*dest++ = *src++;
	}
	else
	{
		data = nullptr;
	}
}

template<class T>
inline TVector<T>::TVector(const TVector& other)
{
	if (other.data) 
	{
		capacity = other.capacity;
		size = other.size;
		data = new T[size]{};
		for (auto i = 0; i < size; i++) data[i] = other.data[i];
	}
	else
	{
		capacity = 0;
		size = 0;
		data = nullptr;
	}
}

template<class T>
inline TVector<T>::TVector(TVector&& other) noexcept
{
	capacity = other.capacity;
	size = other.size;
	data = other.data;
	other.capacity = 0;
	other.size = 0;
	other.data = nullptr;
}

template<class T>
inline size_t TVector<T>::GetSize() const
{
	return size;
}

template<class T>
inline size_t TVector<T>::GetCapacity() const
{
	return capacity;
}

template<class T>
inline TVector<T>::~TVector()
{
	delete[] data;
	capacity = 0;
	size = 0;
}

template<class T>
inline T& TVector<T>::operator[](const size_t index)
{
	if (index < size) return data[index];
	throw TError("index can't be more than size", __func__, __FILE__, __LINE__);
}

template<class T>
inline const T& TVector<T>::operator[](const size_t index) const
{
	if (index < size) return data[index];
	throw TError("index can't be more than size", __func__, __FILE__, __LINE__);
}

template<class T>
inline TVector<T>& TVector<T>::operator=(const TVector<T>& other)
{
	if (this != &other)
	{
		if (data) delete[] data;
		capacity = other.capacity;
		size = other.size;
		if (capacity)
		{
			data = new T[capacity]{};
			for (auto i = 0; i < size; i++) data[i] = other.data[i];
		}
		else data = nullptr;
	}
	return *this;
}

template<class T>
inline TVector<T>& TVector<T>::operator=(TVector<T>&& other) noexcept
{
	if (data) delete[] data;
	data = other.data;
	size = other.size;
	capacity = other.capacity;
	other.data = nullptr;
	other.size = 0;
	other.capacity = 0;
	return *this;
}

template<class T>
inline TVector<T>& TVector<T>::operator=(std::initializer_list<T> init)
{
	if (init.size() == 0)
	{
		capacity = 0;
		size = 0;
		data = nullptr;
	}
	else
	{
		delete[] data;
		size = init.size();
		capacity = init.size();
		data = new T[capacity];

		const T* src = init.begin();
		T* dest = data;
		const T* end = init.end();

		while (src != end)
			*dest++ = *src++;
	}
	return *this;
}

template<class T>
inline void TVector<T>::push_back(const T& value)
{
	if (size == capacity)
	{
		size_t newCapacity = (capacity == 0 ? 1 : capacity * 2);
		T* newData = new T[newCapacity];

		for (size_t i = 0; i < size; i++) newData[i] = std::move(data[i]);

		delete[] data;
		data = newData;
		capacity = newCapacity;

	}

	data[size] = value;
	size++;
}

template<class T>
inline void TVector<T>::push_front(const T& value)
{
	this->push_back(value);
	for (auto i = size - 1; i > 0; i--) data[i] = data[i - 1];
	data[0] = value;
}

template<class T>
inline void TVector<T>::pop_back()
{
	if (size == 0) {
		throw TError("this vector is empty", __func__, __FILE__, __LINE__);
	}
	size--;
}

template<class T>
inline void TVector<T>::pop_front()
{
	this->Reverse();
	this->pop_back();
	this->Reverse();
}

template<class T>
inline void TVector<T>::pop_pos(size_t pos)
{
	if (pos + 1 < size  && size * pos > 0)
	{
		TVector<T> buffer(size-1);
		for (auto i = 0; i < pos; i++) buffer[i] = data[i];
		for (auto i = pos + 1; i < size; i++) buffer[i - 1] = data[i];
		*this = move(buffer);
	}
	else if (pos == 0) pop_front();
	else if (pos == size-1) pop_back();
	else throw TError("Possition can't be more size-1 or size == 0", __func__, __FILE__, __LINE__);
}

template<class T>
inline void TVector<T>::Reverse()
{
	if (size > 1)
	{
		T* copy = new T[size];
		for (auto i = 0; i < size; i++) copy[i] = data[size - 1 - i];
		if (data) delete[] data;
		data = copy;
	}
}

template<class T>
inline bool TVector<T>::IsEmpty() const noexcept
{
	return size == 0;
}

template<class T>
inline bool TVector<T>::IsFull() const noexcept
{
	return size != 0;
}

template<class T>
inline void TVector<T>::Reserve(size_t new_cap)
{
	if (new_cap > capacity)
	{
		TVector<T> copy (*this);
		capacity = new_cap;
		size = copy.size;
		data = new T[capacity];
		for (size_t i = 0; i < size; ++i)
			data[i] = copy[i];
	}
}

template<class T>
inline void TVector<T>::Resize(size_t count)
{
	if (count < size)
		for (size_t i = count; i < size; ++i) pop_back();
	else
	{
		this->Reserve(count);
		for (size_t i = size; i < count; ++i)
			data[i] = T();
	}
	size = count;
}

template<class T>
inline void TVector<T>::push_pos(size_t pos, const T& value)
{
	if (pos > size) {
		throw TError("Position out of range", __func__, __FILE__, __LINE__);
	}
	if (size == capacity) {
		Reserve(capacity == 0 ? 1 : capacity * 2);
	}

	for (size_t i = size; i > pos; --i) {
		data[i] = std::move(data[i - 1]);
	}
	data[pos] = value;
	size++;
}

template<class T>
inline T* TVector<T>::begin() noexcept
{
	return data;
}

template<class T>
inline const T* TVector<T>::begin() const noexcept
{
	return data;
}

template<class T>
inline const T* TVector<T>::cbegin() const noexcept
{
	return data;
}

template<class T>
inline T* TVector<T>::end() noexcept
{
	return data + size;
}

template<class T>
inline const T* TVector<T>::end() const noexcept
{
	return data + size;
}

template<class T>
inline const T* TVector<T>::cend() const noexcept
{
	return data + size;
}

template<class T>
inline T* TVector<T>::rbegin() noexcept
{
	return data + size - 1;
}

template<class T>
inline const T* TVector<T>::rbegin() const noexcept
{
	return data + size - 1;
}

template<class T>
inline const T* TVector<T>::rcbegin() const noexcept
{
	return data + size - 1;
}

template<class T>
inline T* TVector<T>::rend() noexcept
{
	return data - 1;
}

template<class T>
inline const T* TVector<T>::rend() const noexcept
{
	return data - 1;
}

template<class T>
inline const T* TVector<T>::rcend() const noexcept
{
	return data - 1;
}

template<class T>
inline TVector<T> TVector<T>::operator+(const TVector<T>& other)
{
	if (data != nullptr && other.data != nullptr)
	{
		TVector<T> res(capacity + other.capacity);
		for (auto i = 0; i < size; i++) res[i] = data[i];
		for (auto i = size; i < size + other.size; i++) res[i] = other.data[i-size];
		return res;
	}
	else if (data == nullptr)
	{
		TVector<T> res;
		res = other;
		return res;
	}
	else return *this;
}

template<class T>
inline bool TVector<T>::operator==(const TVector<T>& other) const
{
	if (size != other.size) return false;
	for (auto i = 0; i < size; i++) if (data[i] != other.data[i]) return false;
	return true;
}

template<class T>
inline bool TVector<T>::operator!=(const TVector<T>& other) const
{
	if (*this == other) return false;
	return true;
}

template<class O>
inline std::ostream& operator<<(ostream& out, TVector<O>& other)
{
	out << "{ ";
	for (auto i = 0; i < other.size - 1; i++) out << other.data[i] << "; ";
	out << other.data[other.size-1] << " }";
	return out;
}