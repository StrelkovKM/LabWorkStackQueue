#pragma once
#include <fstream>
#include <iostream>
#include <initializer_list>

#include "TError_Adv.h"
#include "TString_Adv.h"

template<class T>
class TQueue {
protected:
	size_t capacity;
	size_t head;
	size_t tail;
	size_t count;
	T* data;

public:
	TQueue();
	TQueue(size_t capacity_);
	TQueue(const TQueue<T>& other);
	TQueue(TQueue<T>&& other) noexcept;
	TQueue(const TString& filename);
	~TQueue();

	size_t GetSize();
	size_t GetHead();
	size_t GetTail();

	T Get();
	void Put(const T& value);

	bool IsFull() const;
	bool IsEmpty()const;

	TQueue& operator=(const TQueue<T>& other);
	TQueue& operator=(TQueue<T>&& other) noexcept;

	bool operator==(const TQueue<T>& other);
	bool operator!=(const TQueue<T>& other);

	T operator[](const size_t& index);
	const T operator[](const size_t& index) const;

	void SaveToFile(const TString& filename);

	
	T FindMin() const;

	template<class O>
	friend std::ostream& operator<<(std::ostream& out, const TQueue<O>& other);

	class TIterator {
	private:
		TQueue<T>* queue;
		size_t current_index;
		size_t steps;

	public:
		TIterator(TQueue<T>* q, size_t idx, size_t s = 0)
			: queue(q), current_index(idx), steps(s) {
		}

		T& operator*() {
			return queue->data[current_index];
		}

		T* operator->() {
			return &(queue->data[current_index]);
		}

		TIterator& operator++() {
			if (steps < queue->count) {
				current_index = (current_index + 1) % queue->capacity;
				steps++;
			}
			return *this;
		}

		TIterator operator++(int) {
			TIterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator==(const TIterator& other) const {
			return queue == other.queue && steps == other.steps;
		}

		bool operator!=(const TIterator& other) const {
			return !(*this == other);
		}
	};

	class TConstIterator {
	private:
		const TQueue<T>* queue;
		size_t current_index;
		size_t steps;

	public:
		TConstIterator(const TQueue<T>* q, size_t idx, size_t s = 0)
			: queue(q), current_index(idx), steps(s) {
		}

		const T& operator*() const {
			return queue->data[current_index];
		}

		const T* operator->() const {
			return &(queue->data[current_index]);
		}

		TConstIterator& operator++() {
			if (steps < queue->count) {
				current_index = (current_index + 1) % queue->capacity;
				steps++;
			}
			return *this;
		}

		TConstIterator operator++(int) {
			TConstIterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator==(const TConstIterator& other) const {
			return queue == other.queue && steps == other.steps;
		}

		bool operator!=(const TConstIterator& other) const {
			return !(*this == other);
		}
	};

	TIterator begin() noexcept {
		return TIterator(this, head, 0);
	}

	TIterator end() noexcept {
		return TIterator(this, tail, count);
	}

	TConstIterator begin() const noexcept {
		return TConstIterator(this, head, 0);
	}

	TConstIterator end() const noexcept {
		return TConstIterator(this, tail, count);
	}

	TConstIterator cbegin() const noexcept {
		return TConstIterator(this, head, 0);
	}

