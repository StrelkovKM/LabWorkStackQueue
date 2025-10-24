#pragma once
#include <fstream>
#include <iostream>
#include <initializer_list>

#include "TError_Adv.h"
#include "TString_Adv.h"
#include "TVector_AdvImp.h"
#include "TStack.h"

template <typename T>
class TMultiStack {
protected:
	TVector<TStack<T>> data;
	void Repack();

public:
	TMultiStack();
	TMultiStack(const size_t& ñapacity_, const size_t& number_of_stacks);
	TMultiStack(const TVector<TStack<T>>& other);
	TMultiStack(TVector<TStack<T>>&& other) noexcept;

	size_t GetSize_M() const;
	size_t GetCapacity_M() const;

	void Put_M(const size_t& number_stack, const T& value);
	T Get_M(const size_t& number_stack);

	bool IsFull_M() const;
	bool IsEmpty_M() const;

	bool IsFullStack_M(const size_t& number_stack) const;
	bool IsEmptyStack_M(const size_t& number_stack) const;

	template<typename O>
	friend ostream& operator<<(ostream& out, const TMultiStack<O>& other);
};

template<typename T>
inline void TMultiStack<T>::Repack()
{
	size_t free_memory = 0;

	for (auto i = 0; i < data.GetSize(); i++) {
		free_memory += data[i].GetCapacity() - data[i].GetSize();
	}

	size_t free_remainder = free_memory % data.GetSize();

	for (auto i = 0; i < free_remainder; i++) {
		data[i].Reserve(data[i].GetCapacity() + ((data[i].GetSize() * free_memory) / GetSize_M()) + 1);
	}
	for (auto i = free_remainder; i < data.GetSize(); i++) {
		data[i].Reserve(data[i].GetCapacity() + ((data[i].GetSize() * free_memory) / GetSize_M()));
	}
}

template<typename T>
inline TMultiStack<T>::TMultiStack() : data() {}

template<typename T>
inline TMultiStack<T>::TMultiStack(const size_t& ñapacity_, const size_t& number_of_stacks)
{
	data.Reserve(number_of_stacks);
	size_t remainder = ñapacity_ % number_of_stacks;
	size_t capacities = ñapacity_ / number_of_stacks;

	for (auto i = 0; i < remainder; i++) {
		data.push_back(TStack<T>(capacities + 1));
	}
	for (auto i = remainder; i < number_of_stacks; i++) {
		data.push_back(TStack<T>(capacities));
	}
}

template<typename T>
inline TMultiStack<T>::TMultiStack(const TVector<TStack<T>>& other) : data(other) {}

template<typename T>
inline TMultiStack<T>::TMultiStack(TVector<TStack<T>>&& other) noexcept : data(std::move(other)) {}

template<typename T>
inline size_t TMultiStack<T>::GetSize_M() const
{
	size_t size = 0;
	for (auto i = 0; i < data.GetSize(); i++) {
		size += data[i].GetSize();
	}
	return size;
}

template<typename T>
inline size_t TMultiStack<T>::GetCapacity_M() const
{
	return data.GetSize();
}

template<typename T>
inline void TMultiStack<T>::Put_M(const size_t& number_stack, const T& value)
{
	if (IsFull_M()) throw TError("MultiStack is full", __func__, __FILE__, __LINE__);
	if (data[number_stack].IsFull()) Repack();
	data[number_stack].Put(value);
}

template<typename T>
inline T TMultiStack<T>::Get_M(const size_t& number_stack)
{
	if (IsEmpty_M()) {
		throw TError("MultiStack is empty", __func__, __FILE__, __LINE__);
	}
	else {
		return data[number_stack].Get();
	}
}

template<typename T>
inline bool TMultiStack<T>::IsFull_M() const
{
	for (auto i = 0; i < data.GetSize(); i++) {
		if (!data[i].IsFull()) return false;
	}
	return true;
}

template<typename T>
inline bool TMultiStack<T>::IsEmpty_M() const
{
	return data == T();
}

template<typename T>
inline bool TMultiStack<T>::IsFullStack_M(const size_t& number_stack) const
{
	return data[number_stack].IsFull();
}

template<typename T>
inline bool TMultiStack<T>::IsEmptyStack_M(const size_t& number_stack) const
{
	return data[number_stack].IsEmpty();
}

template<typename O>
inline ostream& operator<<(ostream& out, const TMultiStack<O>& other)
{
	out << "{ ";
	for (auto i = 0; i < other.data.GetSize() - 1; i++) {
		out << other.data[i] << "; ";
	}
	out << other.data[other.data.GetSize() - 1] << " }";
	return out;
}