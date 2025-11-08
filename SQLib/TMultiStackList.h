#pragma once
#include <fstream>
#include <iostream>
#include <initializer_list>

#include "TError.hpp"
#include "TString_Adv.h"
#include "TVector_AdvImp.h"
#include "TStack.h"

template <typename T>
class TMultiStackList {
protected:
	TVector<TStack<T>> data;
	void Repack();
public:
	TMultiStackList();
	TMultiStackList(const size_t& capacity_, const size_t& number_of_stacks);
	TMultiStackList(const TMultiStackList<T>& other);
	TMultiStackList(TMultiStackList<T>&& other) noexcept;
	~TMultiStackList();

	size_t GetSize_M() const;
	size_t GetCapacity_M() const;
	size_t GetCountStacks() const;
	void SayStatusStack() const;

	void Put_M(const size_t& number_stack, const T& value);
	T Get_M(const size_t& number_stack);

	void PushStack_M(const size_t& number_stack, const size_t& capacity_stack);
	void PopStack_M(const size_t& number_stack);

	bool IsFull_M() const;
	bool IsEmpty_M() const;

	bool IsFullStack_M(const size_t& number_stack) const;
	bool IsEmptyStack_M(const size_t& number_stack) const;

	TMultiStackList& operator=(const TMultiStackList<T>& other);
	TMultiStackList& operator=(TMultiStackList<T>&& other) noexcept;

	bool operator==(const TMultiStackList<T>& other);
	bool operator!=(const TMultiStackList<T>& other);

	template<typename O>
	friend ostream& operator<<(ostream& out, const TMultiStackList<O>& other);
};

template<typename T>
inline void TMultiStackList<T>::Repack()
{
	size_t free_memory = 0;

	for (auto i = 0; i < data.GetSize(); i++) {
		free_memory += data[i].GetCapacity() - data[i].GetSize();
	}

	size_t free_remainder = free_memory % data.GetSize();

	for (auto i = 0; i < free_remainder; i++) {
		data[i].Reserve(data[i].GetSize() + ((data[i].GetSize() * free_memory) / GetSize_M()) + 1);
	}
	for (auto i = free_remainder; i < data.GetSize(); i++) {
		data[i].Reserve(data[i].GetSize() + ((data[i].GetSize() * free_memory) / GetSize_M()));
	}
}

template<typename T>
inline TMultiStackList<T>::TMultiStackList() : data() {}

template<typename T>
inline TMultiStackList<T>::TMultiStackList(const size_t& capacity_, const size_t& number_of_stacks)
{
	if (number_of_stacks != 0) {
		data.Reserve(number_of_stacks);
		size_t remainder = capacity_ % number_of_stacks;
		size_t capacities = capacity_ / number_of_stacks;

		for (auto i = 0; i < remainder; i++) {
			data.push_back(TStack<T>(capacities + 1));
		}
		for (auto i = remainder; i < number_of_stacks; i++) {
			data.push_back(TStack<T>(capacities));
		}
	}
	else {
		data.Reserve(capacity_);
	}
}

template<typename T>
inline TMultiStackList<T>::TMultiStackList(const TMultiStackList<T>& other) : data(other.data) {}

template<typename T>
inline TMultiStackList<T>::TMultiStackList(TMultiStackList<T>&& other) noexcept : data(std::move(other.data)) {}

template<typename T>
inline TMultiStackList<T>::~TMultiStackList()
{
}

template<typename T>
inline size_t TMultiStackList<T>::GetSize_M() const
{
	size_t size = 0;
	for (auto i = 0; i < data.GetSize(); i++) {
		size += data[i].GetSize();
	}
	return size;
}

template<typename T>
inline size_t TMultiStackList<T>::GetCapacity_M() const
{
	size_t size = 0;
	for (auto i = 0; i < data.GetSize(); i++) {
		size += data[i].GetCapacity();
	}
	return size;
}

template<typename T>
inline size_t TMultiStackList<T>::GetCountStacks() const
{
	return data.GetSize();
}