	TConstIterator cend() const noexcept {
		return TConstIterator(this, tail, count);
	}

};

template<class T>
inline TQueue<T>::TQueue() : capacity(0), head(0), tail(0), count(0), data(nullptr) {}

template<class T>
inline TQueue<T>::TQueue(size_t capacity_) : capacity (capacity_), head(0), tail(0), count(0)
{
	if (capacity == 0) data = nullptr;
	else data = new T[capacity];
}


template<class T>
inline TQueue<T>::TQueue(const TQueue<T>& other) : capacity(other.capacity), head(other.head), tail(other.tail), count(other.count)
{
	if (capacity == 0) data = nullptr;
	else {
		data = new T[capacity];

		if (head < tail) {
			for (auto i = head; i < tail; i++) data[i] = other.data[i];
		}
		else {
			for (auto i = head; i < capacity; i++) data[i] = other.data[i];
			for (auto i = 0; i < tail; i++) data[i] = other.data[i];
		}
	}
}

template<class T>
inline TQueue<T>::TQueue(TQueue<T>&& other) noexcept : capacity(other.capacity), head(other.head), tail(other.tail), count(other.count)
{
	data = other.data;
	other.data = nullptr;
	other.capacity = 0;
	other.head = 0;
	other.tail = 0;
	other.count = 0;
}

template<class T>
inline TQueue<T>::TQueue(const TString& filename)
{
	std::ifstream file(filename.CStr());

	if (!file.is_open()) throw TError("Cannot open file ", __func__, __FILE__, __LINE__);

	if (file.is_open()) {
		file >> capacity >> head >> tail >> count;
		if (capacity >= head && head == tail && count == 0) data = nullptr;
		else if (head < capacity && tail < capacity) {
			data = new T[capacity];
			if (head < tail && count == tail - head) {
				for (auto i = head; i < tail; i++) file >> data[i];
			}
			else if (head >= tail && (count == capacity - head + tail)) {
				for (auto i = head; i < capacity; i++) file >> data[i];
				for (auto i = 0; i < tail; i++) file >> data[i];
			}
		}
		else throw TError("Incorrect input", __func__, __FILE__, __LINE__);
		file.close();
	}
}

template<class T>
inline TQueue<T>::~TQueue()
{
	capacity = 0;
	head = 0;
	tail = 0;
	count = 0;
	if (data) delete[] data;
}


template<class T>
inline size_t TQueue<T>::GetSize()
{
	return count;
}

template<class T>
inline size_t TQueue<T>::GetHead()
{
	return data[head];
}

template<class T>
inline size_t TQueue<T>::GetTail()
{
	return data[tail-1];
}

template<class T>
inline T TQueue<T>::Get()
{
	if (!IsEmpty()) {
		T value = data[head];
		count--;
		head = (head + 1) % capacity;
		return value;
	}
	else throw TError("Queue is empty", __func__, __FILE__, __LINE__);
}


template<class T>
inline void TQueue<T>::Put(const T& value)
{
	if (!IsFull()) {
		count++;
		data[tail] = value;
		tail = (tail + 1) % capacity;
	}
	else throw TError("Stack is full", __func__, __FILE__, __LINE__);
}

template<class T>
inline bool TQueue<T>::IsEmpty() const
{
	return (count == 0);
}

template<class T>
inline bool TQueue<T>::IsFull() const
{
	return (count == capacity);
}

template<class T>
inline TQueue<T>& TQueue<T>::operator=(const TQueue<T>& other)
{
	if (this != &other) {
		if (data) delete[] data;
		count = other.count;
		head = other.head;
		tail = other.tail;
		capacity = other.capacity;
		if (capacity > 0) {
			data = new T[capacity];
			for (auto i = 0; i < capacity; ++i) data[i] = other.data[i];
		}
		else data = nullptr;
		return *this;
	}
	else return *this;
}


template<class T>
inline TQueue<T>& TQueue<T>::operator=(TQueue<T>&& other) noexcept
{
	if (this != &other) {
		if (data) delete[] data;

		data = other.data;
		capacity = other.capacity;
		head = other.head;
		tail = other.tail;
		count = other.count;

		other.data = nullptr;
		other.capacity = 0;
		other.head = 0;
		other.tail = 0;
		other.count = 0;
	}
	return *this;
}

template<class T>
inline bool TQueue<T>::operator==(const TQueue<T>& other)
{
	if (capacity != other.capacity || head != other.head || tail != other.tail || count != other.count) {
		return false;
	}
	else if (count == 0) return true;
	else if (head < tail) {
		for (auto i = head; i < tail; i++) {
			if (data[i] != other.data[i]) return false;
		}
		return true;
	}
	else {
		for (auto i = head; i < capacity; i++) {
			if (data[i] != other.data[i]) return false;
		}
		for (auto i = 0; i < tail; i++) {
			if (data[i] != other.data[i]) return false;
		}
		return true;
	}
}

template<class T>
inline bool TQueue<T>::operator!=(const TQueue<T>& other)
{
	return !(*this == other);
}

template<class T>
inline T TQueue<T>::operator[](const size_t& index)
{
	if (index >= capacity) {
		throw TError("Index out of range", __func__, __FILE__, __LINE__);
	}

	if (count == 0) {
		throw TError("Queue is empty", __func__, __FILE__, __LINE__);
	}

	if (head < tail) {
		if (index >= head && index < tail) {
			return data[index];
		}
	}
	else {
		if ((index >= head && index < capacity) || (index < tail)) {
			return data[index];
		}
	}

	throw TError("Index does not point to queue element", __func__, __FILE__, __LINE__);
}

template<class T>
inline const T TQueue<T>::operator[](const size_t& index) const
{
	if (index >= capacity) {
		throw TError("Index out of range", __func__, __FILE__, __LINE__);
	}
	if (count == 0) {
		throw TError("Queue is empty", __func__, __FILE__, __LINE__);
	}

	if (head < tail) {
		if (index >= head && index < tail) {
			return data[index];
		}
	}
	else {
		if ((index >= head && index < capacity) || (index < tail)) {
			return data[index];
		}
	}

	throw TError("Index does not point to queue element", __func__, __FILE__, __LINE__);
}

template<class T>
inline void TQueue<T>::SaveToFile(const TString& filename)
{
	if (count == 0) throw TError("Incorrect input", __func__, __FILE__, __LINE__);

	std::ofstream file(filename.CStr());

	if (!file.is_open()) throw TError("Cannot open file ", __func__, __FILE__, __LINE__);

	if (file.is_open())
	{
		file << capacity << " " << head << " " << tail << " " << count << "\n";
		if (head < tail) {
			for (auto i = head; i < tail; i++) file << data[i] << " ";
		}
		else {
			for (auto i = head; i < capacity; i++) file << data[i] << " ";
			for (auto i = 0; i < tail; i++) file << data[i] << " ";
		}
	} file.close();

}

template<class T>
inline T TQueue<T>::FindMin() const
{
	if (!(IsEmpty())) {
		T buffer = data[head];
		if (head < tail) {
			for (auto i = head; i < tail; i++) {
				if (data[i] < buffer) buffer = data[i];
			}
		}
		else {
			for (auto i = head; i < capacity; i++) {
				if (data[i] < buffer) buffer = data[i];
			}
			for (auto i = 0; i < tail; i++) {
				if (data[i] < buffer) buffer = data[i];
			}
		}
		return buffer;
	}
	else throw TError("Stack is empty", __func__, __FILE__, __LINE__);
}



template<class O>
inline std::ostream& operator<<(std::ostream& out, const TQueue<O>& other)
{
	out << "{ ";
	if (!other.IsEmpty()) {
		size_t current = other.head;
		for (size_t i = 0; i < other.count; i++) {
			out << other.data[current];
			if (i < other.count - 1) out << "; ";
			current = (current + 1) % other.capacity;
		}
	}
	out << " }";
	return out;
}