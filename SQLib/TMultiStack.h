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
	TMultiStack(TVector<TStack<T>>&& other);

	void PutM(const size_t& number_stack, const T& value);

	template<typename O>
	friend ostream& operator<<(ostream& out, const TMultiStack<O>& other);
};

template<typename T>
inline void TMultiStack<T>::Repack()
{
	
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
inline TMultiStack<T>::TMultiStack(TVector<TStack<T>>&& other) : data(std::move(other)) {}

template<typename T>
inline void TMultiStack<T>::PutM(const size_t& number_stack, const T& value)
{
	data[number_stack].Put(value);
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