template<typename T>
inline void TMultiStackList<T>::SayStatusStack() const
{
	if (IsEmpty_M()) std::cout << "MultyStack is empty\n";
	else if (IsFull_M()) std::cout << "MultiStack is full\n";
	else {
		for (auto i = 0; i < data.GetSize(); i++) {
			if (data[i].IsEmpty()) cout << "Stack " << i << " is empty (" << data[i].GetCapacity() << " free space)\n";
			else if (data[i].IsFull()) cout << "Stack " << i << " is full\n";
			else cout << "Stack " << i << " has " << data[i].GetCapacity() - data[i].GetSize() << " free space\n";
		}
	}

}

template<typename T>
inline void TMultiStackList<T>::Put_M(const size_t& number_stack, const T& value)
{
	if (IsFull_M()) throw TError("MultiStack is full", __func__, __FILE__, __LINE__);
	if (data[number_stack].IsFull()) Repack();
	data[number_stack].Put(value);
}

template<typename T>
inline T TMultiStackList<T>::Get_M(const size_t& number_stack)
{
	if (IsEmpty_M()) {
		throw TError("MultiStack is empty", __func__, __FILE__, __LINE__);
	}
	else {
		return data[number_stack].Get();
	}
}

template<typename T>
inline void TMultiStackList<T>::PushStack_M(const size_t& number_stack, const size_t& capacity_stack)
{
	if (IsFull_M()) throw TError("MultiStack is full", __func__, __FILE__, __LINE__);
	else if (GetCapacity_M() < capacity_stack) {
		throw TError("MultiStack can't take it", __func__, __FILE__, __LINE__);
	}

	size_t searh_capacity_stack = 0;

	while (searh_capacity_stack != capacity_stack) {
		for (auto i = 0; i < data.GetSize(); i++) {
			if (!IsFullStack_M(i)) {
				data[i].Reserve(data[i].GetCapacity() - 1);
				searh_capacity_stack++;
			}
			if (searh_capacity_stack == capacity_stack) break;
		}
	}
	data.push_pos(number_stack, TStack<T>(capacity_stack));
}

template<typename T>
inline void TMultiStackList<T>::PopStack_M(const size_t& number_stack)
{
	size_t buffer = data[number_stack].GetCapacity();
	data.pop_pos(number_stack);
	while (buffer != 0) {
		for (auto i = 0; i < data.GetSize(); i++) {
			data[i].Reserve(data[i].GetCapacity() + 1);
			buffer--;
			if (buffer == 0) break;
		}
	}
}

template<typename T>
inline bool TMultiStackList<T>::IsFull_M() const
{
	for (auto i = 0; i < data.GetSize(); i++) {
		if (!data[i].IsFull()) return false;
	}
	return true;
}

template<typename T>
inline bool TMultiStackList<T>::IsEmpty_M() const
{
	if (data.IsEmpty()) return true;
	else {
		for (auto i = 0; i < data.GetSize(); i++) {
			if (!data[i].IsEmpty()) return false;
		}
		return true;
	}
}

template<typename T>
inline bool TMultiStackList<T>::IsFullStack_M(const size_t& number_stack) const
{
	return data[number_stack].IsFull();
}

template<typename T>
inline bool TMultiStackList<T>::IsEmptyStack_M(const size_t& number_stack) const
{
	return data[number_stack].IsEmpty();
}

template<typename T>
inline TMultiStackList<T>& TMultiStackList<T>::operator=(const TMultiStackList<T>& other)
{
	data = other.data;
}

template<typename T>
inline TMultiStackList<T>& TMultiStackList<T>::operator=(TMultiStackList<T>&& other) noexcept
{
	data = std::move(other.data);
}

template<typename T>
inline bool TMultiStackList<T>::operator==(const TMultiStackList<T>& other)
{
	return data == other.data;
}

template<typename T>
inline bool TMultiStackList<T>::operator!=(const TMultiStackList<T>& other)
{
	return data != other.data;
}

template<typename O>
inline ostream& operator<<(ostream& out, const TMultiStackList<O>& other)
{
	out << "{ ";
	for (auto i = 0; i < other.data.GetSize() - 1; i++) {
		out << other.data[i] << "; ";
	}
	out << other.data[other.data.GetSize() - 1] << " }";
	return out;
}